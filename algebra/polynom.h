#ifndef POLYNOM_H
#define POLYNOM_H
#include <vector>
#include "galuafield.h"
#include "Vector.h"
//класс для работы с многочленами с целочисленными коэффициентами. В отличие от Polynomial, этот класс не является суммой любых каких-л функций, не используется
//для упрощения выражений и нужен только для реализации алгоритма Берлекэмпа. Этот класс может работать над простыми полями Галуа
class Polynom
{
    int degree;
    //индекс соответствует степени переменной, индекс 0 - свободный член
    std::vector<GfNumber> polynom;
public:
    Polynom();
    Polynom(const std::vector<long long int>& coefs);
    Polynom(const std::vector<int>& coefs);
    Polynom(const std::vector<GfNumber>& coefs);
    Polynom(std::vector<GfNumber> && coefs);
    Polynom(const std::initializer_list<int> & list);
    Polynom(int deg);
    Polynom(const Polynom & second);
    Polynom(Polynom && second);
    Polynom& operator=(const Polynom & second);
    Polynom& operator=(Polynom && second);
    int deg() const;
    int size() const;
    GfNumber& operator[](int i);
    const GfNumber& operator[](int i) const;

    Polynom& operator+=(const Polynom& right);
    Polynom& operator-=(const Polynom& right);
    Polynom& operator*=(int num);
    Polynom& operator*=(const Polynom& right);
    Polynom& operator/=(GfNumber num);

    Vector<GfNumber> getVectorOfCoefficients();
    void cutZeroDegrees();
    void extendToDegree(int deg);
    void normalize();
    bool isNull();
};
long long int gcdOfCoefficients(const Polynom & polynom);
Polynom operator+(const Polynom& left, const Polynom & right);
Polynom operator-(const Polynom& left, const Polynom & right);
Polynom operator*(long long int num, const Polynom &mult);
Polynom operator*(const Polynom& mult, long long int num);
Polynom operator*(const Polynom& left, const Polynom &right);
Polynom operator/(const Polynom& left, GfNumber num);
Polynom operator/(const Polynom& left, long long int num);
Polynom operator/(const Polynom& left, const Polynom& right);
Polynom operator%(const Polynom& left, const Polynom& right);
double norma2(const Polynom & p);
Polynom gcd(Polynom left, Polynom right);
bool operator==(const Polynom& left, const Polynom& right);
bool operator!=(const Polynom& left, const Polynom& right);
std::pair<Polynom, Polynom> divide(Polynom left, const Polynom& right);
Polynom pow(const Polynom& polynom, int degree);
std::pair<Polynom, std::pair<Polynom, Polynom>> xea(const Polynom & p1, const Polynom & p2);
Polynom numberPolynom(GfNumber num);
//для полиномов таких что g*a + h*b = 1 в Zp^j и их старшие коэффициенты обратимы в Zp^j
//вычисляет полиномы a' и b' такие, что g*a' + h*b' = c над Zp^j и deg(a') < deg(h)
std::pair<Polynom, Polynom> solveSomePolynomialEquation(const Polynom & a, const Polynom & b,
                                                        const Polynom & c, const Polynom & g, const Polynom & h);
Polynom derivative(const Polynom & polynom);
long long int lc(const Polynom & p);
#endif // POLYNOM_H
