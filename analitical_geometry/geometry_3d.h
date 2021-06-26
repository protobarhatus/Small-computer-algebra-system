#ifndef GEOMETRY_3D_H
#define GEOMETRY_3D_H

#include "line3d.h"
#include "plane.h"
#include "polyhedron.h"

AlgVector getPointOfNormalToPlane(const AlgVector& base_point, const Plane & plane);
AlgExpr distance(const AlgVector & point, const Plane & plane);

Plane getPlaneThroughPointPerpendicularToVector(const AlgVector & plain_point, const AlgVector & normal);
Plane getPlaneThroughTwoPointsAndParallelToVector(const AlgVector & point1, const AlgVector & point2, const AlgVector & parallel_vector);
Plane getPlaneThroughTwoPointsAndParallelToLine(const AlgVector & point1, const AlgVector & point2, const Line3d & line);
Plane getPlaneThroughTwoPointsAndPerpendicularToPlane(const AlgVector & point1, const AlgVector & point2, const Plane & perp_plane);

AlgExpr distance(const AlgVector & vec, const Line3d & line);
AlgExpr distance(const Line3d & a, const Line3d & b);
AlgVector projectionToLine(const AlgVector & point, const Line3d & line);

bool isLineParallelToThePlane(const Line3d& line, const Plane& plane);

bool arePointsInSameHalfSpace(const AlgVector& a, const AlgVector& b, const Plane& plane);

bool isPointInsideTriangle(const AlgVector& point, const AlgVector& a, const AlgVector& b, const AlgVector& c, const Plane& its_plance);
//линия не параллельна плоскости
AlgVector getIntersection(const Line3d& line, const Plane& plane);

AlgVector middle(const AlgVector & a, const AlgVector & b);
AlgVector ratio(const AlgVector & a, const AlgVector & b, int m, int n);

//вершины должны быть в ПРАВИЛЬНОМ порядке (в порядке выпуклого многоугольника)
AlgExpr surface(const std::vector<AlgVector> & polygon);
AlgExpr surface(const AlgVector &a, const AlgVector &b, const AlgVector &c);

AlgExpr angle(const Plane & a, const Plane & b);
AlgExpr angle(const Line3d & a, const Plane & b);
AlgExpr angle(const AlgVector & a, const AlgVector &b);
AlgExpr angle(const Line3d & a, const Line3d & b);
AlgExpr angle(const Line3d & a, const AlgVector & b);
AlgExpr angle(const AlgVector & a, const Line3d & b);
//на вход подается набор точек, лежащий в одной плоскости в 3д координатах. (это должен гарантировать вызывающий ф-ю)
//функция вернет этот набор в неких 2д координатах, лежащих в этой плоскости, а также орты (второй вектор) этих координат, выраженные в 3д пространстве
//первая вершина будет взята за ноль, вторая - за ось Ox. Расстояния не изменяются
//если не передать plane, то она сама построится
std::pair<std::vector<AlgVector>, std::vector<AlgVector>> translateTo2Dcoordinates(const std::vector<AlgVector> & vertexes);
std::pair<std::vector<AlgVector>, std::vector<AlgVector>> translateTo2Dcoordinates(const std::vector<AlgVector> & vertexes, const Plane & plane);

std::vector<AlgVector> order2DPolygonVertexes(const std::vector<AlgVector>& vertexes);
std::vector<AlgVector> order3DPolygonVertexes(const std::vector<AlgVector> & vertexes);
#endif // GEOMETRY_3D_H
