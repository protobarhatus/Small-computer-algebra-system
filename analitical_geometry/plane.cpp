#include "plane.h"
//#include "geometry_3d.h"
#include <assert.h>
#include <cmath>
#include <vector>
#include "geometry_3d.h"
Plane::Plane()
{
}


Plane::~Plane()
{
}

Plane::Plane(const AlgExpr& a, const AlgExpr& b, const AlgExpr& c, const AlgExpr& d) : _a(a), _b(b), _c(c), _d(d)
{
    this->getRidOfGcd();
}
AlgVector Plane::normal() const
{
    return AlgVector(_a, _b, _c);
}
AlgExpr Plane::a() const
{
    return this->_a;
}
AlgExpr Plane::b() const
{
    return this->_b;
}
AlgExpr Plane::c() const
{
    return this->_c;
}
AlgExpr Plane::d() const
{
    return this->_d;
}
void Plane::setA(const AlgExpr& a)
{
    this->_a = a;
}
void Plane::setB(const AlgExpr& b)
{
    this->_b = b;
}
void Plane::setC(const AlgExpr& c)
{
    this->_c = c;
}
void Plane::setD(const AlgExpr& d)
{
    this->_d = d;
}
AlgExpr Plane::z(const AlgExpr& x, const AlgExpr& y)
{
    assert(this->_c == 0);
    return (-_d - _a * x - _b * y) / _c;
}
AlgExpr Plane::y(const AlgExpr& x, const AlgExpr& z)
{
    assert(this->_b == 0);
    return (-_d - _a * x - _c * z) / _b;
}
AlgExpr Plane::x(const AlgExpr& y, const AlgExpr& z)
{
    assert(this->_a == 0);
    return (-_d - _b * y - _c * z) / _a;
}
/*void Plane::makeOwnUnitsVectors()
{
    if (has_counted_units_vectors)
        return;
    Vector3d zero = getPointOfNormalToPlane(Vector3d(0, 0, 0), *this);

    Vector3d one = getPointOfNormalToPlane(Vector3d(1, 1, 1), *this);

    if (one == zero)
        one = getPointOfNormalToPlane(Vector3d(0, 0, 1), *this);
    one += (one * ((sqrt(2) - (one - zero).length()) / one.length()));
    Vector3d middle = (zero + one) * 0.5;

    Line3d another_diagonal = this->getIntersection(getPlaneThroughPointPerpendicularToVector(middle, one - zero));
    Vector3d sq_diagonal = another_diagonal.getBaseVector() * (sqrt(2.) / 2);

    this->abscissa_unit_vector = middle - sq_diagonal - zero;
    this->ordinata_unit_vector = middle + sq_diagonal - zero;

    if (this->abscissa_unit_vector.length() != 1.0)
    {
        abscissa_unit_vector *= (1. / abscissa_unit_vector.length());
        ordinata_unit_vector *= (1. / ordinata_unit_vector.length());
    }

    this->abscissa_axis = Line3d(zero, zero + this->abscissa_unit_vector);
    this->ordinata_axis = Line3d(zero, zero + this->ordinata_unit_vector);

    has_counted_units_vectors = true;
}*/

