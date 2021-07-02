#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "line3d.h"
#include "plane.h"

#include <map>
#include <set>
#include <QString>
class Polyhedron
{
    //здесь также и точки, которые просто привязаны к многограннику (points)
    std::map<QString, AlgVector> points;
    //здесь точки, которые являются вершинами многогранника (vertexes)
    std::map<QString, AlgVector*> base_points;
    //здесь хранятся ребра, к каждой вершине хранятся вершины, с которыми она соединена (понятное дело, они дублируются)
    std::map<QString, std::set<QString>> dots_connections;
public:
    Polyhedron();

    AlgVector point(const QString & name) const;
    AlgVector operator[](const QString & name) const;
    Line3d line(const QString & name) const;
    Plane plane(const QString & name) const;
    //для функций-фабрик
    void addVertex(const AlgVector & point, const QString & name);
    void addVertex(AlgVector && point, const QString & name);

    void addPoint(const AlgVector & point, const QString & name);
    void addPoint(AlgVector && point, const QString & name);

    void addConnection(const QString & a, const QString & b);

    bool hasConnection(const QString & a, const QString & b) const;

    std::vector<AlgVector> section(const Plane & plane);

};
std::vector<QString> splitPointsNames(const QString & points);
//возвращает вектор 2 размерности, последняя точка - центр
std::vector<AlgVector> getBaseRightPolygonWithCenter(const AlgExpr &base_edge, int n);
std::vector<AlgVector> getBaseRightPolygon(const AlgExpr &base_edge, int n);
std::vector<AlgVector> getBaseRectangular(const AlgExpr & a, const AlgExpr & b);
std::vector<AlgVector> getRightTriangle(const AlgExpr & a, const AlgExpr & b);

//name - точки, составляющие пирамиду, первая точка берется как вершина, X ось координат берется вдоль линии второй и третьей точки, пример задания - TABCD: пирамида
//с вершиной T и основанием A, B, C, D. Имена точек не анализируются, важно лишь их количество и то, переданы ли они в пирамиду или призму.
//Имена точек начинаются с заглавной латинской буквы, дальше следуют любые символы, кроме заглавной латинской буквы (она означает начало нового имени)
//пример корректного имени тетраэдра: TAbB12Cdot
Polyhedron makeRightPiramid(const AlgExpr & base_edge, const AlgExpr & height, const QString & name);
Polyhedron makeRightEquilateralPiramid(const AlgExpr & base_edge, const QString & name);
Polyhedron makeRightPiramidWithGivenBaseEdgeAndSideEdge(const AlgExpr & base_edge, const AlgExpr & side_edge, const QString & name);
Polyhedron makeTetrahedron(const AlgExpr & edge, const QString & name);
Polyhedron makePiramidOverRectangleWithHeightInCenter(const AlgExpr & a, const AlgExpr & b, const AlgExpr & height, const QString & name);
Polyhedron makePiramidOverPolygonWithHeightInCenter(const std::vector<AlgVector> & base, const AlgVector & center, const AlgExpr & height, const QString & name);
Polyhedron makePiramidOverPolygonWithHeightInVertex(const std::vector<AlgVector> & base, const AlgExpr & height, const QString & vertex_name, const QString & name);
Polyhedron makePiramidOverPolygon(const std::vector<AlgVector> & base, const AlgVector & height_base, const AlgExpr & height, const QString & name);


Polyhedron makeRightPrizm(const AlgExpr & edge, const AlgExpr & height, const QString & name);
Polyhedron makeRectangularParallelepiped(const AlgExpr & a, const AlgExpr & b, const AlgExpr & c, const QString & name);
Polyhedron makePrizmOverPolygon(const std::vector<AlgVector> & base, const AlgExpr & height, const QString & name);
#endif // POLYHEDRON_H
