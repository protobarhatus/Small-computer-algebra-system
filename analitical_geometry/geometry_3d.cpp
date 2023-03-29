#include "geometry_3d.h"
#include <QIODevice>
#define sq(X) ((X)*(X))
AlgExpr distance(const AlgVector & point, const Plane & plane)
{
    return abs(plane.a() * point.x() + plane.b() * point.y() + plane.c() * point.z() + plane.d()) / sqrt(sq(plane.a()) + sq(plane.b()) + sq(plane.c()));
}
AlgVector getPointOfNormalToPlane(const AlgVector& base_point, const Plane & plane)
{
    AlgExpr t = -(plane.a() * base_point.x() + plane.b() * base_point.y() + plane.c() * base_point.z() + plane.d()) / (sq(plane.a()) + sq(plane.b()) + sq(plane.c()));
    return base_point + plane.normal() * t;
}
Plane getPlaneThroughPointPerpendicularToVector(const AlgVector & plain_point, const AlgVector & normal)
{
    Plane plain;
    plain.setA(normal.x());
    plain.setB(normal.y());
    plain.setC(normal.z());

    plain.setD(-plain.a() * plain_point.x() - plain.b() * plain_point.y() - plain.c() * plain_point.z());
    return plain;
}
AlgExpr distance(const AlgVector & vec, const Line3d & line)
{

    AlgVector v = line.getPointOnLine() - vec;
    return len(v * line.getBaseVector()) / len(line.getBaseVector());
}

bool isLineParallelToThePlane(const Line3d& line, const Plane& plane)
{
    return scalar(line.getBaseVector(), plane.normal()) == 0;
}

bool arePointsInSameHalfSpace(const AlgVector& a, const AlgVector& b, const Plane& plane)
{
    AlgExpr a_res = plane.a() * a.x() + plane.b() * a.y() + plane.c() * a.z() + plane.d();
    AlgExpr b_res = plane.a() * b.x() + plane.b() * b.y() + plane.c() * b.z() + plane.d();
    return a_res * b_res >= 0;
}

bool isPointInsideTriangle(const AlgVector& p, const AlgVector& a, const AlgVector& b, const AlgVector& c, const Plane& its_plane)
{
    AlgVector v1 = p + (b - a) * (p - a);
    AlgVector v2 = p + (c - b) * (p - b);
    AlgVector v3 = p + (a - c) * (p - c);

    return arePointsInSameHalfSpace(v1, v2, its_plane) && arePointsInSameHalfSpace(v1, v3, its_plane) && arePointsInSameHalfSpace(v2, v3, its_plane);
}

AlgVector getIntersection(const Line3d& line, const Plane& plane)
{
    if (isLineParallelToThePlane(line, plane))
        throw QIODevice::tr("Нельзя найти пересечение параллельных линии и плоскостей");
    AlgVector base = line.getBaseVector();
    AlgVector point = line.getPointOnLine();
    AlgExpr t = -(plane.a() * point.x() + plane.b() * point.y() + plane.c() * point.z() + plane.d()) / (plane.a() * base.x() + plane.b() * base.y() + plane.c() * base.z());
    return point + base * t;
}

AlgVector middle(const AlgVector &a, const AlgVector &b)
{
    assert(a.size() == b.size());
    AlgVector mid = AlgVector::create(a.size());
    for (int i = 0; i < a.size(); ++i)
        mid[i] = (a[i] + b[i]) / 2;
    return mid;
}

AlgVector ratio(const AlgVector &a, const AlgVector &b, AlgExpr m, AlgExpr n)
{
    return  a + (b - a) * (AlgExpr(m)/(m + n));
}

AlgExpr area(const std::vector<AlgVector> &polygon)
{
    AlgExpr res = 0;
    for (int i = 1; i <= polygon.size() - 2; ++i)
    {
        res += area(polygon[0], polygon[i], polygon[i + 1]);
    }
    return res;
}

AlgExpr area(const AlgVector &a, const AlgVector &b, const AlgVector &c)
{
   /* AlgExpr det1 = det(Matrix<AlgExpr>({Vector<AlgExpr>(1, a.x(), a.y()),
                                   Vector<AlgExpr> (1, b.x(), b.y()),
                                   Vector<AlgExpr> (1, c.x(), c.y())}));
    AlgExpr det2 = det(Matrix<AlgExpr>({Vector<AlgExpr>(1, a.x(), a.z()),
                                   Vector<AlgExpr> (1, b.x(), b.z()),
                                   Vector<AlgExpr> (1, c.x(), c.z())}));
    AlgExpr det3 = det(Matrix<AlgExpr>({Vector<AlgExpr>(1, a.y(), a.z()),
                                   Vector<AlgExpr> (1, b.y(), b.z()),
                                   Vector<AlgExpr> (1, c.y(), c.z())}));
    return (sqrt(sqr(det1) + sqr(det2) + sqr(det3))) / 2;*/
    if (a.size() == 3)
        return len((b - a) * (c - a))/2;
    if (a.size() == 2)
        return abs((b.x() - a.x()) * (c.y() - a.y()) - (c.x() - a.x()) * (b.y() - a.y())) / 2;
    throw QIODevice::tr("Вектора должны 2-х или 3-х мерные");
}

