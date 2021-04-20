#include "geometry_3d.h"

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
    AlgVector base = line.getBaseVector();
    AlgVector point = line.getPointOnLine();
    AlgExpr t = -(plane.a() * point.x() + plane.b() * point.y() + plane.c() * point.z() + plane.d()) / (plane.a() * base.x() + plane.b() * base.y() + plane.c() * base.z());
    return point + base * t;
}
