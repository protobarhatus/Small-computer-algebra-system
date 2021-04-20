#ifndef PLANE_H
#define PLANE_H
#include "algebra/Vector.h"
#include "line3d.h"

class Plane
{
public:
    Plane();
    Plane(const AlgExpr& a, const AlgExpr& b, const AlgExpr& c, const AlgExpr& d);
    ~Plane();
    AlgVector normal() const;
    AlgExpr a() const;
    AlgExpr b() const;
    AlgExpr c() const;
    AlgExpr d() const;
    void setA(const AlgExpr& a);
    void setB(const AlgExpr& b);
    void setC(const AlgExpr& c);
    void setD(const AlgExpr& d);
    //возвращает аппликату точки, принадлежащей плоскости, по ее абсциссе и ординате
    AlgExpr z(const AlgExpr& x, const AlgExpr& y);
    //возвращает ординату точки, принадлежащей плоскости, по ее абсциссе и аппликате
    AlgExpr y(const AlgExpr & x, const AlgExpr& z);
    //возвращает абсциссу точки, принадлежащей плоскости, по ее ординате и аппликате
    AlgExpr x(const AlgExpr& y, const AlgExpr& z);


    //рассчитывает орты, которые образуют декартову систему координат в данной плоскости и представляет их в виде векторов трехмерного пространства
    //void makeOwnUnitsVectors();

    Line3d getIntersection(const Plane & plane);
    //Vector2 getCoordinatesInPlanesUnitsVector(const Vector3d & point) const;
private:
    AlgExpr _a, _b, _c, _d;


    //bool has_counted_units_vectors = false;
    //Vector3d abscissa_unit_vector, ordinata_unit_vector;
    //Line3d abscissa_axis, ordinata_axis;
};

Plane getPlaneThroughPointAndTwoVectors(const AlgVector& point, const AlgVector& vector1, const AlgVector& vector2);

#endif // PLANE_H