AlgExpr angle(const Plane &a, const Plane &b)
{
    return acos(abs(scalar(a.normal(), b.normal()) / (len(a.normal()) * len(b.normal()))));
}

std::pair<std::vector<AlgVector>, std::vector<AlgVector> > translateTo2Dcoordinates(const std::vector<AlgVector> &vertexes)
{
    assert(vertexes.size() >= 3);
    return translateTo2Dcoordinates(vertexes, getPlaneThroughThreePoints(vertexes[0], vertexes[1], vertexes[2]));

}

std::pair<std::vector<AlgVector>, std::vector<AlgVector> > translateTo2Dcoordinates(const std::vector<AlgVector> &vertexes, const Plane &plane)
{
    AlgVector x = (vertexes[1] - vertexes[0])/distance(vertexes[1], vertexes[0]);
    // если (x2, y2, z2) - координаты орты y, то{
    //A*x2 + B*y2 + C*z2 = -D;
    //x1 * x2 + y1 * y2 + z1 * z2 = 0  }
    auto y_coords = gauss( Matrix<AlgExpr>({ {plane.a(), plane.b(), plane.c(),  0},
                             {x.x(), x.y(), x.z(), 0}}));
    AlgVector y(y_coords[0][3], y_coords[1][3], y_coords[2][3]);
  //  qDebug() << "X: " << x.x().toString() << " " << x.y().toString() << " " << x.z().toString();
  //  qDebug() << "Y: " << y.x().toString() << " " << y.y().toString() << " " << y.z().toString();
  //  qDebug() << y_coords[0][0].toString() << " " << y_coords[0][1].toString()  << " " << y_coords[0][2].toString()  << " " << y_coords[0][3].toString();
 //   qDebug() <<        y_coords[1][0].toString()  << " " << y_coords[1][1].toString()  << " " << y_coords[1][2].toString()  << " " << y_coords[1][3].toString();
  //  qDebug() <<        y_coords[2][0].toString()  << " " << y_coords[2][1].toString()  << " " << y_coords[2][2].toString()  << " " << y_coords[2][3].toString();
    if (y.isZero())
    {
        y = AlgVector(y_coords[0][0] + y_coords[0][1] + y_coords[0][2] + y_coords[0][3],
                y_coords[1][0] + y_coords[1][1] + y_coords[1][2] + y_coords[1][3],
                y_coords[2][0] + y_coords[2][1] + y_coords[2][2] + y_coords[2][3]);
    }
  //  qDebug() << "Y: " << y.x().toString() << " " << y.y().toString() << " " << y.z().toString();
    y /= len(y);

    Line3d x_axis(vertexes[0], vertexes[0] + x);
    Line3d y_axis(vertexes[0], vertexes[0] + y);

   // qDebug() << "VERTEXES: ";
   // for (auto &it : vertexes)
   //     qDebug() <<"("<< it.x().toString() << ";  " << it.y().toString() << ";  " << it.z().toString() << ")";

    std::vector<AlgVector> res(vertexes.size());
    for (int i = 0; i < vertexes.size(); ++i)
    {
        //res[i] = AlgVector(distance(vertexes[i], y_axis), distance(vertexes[i], x_axis));
        AlgVector x_proj = projectionToLine(vertexes[i], x_axis) - vertexes[0];
        AlgVector y_proj = projectionToLine(vertexes[i], y_axis) - vertexes[0];
        res[i] = AlgVector(divCollinear(x_proj, x), divCollinear(y_proj, y));
    }

   // qDebug() <<"PROJS:";
    //           for (auto &it : res)
    //               qDebug() <<"("<< it.x().toString() << ";  " << it.y().toString() << ")";

    return {res, {x, y}};
}

AlgVector projectionToLine(const AlgVector &point, const Line3d &line)
{
    AlgVector vec = (point - line.getPointOnLine());
    AlgExpr ang_cos = scalar(vec, line.getBaseVector()) / (len(vec) * len(line.getBaseVector()));
    return line.getPointOnLine() + line.getBaseVector() * ang_cos;
}


