#include "numberpolynom.h"
#include "some_algebra_expression_conversions.h"
NumberPolynom::NumberPolynom()
{

}

NumberPolynom::NumberPolynom(const std::vector<long long int> &coefs)
{
    this->polynom.resize(coefs.size());
    for (int i = 0; i < this->polynom.size(); ++i)
        this->polynom[i] = coefs[i];
    this->degree = this->polynom.size() - 1;
}

NumberPolynom::NumberPolynom(const std::vector<int> &coefs)
{
    this->polynom.resize(coefs.size());
    for (int i = 0; i < this->polynom.size(); ++i)
        this->polynom[i] = coefs[i];
    this->degree = this->polynom.size() - 1;

}

NumberPolynom::NumberPolynom(const std::vector<Number> &coefs)
{
    this->polynom = coefs;
    this->degree = this->polynom.size() - 1;
}

NumberPolynom::NumberPolynom(std::vector<Number> &&coefs)
{
    this->polynom = std::move(coefs);
    this->degree = this->polynom.size() - 1;
}

NumberPolynom::NumberPolynom(const std::initializer_list<int> &list)
{
    //лист задается в формате {1, 2, 3, 4}, что для пользователя интуитивно означает
    //x^3 + 2x^2+3x+4. Однако, так как в векторе позиция равна степени, конструктор через вектор
    //задаст 4x^3 + 3x^2 + 2x + 1. поэтому этот конструктор разворачивает initializer_list
    this->polynom.resize(list.size());
    int ind = this->polynom.size() - 1;
    for (auto &it : list)
    {
        this->polynom[ind] = it;
        --ind;
    }
    this->degree = this->polynom.size() - 1;
}

NumberPolynom::NumberPolynom(int deg)
{
    this->polynom = std::vector<Number>(deg + 1);
    this->degree = this->polynom.size() - 1;
}

NumberPolynom::NumberPolynom(const NumberPolynom &second)
{
    this->polynom = second.polynom;
    this->degree = second.degree;
}

NumberPolynom::NumberPolynom(NumberPolynom &&second)
{
    this->polynom = std::move(second.polynom);
    this->degree = second.degree;
}

NumberPolynom::NumberPolynom(const Polynom &pol)
{
    this->degree = pol.deg();
    this->polynom.resize(degree + 1);
    for (int i = 0; i < pol.size(); ++i)
        this->polynom[i] = pol[i].toInt();
}

NumberPolynom &NumberPolynom::operator=(const NumberPolynom &second)
{
    this->polynom = second.polynom;
    this->degree = second.degree;
    return *this;
}

NumberPolynom &NumberPolynom::operator=(NumberPolynom &&second)
{
    this->polynom = std::move(second.polynom);
    this->degree = second.degree;
    return *this;
}

int NumberPolynom::deg() const
{
    return this->degree;
}

int NumberPolynom::size() const
{
    return this->degree + 1;
}

Number &NumberPolynom::operator[](int i)
{

    return this->polynom[i];
}

const Number &NumberPolynom::operator[](int i) const
{
    return this->polynom[i];
}

NumberPolynom &NumberPolynom::operator+=(const NumberPolynom &right)
{
    *this = *this + right;
    return *this;
}

NumberPolynom &NumberPolynom::operator-=(const NumberPolynom &right)
{
    *this = *this - right;
    return *this;
}

NumberPolynom &NumberPolynom::operator*=(int num)
{
    *this = *this * num;
    return *this;
}

NumberPolynom &NumberPolynom::operator*=(const NumberPolynom &right)
{
    *this = *this * right;
    return *this;
}

NumberPolynom &NumberPolynom::operator/=(Number num)
{
    *this = *this / num;
    return *this;
}

Polynom NumberPolynom::toPolynom()
{
    long long int lcm_of_denominators = 1;
    for (int i = 0; i < this->size(); ++i)
        lcm_of_denominators = lcm(lcm_of_denominators, this->polynom[i].getDenominator());

    Polynom res(this->degree);
    for (int i = 0; i < this->size(); ++i)
        res[i] = (this->polynom[i] * lcm_of_denominators).getNumerator();
    return res;
}

