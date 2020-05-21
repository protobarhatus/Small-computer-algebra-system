#include "polynom.h"
#include <algorithm>
#include "assert.h"

Polynom::Polynom(const std::vector<long long int> &coefs)
{
    this->polynom.resize(coefs.size());
    for (int i = 0; i < this->polynom.size(); ++i)
        this->polynom[i] = coefs[i];
    this->degree = this->polynom.size() - 1;
}

Polynom::Polynom(const std::vector<int> &coefs)
{
    this->polynom.resize(coefs.size());
    for (int i = 0; i < this->polynom.size(); ++i)
        this->polynom[i] = coefs[i];
    this->degree = this->polynom.size() - 1;

}

Polynom::Polynom(const std::vector<GfNumber> &coefs)
{
    this->polynom = coefs;
    this->degree = this->polynom.size() - 1;
}

Polynom::Polynom(std::vector<GfNumber> &&coefs)
{
    this->polynom = std::move(coefs);
    this->degree = this->polynom.size() - 1;
}

Polynom::Polynom(int deg)
{
    this->polynom = std::vector<GfNumber>(deg + 1);
    this->degree = this->polynom.size() - 1;
}

int Polynom::deg() const
{
    return this->degree;
}

int Polynom::size() const
{
    return this->degree + 1;
}

GfNumber &Polynom::operator[](int i)
{
    return this->polynom[i];
}

const GfNumber &Polynom::operator[](int i) const
{
    return this->polynom[i];
}

Polynom &Polynom::operator+=(const Polynom &right)
{
    *this = *this + right;
    return *this;
}

Polynom &Polynom::operator-=(const Polynom &right)
{
    *this = *this - right;
    return *this;
}

Polynom &Polynom::operator*=(int num)
{
    *this = *this * num;
    return *this;
}

Polynom &Polynom::operator*=(const Polynom &right)
{
    *this = *this * right;
    return *this;
}

Polynom &Polynom::operator/=(GfNumber num)
{
    *this = *this / num;
    return *this;
}

void Polynom::cutZeroDegrees()
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

Polynom operator+(const Polynom &left, const Polynom &right)
{
    Polynom res(std::max(left.deg(), right.deg()));
    const Polynom& min = (left.size() <= right.size() ? left : right);
    const Polynom& max = (left.size() > right.size() ? left : right);
    for (int i = 0; i < min.size(); ++i)
        res[i] = left[i] + right[i];
    for (int i = min.size(); i < max.size(); ++i)
        res[i] = max[i];
    return res;
}

Polynom operator-(const Polynom &left, const Polynom &right)
{
    Polynom res(std::max(left.deg(), right.deg()));
    const Polynom& min = (left.size() <= right.size() ? left : right);
    const Polynom& max = (left.size() > right.size() ? left : right);
    for (int i = 0; i < min.size(); ++i)
        res[i] = left[i] - right[i];
    for (int i = min.size(); i < max.size(); ++i)
        res[i] = max[i];
    return res;
}

Polynom operator*(int num, const Polynom &mult)
{
    Polynom res(mult.deg());
    for (int i = 0; i < mult.size(); ++i)
        res[i] = mult[i] * num;
    return res;
}

Polynom operator*(const Polynom &mult, int num)
{
    return num*mult;
}

Polynom operator*(const Polynom &left, const Polynom &right)
{
    Polynom res(left.deg() + right.deg());
    for (int i = 0; i <= left.deg(); ++i)
    {
        for (int j = 0; j <= right.deg(); ++j)
            res[i + j] = left[i] * right[j];
    }
    return res;
}

Polynom operator/(const Polynom &left, GfNumber num)
{
    Polynom res(left.deg());
    for (int i = 0; i <= left.size(); ++i)
        res[i] = left[i] / num;
    return res;
}

std::pair<Polynom, Polynom> divide(Polynom left, const Polynom &right)
{
    assert(left.deg() >= right.deg());
    Polynom result(left.deg() - right.deg());
    for (int i = left.deg(); i >= right.deg(); --i)
    {
        result[i - right.deg()] = left[i] / right[right.deg()];
        Polynom multiplier(i - right.deg());
        multiplier[i - right.deg()] = result[i - right.deg()];
        left -= right * multiplier;
    }
    left.cutZeroDegrees();
    return {result, left};
}