std::vector<int> order2DPolygonVertexesAndGetOrder(std::vector<AlgVector> vertexes)
{
    AlgVector center = AlgVector(0, 0);
    for (auto &it : vertexes)
        center += it;
    center /= vertexes.size();

    for (auto &it : vertexes)
        it -= center;
    //делаем в дабле потому что вряд ли точки будут настолько близко
    std::vector<std::pair<int, double>> ang(vertexes.size());
    for (int i = 0; i < ang.size(); ++i)
    {
        ang[i].first = i;
        ang[i].second = atan2(vertexes[i].x().getExpr()->getApproximateValue(), vertexes[i].y().getExpr()->getApproximateValue());
    }
    std::sort(ang.begin(), ang.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b)->bool {
        return a.second < b.second;
    });

    std::vector<int> res(vertexes.size());
    for (int i = 0; i < res.size(); ++i)
    {
        res[i] = ang[i].first;
    }
    return res;

}
std::vector<AlgVector> order2DPolygonVertexes(const std::vector<AlgVector>& vertexes)
{
    auto order = order2DPolygonVertexesAndGetOrder(vertexes);
    std::vector<AlgVector> res(vertexes.size());
    for (int i = 0; i < vertexes.size(); ++i)
        res[i] = vertexes[order[i]];
    return res;

}

std::vector<AlgVector> order3DPolygonVertexes(const std::vector<AlgVector> &vertexes)
{
    auto vert_2d = translateTo2Dcoordinates(vertexes);
    auto ordered = order2DPolygonVertexesAndGetOrder(vert_2d.first);
    std::vector<AlgVector> res(vertexes.size());
    for (int i = 0; i < vertexes.size(); ++i)
    {
        res[i] = vertexes[ordered[i]];
    }
    return res;
}

Plane getPlaneThroughTwoPointsAndParallelToVector(const AlgVector &point1, const AlgVector &point2, const AlgVector &parallel_vector)
{
    return getPlaneThroughThreePoints(point1, point2, point1 + parallel_vector);
}

Plane getPlaneThroughTwoPointsAndParallelToLine(const AlgVector &point1, const AlgVector &point2, const Line3d &line)
{
    return getPlaneThroughThreePoints(point1, point2, point1 + line.getBaseVector());
}

Plane getPlaneThroughTwoPointsAndPerpendicularToPlane(const AlgVector &point1, const AlgVector &point2, const Plane &perp_plane)
{
    return getPlaneThroughTwoPointsAndParallelToVector(point1, point2, perp_plane.normal());
}

AlgExpr angle(const Line3d &a, const Plane &b)
{
    return pi()/2 - angle(a, b.normal());
}

AlgExpr angle(const AlgVector &a, const AlgVector &b)
{
    return acos(scalar(a, b) / (a.size() * b.size()));
}

AlgExpr angle(const Line3d &a, const Line3d &b)
{
    return acos(abs(scalar(a.getBaseVector(), b.getBaseVector()) / (a.getBaseVector().size() * b.getBaseVector().size())));
}

AlgExpr angle(const Line3d &a, const AlgVector &b)
{
    return angle(a, Line3d(b, b*2));
}

AlgExpr angle(const AlgVector &a, const Line3d &b)
{
    return angle(b, a);
}

AlgExpr distance(const Line3d &a, const Line3d &b)
{
    if (areParallel(a, b))
    {
        AlgVector h = b.getPointOnLine() - a.getPointOnLine();
        return abs(scalar(h, a.getBaseVector()*b.getBaseVector())/len(a.getBaseVector())/len(b.getBaseVector()));
    }
    return distance(a.getPointOnLine(), getPlaneThroughLineParallelToLine(b, a, false));
}

bool areParallel(const Line3d &a, const Line3d &b)
{
    return areColliniar(a.getBaseVector(), b.getBaseVector());
}

Plane getPlaneThroughLineParallelToLine(const Line3d & line_in_plane, const Line3d &parallel_line, bool check_parall)
{
    if (check_parall)
        if (areParallel(line_in_plane, parallel_line))
            throw QIODevice::tr("Нельзя выбрать плоскость проходящую через прямую, параллельную другой прямой, если эти прямые параллельны");
    return getPlaneThroughTwoPointsAndParallelToLine(line_in_plane.getPointOnLine(), line_in_plane.getPointOnLine() + line_in_plane.getBaseVector(),
                                                     parallel_line);
}

