#include "polyhedron.h"
#include "geometry_3d.h"
Polyhedron::Polyhedron()
{

}

AlgVector Polyhedron::point(const QString &name) const
{
    if (this->points.find(name) == this->points.end())
        throw (QString)"Has not point \"" + name + "\"";
    return this->points.find(name)->second;
}

AlgVector Polyhedron::operator[](const QString &name) const
{
    return point(name);
}
std::vector<QString> splitPointsNames(const QString & points)
{
    std::vector<QString> res;
    int ind = 0;
    do
    {
        QString name = points[ind];
        ++ind;
        while (ind < points.size() && !(points[ind] >= 'A' && points[ind] <= 'Z'))
        {
            name += points[ind];
            ++ind;
        }
        res.push_back(name);
    } while (ind < points.size());
    return res;
}
bool isAppropriatePointName(const QString &str)
{
    if (! (str[0] >= 'A' && str[0] <= 'Z'))
            return false;
    for (int i = 1; i < str.length(); ++i)
        if (! ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9')))
            return false;
    return true;
}
Line3d Polyhedron::line(const QString &name) const
{
    auto names = splitPointsNames(name);
    if (names.size() != 2)
        throw (QString)"Line should be called after 2 points";
    return Line3d(this->point(names[0]), this->point(names[1]));
}

Plane Polyhedron::plane(const QString &name) const
{
    auto names = splitPointsNames(name);
    if (names.size() != 3)
        throw (QString)"Plane should be called after 3 points";
    return getPlaneThroughThreePoints(this->point(names[0]), this->point(names[1]), this->point(names[2]));
}

void Polyhedron::addVertex(const AlgVector &point, const QString &name)
{
    auto it = this->points.insert({name, point});
    this->base_points.insert({name, &it.first->second});
}

void Polyhedron::addVertex(AlgVector &&point, const QString &name)
{
    auto it = this->points.insert({name, std::move(point)});
    this->base_points.insert({name, &it.first->second});
}

void Polyhedron::addPoint(const AlgVector &point, const QString &name)
{
    this->points.insert({name, point});
}

void Polyhedron::addPoint(AlgVector &&point, const QString &name)
{
    this->points.insert({name, std::move(point)});
}

void Polyhedron::addConnection(const QString &a, const QString &b)
{
    this->dots_connections[a].insert(b);
    this->dots_connections[b].insert(a);
}

bool Polyhedron::hasConnection(const QString &a, const QString &b) const
{
    auto it = this->dots_connections.find(a);
    return it != this->dots_connections.end() && it->second.find(b) != it->second.end();
}

std::vector<AlgVector> Polyhedron::section(const Plane &plane)
{
    std::vector<AlgVector> res;
    for (auto it = this->base_points.begin(); it != this->base_points.end(); ++it)
    {
        for (auto it1 = std::next(it); it1 != this->base_points.end(); ++it1)
        {
            if (!hasConnection(it->first, it1->first))
                continue;
            Line3d edge = this->line(it->first + it1->first);
            if (isLineParallelToThePlane(edge, plane))
                continue;
            AlgVector point = getIntersection(edge, plane);
           // qDebug() << point.x().toString() << " " << point.y().toString() << " " << point.z().toString();
            AlgExpr dist = distance(*it->second, *it1->second);
           // qDebug() << dist.toString();
           // qDebug() << distance(point, *it->second).toString();
          //  qDebug() << distance(point, *it1->second).toString();
            if (distance(point, *it->second) <= dist && distance(point, *it1->second) <= dist)
            {
                bool has_it = false;
                for (auto &it : res)
                    if ((point - it).isZero())
                        has_it = true;
                if (!has_it)
                    res.push_back(point);
            }
        }
    }

    return  order3DPolygonVertexes(res);
}

const std::map<QString, AlgVector *> &Polyhedron::getBasePoints() const
{
    return this->base_points;
}
AlgExpr getOutcribedRadOfRightPolygon(const AlgExpr & edge, int n)
{
    return edge / (2 * sin(degToRad(AlgExpr(180) / n)));
}
std::pair<std::vector<AlgVector>, AlgVector> getBaseRightPolygonAndCenter(const AlgExpr &base_edge, int n)
{
    std::vector<AlgVector> base(n);
    AlgExpr outscribed_rad = getOutcribedRadOfRightPolygon(base_edge, n);
    AlgExpr dist_from_edge_to_center = sqrt(sqr(outscribed_rad) - sqr(base_edge / 2));

    Vector<AlgExpr> vertex_vector ( - base_edge / 2, - dist_from_edge_to_center);
    Matrix<AlgExpr> rot_matrix = rotationMatrix2D<AlgExpr>(degToRad(AlgExpr(360) / n));

 /*   for (int i = 0; i < rot_matrix.lines(); ++i)
        for (int j = 0; j < rot_matrix.columns(); ++j)
            qDebug() << rot_matrix[i][j].toString() << "  ;  ";*/

    for (int i = 0; i < n; ++i)
    {
        //qDebug() << "STEP: " << i;
        base[i] = AlgVector(base_edge / 2 + vertex_vector.x(), dist_from_edge_to_center + vertex_vector.y());
        //qDebug() << " ( " << vertex_vector.x().toString() << " ; " << vertex_vector.y().toString() << " )";
       // vertex_vector.x().getExpr()->setSimplified(false);
       // vertex_vector.y().getExpr()->setSimplified(false);
       // vertex_vector.x().getExpr()->simplify();
       // vertex_vector.y().getExpr()->simplify();
       // qDebug() << " ( " << vertex_vector.x().toString() << " ; " << vertex_vector.y().toString() << " )";
        vertex_vector = rot_matrix * vertex_vector;
    }

    return {base, AlgVector(base_edge / 2, dist_from_edge_to_center)};
}
std::vector<AlgVector> getBaseRightPolygonWithCenter(const AlgExpr &base_edge, int n)
{
    auto polygon = getBaseRightPolygonAndCenter(base_edge, n);
    polygon.first.push_back(std::move(polygon.second));
    return polygon.first;
}
std::vector<AlgVector> getBaseRightPolygon(const AlgExpr &base_edge, int n)
{

    return getBaseRightPolygonAndCenter(base_edge, n).first;
}
Polyhedron makeRightPiramid(const AlgExpr &base_edge, const AlgExpr & height, const QString &name)
{
    auto points = splitPointsNames(name);
    auto base = getBaseRightPolygonAndCenter(base_edge, points.size() - 1);
    return makePiramidOverPolygonWithHeightInCenter(base.first, base.second, height, name);


   /* auto base = getBaseRightPolygonWithCenter(base_edge, points.size() - 1);

    Polyhedron piramid;
    for (int i = 0; i < points.size() - 1; ++i)
    {
        piramid.addPoint(addDimension<AlgExpr>(std::move(base[i]), 0), points[i + 1]);
    }
    piramid.addPoint(AlgVector(std::move(base.back().x()), std::move(base.back().y()), height), points[0]);
    return piramid;*/
}
Polyhedron makeRightPiramidWithGivenBaseEdgeAndSideEdge(const AlgExpr &base_edge, const AlgExpr &side_edge, const QString &name)
{
    auto points = splitPointsNames(name);
    return makeRightPiramid(base_edge, sqrt(sqr(side_edge) - sqr(getOutcribedRadOfRightPolygon(base_edge, points.size() - 1))), name);
}

Polyhedron makeRightEquilateralPiramid(const AlgExpr &base_edge, const QString &name)
{
    auto points = splitPointsNames(name);
    return makeRightPiramid(base_edge, sqrt(sqr(base_edge) - sqr(getOutcribedRadOfRightPolygon(base_edge, points.size() - 1))), name);

}

Polyhedron makeTetrahedron(const AlgExpr &edge, const QString & name)
{
    return makeRightEquilateralPiramid(edge, name);
}

Polyhedron makeRightPrizm(const AlgExpr &edge, const AlgExpr &height, const QString &name)
{
    auto points = splitPointsNames(name);
    assert(points.size() % 2 == 0);
    auto base = getBaseRightPolygon(edge, points.size() / 2);
    return makePrizmOverPolygon(base, height, name);

   /* auto base = getBaseRightPolygonWithCenter(edge, points.size() / 2);
    Polyhedron prizm;
    for (int i = 0; i < points.size() / 2; ++i)
    {
        prizm.addPoint(addDimension<AlgExpr>(base[i], 0), points[i]);
        prizm.addPoint(addDimension<AlgExpr>(std::move(base[i]), height), points[i + points.size() / 2]);
    }
    return prizm;*/
}

Polyhedron makeRectangularParallelepiped(const AlgExpr &a, const AlgExpr &b, const AlgExpr &c, const QString &name)
{
    return makePrizmOverPolygon(getBaseRectangular(a, b), c, name);


   /* auto points = splitPointsNames(name);
    assert(points.size() == 8);
    auto base = getBaseRectangular(a, b);
    Polyhedron par;
    for (int i = 0; i < points.size() / 2; ++i)
    {
        par.addPoint(addDimension<AlgExpr>(base[i], 0), points[i]);
        par.addPoint(addDimension<AlgExpr>(std::move(base[i]), c), points[i + points.size() / 2]);
    }
    return par;*/
}

std::vector<AlgVector> getBaseRectangular(const AlgExpr &a, const AlgExpr &b)
{
    std::vector<AlgVector> base(4);
    base[0] = AlgVector(AlgExpr(0), 0);
    base[1] = AlgVector(a, 0);
    base[2] = AlgVector(a, b);
    base[3] = AlgVector(0, b);
    return base;
}
std::vector<AlgVector> getRightTriangle(const AlgExpr &a, const AlgExpr &b)
{
    std::vector<AlgVector> base(3);
    base[0] = AlgVector(AlgExpr(0), 0);
    base[1] = AlgVector(a, 0);
    base[2] = AlgVector(0, b);
    return base;
}

Polyhedron makePiramidOverRectangleWithHeightInCenter(const AlgExpr &a, const AlgExpr &b, const AlgExpr &height, const QString &name)
{
    //СЧИТАЕМ ЧТО ОДНА ИЗ ВЕРШИН В НУЛЕ А ОСИ ПО СТОРОНАМ
    return makePiramidOverPolygonWithHeightInCenter(getBaseRectangular(a, b), (a + b) / 2, height, name);


    /*auto points = splitPointsNames(name);
    assert(points.size() == 5);
    auto base = getBaseRectangular(a, b);

    Polyhedron piram;
    for (int i = 0; i < base.size(); ++i)
    {
        piram.addPoint(addDimension<AlgExpr>(std::move(base[i]), 0), points[i + 1]);
    }
    piram.addPoint(AlgVector(a / 2, b / 2, height), points[0]);
    return piram;*/
}

Polyhedron makePiramidOverPolygonWithHeightInCenter(const std::vector<AlgVector> &base, const AlgVector &center, const AlgExpr &height, const QString &name)
{
    return makePiramidOverPolygon(base, center, height, name);


    /*auto points = splitPointsNames(name);
    assert(points.size() == base.size() + 1);
    Polyhedron piram;
    for (int i = 0; i < base.size(); ++i)
    {
        piram.addPoint(addDimension<AlgExpr>(std::move(base[i]), 0), points[i + 1]);
    }
    piram.addPoint(addDimension(center, height), points[0]);
    return piram;*/
}



Polyhedron makePiramidOverPolygonWithHeightInVertex(const std::vector<AlgVector> &base, const AlgExpr &height, const QString &vertex_name, const QString &name)
{

    auto points = splitPointsNames(name);
    assert(points.size() == base.size() + 1);
    int height_index = -1;
    for (int i = 0; i < base.size(); ++i)
    {
        if (points[i + 1] == vertex_name)
            height_index = i;
    }
    if (height_index == -1)
        throw (QString)"Height's vertex name are not presented in piramid";

    return makePiramidOverPolygon(base, base[height_index], height, name);


   /* auto points = splitPointsNames(name);
    assert(points.size() == base.size() + 1);
    Polyhedron piram;
    int height_index = -1;
    for (int i = 0; i < base.size(); ++i)
    {
        if (points[i + 1] == vertex_name)
            height_index = i;
        piram.addPoint(addDimension<AlgExpr>(std::move(base[i]), 0), points[i + 1]);
    }
    if (height_index == -1)
        throw (QString)"Height's vertex name are not presented in piramid";
    piram.addPoint(addDimension<AlgExpr>(base[height_index], height), points[0]);
    return piram;*/
}

Polyhedron makePrizmOverPolygon(const std::vector<AlgVector> &base, const AlgExpr &height, const QString &name)
{
    auto points = splitPointsNames(name);
    assert(points.size()  == base.size() * 2);

    Polyhedron prizm;
    for (int i = 0; i < points.size() / 2; ++i)
    {
        prizm.addVertex(addDimension<AlgExpr>(base[i], 0), points[i]);
        prizm.addVertex(addDimension<AlgExpr>(std::move(base[i]), height), points[i + points.size() / 2]);
    }
    for (int i = 0; i < points.size() / 2; ++i)
    {
        prizm.addConnection(points[i], points[(i + 1) % (points.size() / 2)]);
        prizm.addConnection(points[i + points.size() / 2], points[(i + 1) % (points.size() / 2) + points.size() / 2]);
        prizm.addConnection(points[i], points[i + points.size() / 2]);

    }
    return prizm;
}







Polyhedron makePiramidOverPolygon(const std::vector<AlgVector> &base, const AlgVector &height_base, const AlgExpr &height, const QString &name)
{
    auto points = splitPointsNames(name);
    assert(points.size() == base.size() + 1);
    Polyhedron piram;
    for (int i = 0; i < base.size(); ++i)
    {
        piram.addVertex(addDimension<AlgExpr>(std::move(base[i]), 0), points[i + 1]);
    }
    piram.addVertex(addDimension(height_base, height), points[0]);

    for (int i = 0; i < base.size(); ++i)
    {
        piram.addConnection(points[i + 1], points[(i + 1) % base.size() + 1]);
        piram.addConnection(points[i + 1], points[0]);
    }
    piram.addPoint(addDimension(height_base, AlgExpr(0)), "O");
    return piram;
}