Vector<Number> NumberPolynom::getVectorOfCoefficients()
{
    return Vector<Number>(this->polynom);
}

void NumberPolynom::cutZeroDegrees()
{
    int first_non_zero_degree = 0;
    for (int i = this->deg(); i >= 0; --i)
        if (this->polynom[i] != 0)
        {
            first_non_zero_degree = i;
            break;
        }
    this->polynom.resize(first_non_zero_degree + 1);
    this->degree = first_non_zero_degree;
}

void NumberPolynom::extendToDegree(int deg)
{
    this->polynom.resize(deg + 1);
}

void NumberPolynom::normalize()
{
    Number first = this->polynom[degree];
    for (auto &it : this->polynom)
        it /= first;

}

bool NumberPolynom::isNull()
{
    for (auto &it : this->polynom)
        if (it != 0)
            return false;
    return true;
}

NumberPolynom operator+(const NumberPolynom &left, const NumberPolynom &right)
{
    NumberPolynom res(std::max(left.deg(), right.deg()));
    const NumberPolynom& min = (left.size() <= right.size() ? left : right);
    const NumberPolynom& max = (left.size() > right.size() ? left : right);
    for (int i = 0; i < min.size(); ++i)
        res[i] = left[i] + right[i];
    for (int i = min.size(); i < max.size(); ++i)
        res[i] = max[i];
    return res;
}

NumberPolynom operator-(const NumberPolynom &left, const NumberPolynom &right)
{
    NumberPolynom res(std::max(left.deg(), right.deg()));
    const NumberPolynom& min = (left.size() <= right.size() ? left : right);
    const NumberPolynom& max = (left.size() > right.size() ? left : right);
    for (int i = 0; i < min.size(); ++i)
        res[i] = left[i] - right[i];
    for (int i = min.size(); i < max.size(); ++i)
        res[i] = max[i]*(left.size() > right.size() ? 1 : -1);
    return res;
}

NumberPolynom operator*(long long int num, const NumberPolynom &mult)
{
    NumberPolynom res(mult.deg());
    for (int i = 0; i < mult.size(); ++i)
        res[i] = mult[i] * num;
    return res;
}

NumberPolynom operator*(const NumberPolynom &mult, long long int num)
{
    return num*mult;
}

NumberPolynom operator*(const NumberPolynom &left, const NumberPolynom &right)
{
    NumberPolynom res(left.deg() + right.deg());
    for (int i = 0; i <= left.deg(); ++i)
    {
        for (int j = 0; j <= right.deg(); ++j)
            res[i + j] += left[i] * right[j];
    }
    return res;
}

NumberPolynom operator/(const NumberPolynom &left, Number num)
{
    NumberPolynom res(left.deg());
    for (int i = 0; i <= left.deg(); ++i)
        res[i] = left[i] / num;
    return res;
}

std::pair<NumberPolynom, NumberPolynom> divide(NumberPolynom left, const NumberPolynom &right)
{
    if (left.deg() < right.deg())
    {
        NumberPolynom zero(0);
        zero[0] = 0;
        return {zero, left};
    }
    NumberPolynom result(left.deg() - right.deg());
    for (int i = left.deg(); i >= right.deg(); --i)
    {;
        result[i - right.deg()] = left[i] / right[right.deg()];
        NumberPolynom multiplier(i - right.deg());
        multiplier[i - right.deg()] = result[i - right.deg()];
        NumberPolynom substr = right * multiplier;
        left -= right * multiplier;
    }
    left.cutZeroDegrees();
    return {result, left};
}

bool operator==(const NumberPolynom &left, const NumberPolynom &right)
{
    if (left.deg() != right.deg())
        return false;
    for (int i = 0; i < left.size(); ++i)
        if (left[i] != right[i])
            return false;
    return true;
}

bool operator!=(const NumberPolynom &left, const NumberPolynom &right)
{
    return !(left == right);
}

NumberPolynom operator%(const NumberPolynom &left, const NumberPolynom &right)
{
    return divide(left, right).second;
}

NumberPolynom operator/(const NumberPolynom &left, const NumberPolynom &right)
{
    return divide(left, right).first;
}