AlgExpr distance(const Line3d &a, const Plane &p)
{
    if (isLineParallelToThePlane(a, p))
        return distance(a.getPointOnLine(), p);
    throw QIODevice::tr("Нельзя посчитать расстояние между плоскостью и линией, пересекающейся с ней");
}

AlgExpr distance(const Plane &a, const Plane &b)
{
    if (areParallel(a, b))
    {
        if (b.a() != 0)
            return abs (a.d() - b.d() * a.a() / b.a()) / sqrt(sqr(a.a()) + sqr(a.b()) + sqr(a.c()));
        if (b.b() != 0)
            return abs (a.d() - b.d() * a.b() / b.b()) / sqrt(sqr(a.a()) + sqr(a.b()) + sqr(a.c()));
        return abs (a.d() - b.d() * a.c() / b.c()) / sqrt(sqr(a.a()) + sqr(a.b()) + sqr(a.c()));
    }
    throw QIODevice::tr("Нельз посчитать расстояние между пересекающимися плоскостями");
}

bool areParallel(const Plane &a, const Plane &b)
{
    return areColliniar(a.normal(), b.normal());
}

AlgExpr angle(const AlgVector &a, const Plane &b)
{
    return abs(pi() / 2 - angle(a, b.normal()));
}
bool areMathes(const Line3d & a, const Line3d & b)
{
    if (!areColliniar(a.getBaseVector(), b.getBaseVector()))
        return false;
    return a.getPointOnLine() == b.getPointOnLine() || areColliniar(b.getPointOnLine() - a.getPointOnLine(), a.getBaseVector());
}
AlgVector getIntersection(const Line3d &a, const Line3d &b)
{
    if (areMathes(a, b))
        throw QIODevice::tr("Прямые совпадают");
    if (areParallel(a, b))
        throw QIODevice::tr("Прямые параллельны");
    //L1 = p1 + t1 * b1
    //L2 = p2 + t2 * b2
    //p1 + t1 * b1 == p2 + t2 * b2

    //x1 + t1 * b1.x == x2 + t2 * b2.x
    //y1 + t1 * b1.y == y2 + t2 * b2.y
    //z1 + t1 * b1.z == z2 + t2 * b2.z

    // t1 * b1.x - t2 * b2.x == x2 - x1
    // t1 * b1.y - t2 * b2.y == y2 - y1
    auto M = Matrix<AlgExpr>(std::vector<Vector<AlgExpr>> ({ {a.getBaseVector().x(), -1*b.getBaseVector().x(), 0, b.getPointOnLine().x() - a.getPointOnLine().x()},
                                                             {a.getBaseVector().y(), -1 * b.getBaseVector().y(), 0, b.getPointOnLine().y() - b.getPointOnLine().y()},
                                                                                                    {a.getBaseVector().z(), -1 * b.getBaseVector().z(), 0, b.getPointOnLine().z() - a.getPointOnLine().z()}}));

    auto eqres = gauss(Matrix<AlgExpr>(std::vector<Vector<AlgExpr>> ({ {a.getBaseVector().x(), -1*b.getBaseVector().x(), 0, b.getPointOnLine().x() - a.getPointOnLine().x()},
                                {a.getBaseVector().y(), -1 * b.getBaseVector().y(), 0, b.getPointOnLine().y() - a.getPointOnLine().y()},
                                                                       {a.getBaseVector().z(), -1 * b.getBaseVector().z(), 0, b.getPointOnLine().z() - a.getPointOnLine().z()}})));

  //  for (int i = 0; i < 3; ++i)
    //{
      //  auto d = qDebug() ;
        //for (int j = 0; j < 4; ++j)
          //  d << M[i][j].toString() << " | ";
   // }




    if (eqres.size() == 0)
        throw QIODevice::tr("Прямые скрещиваются");
    AlgExpr t1 = eqres[0].back();
    AlgExpr t2 = eqres[1].back();

    if (a.getPointOnLine() + t1 * a.getBaseVector() == b.getPointOnLine() + t2 * b.getBaseVector())
        return a.getPointOnLine() + t1 * a.getBaseVector();
    throw QIODevice::tr("Прямые скрещиваются");
}

AlgVector geoCenter(const Polyhedron &pol)
{
    auto vert = pol.getBasePoints();
    AlgVector sum = Vector<AlgExpr>(0,0 ,0);
    for (auto &it: vert)
        sum += *it.second;
    sum /= vert.size();
    return sum;

}

AlgExpr pyramidVolume(const std::vector<AlgVector> &base, const AlgVector &top_vertex)
{
    auto h = distance(top_vertex, getPlaneThroughThreePoints(base[0], base[1], base[2]));
    return area(base) * h / 3;
}
