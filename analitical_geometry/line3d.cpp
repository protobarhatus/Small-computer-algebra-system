#include "line3d.h"



Line3d::Line3d()
{
}


Line3d::~Line3d()
{
}
Line3d::Line3d(const AlgVector& point1, const AlgVector& point2)
{
    this->point_on_line = point1;
    AlgVector kovector = point2 - point1;
    this->base_vector = kovector * (1. / len(kovector));
    if (this->base_vector.x() + this->base_vector.y() + this->base_vector.z() < 0)
        this->base_vector *= -1;
}
AlgVector Line3d::getBaseVector() const
{
    return this->base_vector;
}
AlgVector Line3d::getPointOnLine() const
{
    return this->point_on_line;
}
