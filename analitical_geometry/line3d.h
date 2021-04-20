#pragma once
#include "algebra/Vector.h"
#include "algebra/algexpr.h"
class Line3d
{
public:
    Line3d(const AlgVector& first_point, const AlgVector & second_point);
    ~Line3d();
    Line3d();
    AlgVector getBaseVector() const;
    AlgVector getPointOnLine() const;
private:
    //какая-либо точка
    AlgVector point_on_line;
    //направляющий вектор
    AlgVector base_vector;
};

