#ifndef GEOMETRY_3D_H
#define GEOMETRY_3D_H

#include "line3d.h"
#include "plane.h"


AlgVector getPointOfNormalToPlane(const AlgVector& base_point, const Plane & plane);
AlgExpr distance(const AlgVector & point, const Plane & plane);
Plane getPlaneThroughPointPerpendicularToVector(const AlgVector & plain_point, const AlgVector & normal);
AlgExpr distance(const AlgVector & vec, const Line3d & line);

bool isLineParallelToThePlane(const Line3d& line, const Plane& plane);

bool arePointsInSameHalfSpace(const AlgVector& a, const AlgVector& b, const Plane& plane);

bool isPointInsideTriangle(const AlgVector& point, const AlgVector& a, const AlgVector& b, const AlgVector& c, const Plane& its_plance);
//линия не параллельна плоскости
AlgVector getIntersection(const Line3d& line, const Plane& plane);


#endif // GEOMETRY_3D_H
