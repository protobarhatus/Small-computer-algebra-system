#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "line3d.h"
#include "plane.h"

#include <map>
#include <QString>
class Polyhedron
{
    std::map<QString, AlgVector> points;
    std::map<QString, AlgVector*> base_points;
public:
    Polyhedron();

    AlgVector point(const QString & name);
    AlgVector operator[](const QString & name);
    Line3d line(const QString & name);
    Plane plane(const QString & name);

    void addPoint(const AlgVector & point, const QString & name);
    void addPoint(AlgVector && point, const QString & name);

    void connectPoint(const AlgVector & point, const QString & name);
    void connectPoint(AlgVector && point, const QString & name);

    std::vector<AlgVector> section(const Plane & plane);

};
//возвращает вектор 2 размерности, последняя точка - центр
std::vector<AlgVector> getBaseRightPolygonWithCenter(const AlgExpr &base_edge, int n);
std::vector<AlgVector> getBaseRightPolygon(const AlgExpr &base_edge, int n);
std::vector<AlgVector> getBaseRectangular(const AlgExpr & a, const AlgExpr & b);

//name - точки, составляющие пирамиду, первая точка берется как вершина, X ось координат берется вдоль линии второй и третьей точки, пример задания - TABCD
Polyhedron makeRightPiramid(const AlgExpr & base_edge, const AlgExpr & height, const QString & name);
Polyhedron makeRightEquilateralPiramid(const AlgExpr & base_edge, const QString & name);
Polyhedron makeRightPiramidWithGivenBaseEdgeAndSideEdge(const AlgExpr & base_edge, const AlgExpr & side_edge, const QString & name);
Polyhedron makeTetrahedron(const AlgExpr & edge, const QString & name);
Polyhedron makePiramidOverRectangleWithHeightInCenter(const AlgExpr & a, const AlgExpr & b, const AlgExpr & height, const QString & name);
Polyhedron makePiramidOverPolygonWithHeightInCenter(const std::vector<AlgVector> & base, const AlgVector & center, const AlgExpr & height, const QString & name);
Polyhedron makePiramidOverPolygonWithHeightInVertex(const std::vector<AlgVector> & base, const AlgExpr & height, const QString & vertex_name, const QString & name);


Polyhedron makeRightPrizm(const AlgExpr & edge, const AlgExpr & height, const QString & name);
Polyhedron makeRectangularParallelepiped(const AlgExpr & a, const AlgExpr & b, const AlgExpr & c, const QString & name);
Polyhedron makePrizmOverPolygon(const std::vector<AlgVector> & base, const AlgExpr & height, const QString & name);
#endif // POLYHEDRON_H
