#include "polynom.h"
#include <algorithm>
#include "assert.h"
#include "numberpolynom.h"
Polynom::Polynom()
{

}

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

Polynom::Polynom(const std::initializer_list<int> &list)
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

Polynom::Polynom(int deg)
{
    this->polynom = std::vector<GfNumber>(deg + 1);
    this->degree = this->polynom.size() - 1;
}

Polynom::Polynom(const Polynom &second)
{
    this->polynom = second.polynom;
    this->degree = second.degree;
}

Polynom::Polynom(Polynom &&second)
{
    this->polynom = std::move(second.polynom);
    this->degree = second.degree;
}

Polynom &Polynom::operator=(const Polynom &second)
{
    this->polynom = second.polynom;
    this->degree = second.degree;
    return *this;
}

Polynom &Polynom::operator=(Polynom &&second)
{
    this->polynom = std::move(second.polynom);
    this->degree = second.degree;
    return *this;
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

Vector<GfNumber> Polynom::getVectorOfCoefficients()
{
    return Vector<GfNumber>(this->polynom);
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

void Polynom::extendToDegree(int deg)
{
    this->polynom.resize(deg + 1);
}

void Polynom::normalize()
{
    GfNumber first = this->polynom[degree];
    for (auto &it : this->polynom)
        it /= first;

}

bool Polynom::isNull()
{
    for (auto &it : this->polynom)
        if (it != 0)
            return false;
    return true;
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
        res[i] = (left.size() > right.size() ? 1 : -1)*max[i];
    return res;
}

Polynom operator*(long long int num, const Polynom &mult)
{
    Polynom res(mult.deg());
    for (int i = 0; i < mult.size(); ++i)
        res[i] = mult[i] * num;
    return res;
}

Polynom operator*(const Polynom &mult, long long int num)
{
    return num*mult;
}

Polynom operator*(const Polynom &left, const Polynom &right)
{
    Polynom res(left.deg() + right.deg());
    for (int i = 0; i <= left.deg(); ++i)
    {
        for (int j = 0; j <= right.deg(); ++j)
            res[i + j] += left[i] * right[j];
    }
    return res;
}

Polynom operator/(const Polynom &left, GfNumber num)
{
    Polynom res(left.deg());
    for (int i = 0; i <= left.deg(); ++i)
        res[i] = left[i] / num;
    return res;
}

std::pair<Polynom, Polynom> divide(Polynom left, const Polynom &right)
{
    if (left.deg() < right.deg())
    {
        Polynom zero(0);
        zero[0] = 0;
        return {zero, left};
    }
    Polynom result(left.deg() - right.deg());
    for (int i = left.deg(); i >= right.deg(); --i)
    {
        if (GaluaField::isOverIntegers() &&  left[i].toInt() % right[right.deg()].toInt() != 0)
            return {numberPolynom(0), numberPolynom(1)};
        result[i - right.deg()] = left[i] / right[right.deg()];
        Polynom multiplier(i - right.deg());
        multiplier[i - right.deg()] = result[i - right.deg()];
        Polynom substr = right * multiplier;
        left -= right * multiplier;
    }
    left.cutZeroDegrees();
    return {result, left};
}

bool operator==(const Polynom &left, const Polynom &right)
{
    if (left.deg() != right.deg())
        return false;
    for (int i = 0; i < left.size(); ++i)
        if (left[i] != right[i])
            return false;
    return true;
}

bool operator!=(const Polynom &left, const Polynom &right)
{
    return !(left == right);
}
long long int gcdOfCoefficients(const Polynom & polynom)
{
    long long int gcf = polynom[0].toInt();
    for (int i = 1; i < polynom.size(); ++i)
        gcf = gcd(gcf, polynom[i].toInt());
    return gcf;
}
Polynom gcd(Polynom pleft, Polynom pright)
{
    if (pright.deg() > pleft.deg())
        std::swap(pleft, pright);
    if (GaluaField::isOverIntegers())
    {
        try {
            NumberPolynom left(pleft), right(pright);
            auto remainder = left % right;
            while (!remainder.isNull())
            {
                left = std::move(right);
                right = std::move(remainder);
                remainder = left % right;
            }
            Polynom prob_gcd = right.toPolynom();
            prob_gcd /= gcdOfCoefficients(prob_gcd);
            if ((pleft % prob_gcd).isNull() && ((pright % prob_gcd).isNull()))
                return prob_gcd;
            return numberPolynom(1);
        } catch (...) {
            return numberPolynom(1);
        }

    }
    auto remainder = pleft % pright;
    while (!remainder.isNull())
    {
        pleft = std::move(pright);
        pright = std::move(remainder);
        remainder = pleft % pright;
    }
    return pright;
}

Polynom pow(const Polynom &polynom, int degree)
{
    if (degree == 1)
        return polynom;
    if (degree == 2)
        return polynom * polynom;
    return pow(polynom, degree / 2) * pow(polynom, degree - degree/2);
}

Polynom operator/(const Polynom &left, const Polynom &right)
{
    return divide(left, right).first;
}

Polynom operator%(const Polynom &left, const Polynom &right)
{
    return divide(left, right).second;
}

std::pair<Polynom, std::pair<Polynom, Polynom> > xea(const Polynom &p1, const Polynom &p2)
{
    std::pair<Polynom, Polynom> p = {p1, p2};
    std::pair<Polynom, Polynom> g = {numberPolynom(1), numberPolynom(0)};
    std::pair<Polynom, Polynom> f = {numberPolynom(0), numberPolynom(1)};
    while (p.second != numberPolynom(0))
    {
        Polynom q = p.first / p.second;
        p = {p.second, p.first - p.second * q};
        p.first.cutZeroDegrees();
        p.second.cutZeroDegrees();
        g = {g.second, g.first - g.second * q};
        g.first.cutZeroDegrees();
        g.second.cutZeroDegrees();
        f = {f.second, f.first - f.second * q};
        f.first.cutZeroDegrees();
        f.second.cutZeroDegrees();
    }
    p.first.cutZeroDegrees();
    g.first.cutZeroDegrees();
    f.first.cutZeroDegrees();
    return {p.first, {g.first, f.first}};

}

Polynom numberPolynom(GfNumber num)
{
    Polynom res(0);
    res[0] = num;
    return res;
}
//для полиномов таких что g*a + h*b = 1 в Zp^j и их старшие коэффициенты обратимы в Zp^j
//вычисляет полиномы a' и b' такие, что g*a' + h*b' = c над Zp^j и deg(a') < deg(h)
std::pair<Polynom, Polynom> solveSomePolynomialEquation(const Polynom &a, const Polynom &b, const Polynom &c, const Polynom &g, const Polynom &h)
{
    //Polynom check1 = g*a + h*b;

    auto div_res = divide(a*c, h);
    Polynom q = std::move(div_res.first);
    Polynom r = std::move(div_res.second);

    Polynom res = b*c + g*q;
    res.cutZeroDegrees();

    //Polynom check2 = g*r + h*res - c;
    return {r, res};
}

Polynom operator/(const Polynom &left, long long num)
{
    return left / GfNumber(num);
}

long long int lc(const Polynom &p)
{
    long long int max = std::numeric_limits<long long int>::min();
    for (int i = 0; i < p.size(); ++i)
        max = std::max(max, p[i].toInt());
    return max;
}

Polynom derivative(const Polynom &polynom)
{
    Polynom res(polynom.deg() - 1);
    for (int i = polynom.deg() - 1; i >= 0; --i)
        res[i] = (i + 1)*polynom[i + 1];
    res.cutZeroDegrees();
    return res;
}


double norma2(const Polynom &p)
{
    long long int sum = 0;
    for (int i = 0; i < p.size(); ++i)
        sum += abs(p[i].toInt());
    return sqrt(sum);
}