Line3d Plane::getIntersection(const Plane & plane)
{

    if (areParallel(*this, plane))
        throw QIODevice::tr("Нельзя пересечь параллельные или совпадающие плоскости");
    //это методом гаусса. плоскости не совпадают и не параллельны
    std::vector<std::vector<AlgExpr>> extended_equation_mattrix =
    {
        { this->_a, this->_b, this->_c, -this->_d},
        { plane._a, plane._b, plane._c, -plane._d}
    };
    auto get_index_of_first_non_zero = [](std::vector<AlgExpr> & vec)->int {
        for (int i = 0; i < vec.size(); ++i)
            if (vec[i] != 0)
                return i;
        return vec.size();
    };
    int ind1 = get_index_of_first_non_zero(extended_equation_mattrix[0]);
    int ind2 = get_index_of_first_non_zero(extended_equation_mattrix[1]);
    if (ind1 > ind2)
    {
        std::swap(extended_equation_mattrix[0], extended_equation_mattrix[1]);
        std::swap(ind1, ind2);
    }
    if (ind1 == ind2)
    {
        AlgExpr multiplier = -extended_equation_mattrix[1][ind2] / extended_equation_mattrix[0][ind1];
        extended_equation_mattrix[1][ind2] = 0;
        ++ind2;
        for (int i = ind2; i < extended_equation_mattrix[1].size(); ++i)
            extended_equation_mattrix[1][i] += (extended_equation_mattrix[0][i] * multiplier);
        ind2 = get_index_of_first_non_zero(extended_equation_mattrix[1]);
    }
    AlgExpr multiplier = -extended_equation_mattrix[0][ind2] / extended_equation_mattrix[1][ind2];

    for (int i = ind2; i < extended_equation_mattrix[0].size(); ++i)
        extended_equation_mattrix[0][i] += (extended_equation_mattrix[1][i] * multiplier);
    ind1 = get_index_of_first_non_zero(extended_equation_mattrix[0]);
    auto div_on_lead_element = [](std::vector<AlgExpr>& vec)->void {
        AlgExpr lead_el = 0;
        for (int i = 0; i < vec.size(); ++i)
        {
            if (lead_el == 0)
                lead_el = vec[i];
            if (lead_el != 0)
                vec[i] /= lead_el;
        }
    };
    div_on_lead_element(extended_equation_mattrix[0]);
    div_on_lead_element(extended_equation_mattrix[1]);
    assert(ind1 != ind2 && ind1 < ind2 && extended_equation_mattrix[0][ind1] == 1 && extended_equation_mattrix[1][ind2] == 1 &&
        extended_equation_mattrix[0][ind2] == 0 && extended_equation_mattrix[1][ind1] == 0 && ind1 >= 0 && ind1 <= 2 && ind2 >= 0 && ind2 <= 2);

    std::vector<AlgExpr> res0(3);
    std::vector<AlgExpr> rest(3);
    res0[3 - ind1 - ind2] = 0;
    rest[3 - ind1 - ind2] = 1;
    res0[ind1] = extended_equation_mattrix[0][3];
    rest[ind1] = -extended_equation_mattrix[0][3 - ind1 - ind2];
    res0[ind2] = extended_equation_mattrix[1][3];
    rest[ind2] = -extended_equation_mattrix[1][3 - ind1 - ind2];
    //ставя минус перед res0 мы получаем результат решения уравнения. без минуса - точку
    AlgVector point_on_line(res0[0], res0[1], res0[2]);
    AlgVector base_vector(rest[0], rest[1], rest[2]);
    return Line3d(point_on_line, point_on_line + base_vector);
}

void Plane::getRidOfGcd()
{
    AlgExpr com = gcd(gcd(this->_a, this->_b), gcd(this->_c, this->_d));
    if (com != 1)
    {
        this->_a /= com;
        this->_b /= com;
        this->_c /= com;
        this->_d /= com;
    }

}
/*Vector2 Plane::getCoordinatesInPlanesUnitsVector(const Vector3d & point) const
{
    assert(this->has_counted_units_vectors);
    Vector2 preresult = Vector2(distance(point, this->ordinata_axis), distance(point, this->abscissa_axis));
    if (point == this->abscissa_unit_vector * preresult.x() + this->ordinata_unit_vector * preresult.y())
        return preresult;
    if (point == this->abscissa_unit_vector * (-preresult.x()) + this->ordinata_unit_vector * preresult.y())
        return Vector2(-preresult.x(), preresult.y());
    if (point == this->abscissa_unit_vector * preresult.x() + this->ordinata_unit_vector * (-preresult.y()))
        return Vector2(preresult.x(), -preresult.y());
    return preresult * -1;
}*/

Plane getPlaneThroughPointAndTwoVectors(const AlgVector& point, const AlgVector& vector1, const AlgVector& vector2)
{
    AlgExpr D1 = (vector1.y() * vector2.z() - vector1.z() * vector2.y());
    AlgExpr D2 = (vector1.x() * vector2.z() - vector1.z() * vector2.x());
    AlgExpr D3 = (vector1.x() * vector2.y() - vector1.y() * vector2.x());

    return Plane(D1, -D2, D3, -point.x() * D1 + point.y() * D2 - point.z() * D3);
}

Plane getPlaneThroughThreePoints(const AlgVector &point1, const AlgVector &point2, const AlgVector &point3)
{
    return getPlaneThroughPointAndTwoVectors(point1, point2 - point1, point3 - point1);
}
