#ifndef POLYNOM_H
#define POLYNOM_H
#include <vector>
#include "galuafield.h"
//класс для работы с многочленами с целочисленными коэффициентами. В отличие от Polynomial, этот класс не является суммой любых каких-л функций, не используется
//для упрощения выражений и нужен только для реализации алгоритма Берлекэмпа. Этот класс может работать над простыми полями Галуа
class Polynom
{
    int degree;
    //индекс соответствует степени переменной, индекс 0 - свободный член
    std::vector<GfNumber> polynom;
public:
    Polynom(const std::vector<long long int>& coefs);
    Polynom(const std::vector<int>& coefs);
    Polynom(const std::vector<GfNumber>& coefs);
    Polynom(std::vector<GfNumber> && coefs);
    Polynom(int deg);
    int deg() const;
    int size() const;
    GfNumber& operator[](int i);
    const GfNumber& operator[](int i) const;

    Polynom& operator+=(const Polynom& right);
    Polynom& operator-=(const Polynom& right);
    Polynom& operator*=(int num);
    Polynom& operator*=(const Polynom& right);
    Polynom& operator/=(GfNumber num);

    void cutZeroDegrees();
};

Polynom operator+(const Polynom& left, const Polynom & right);
Polynom operator-(const Polynom& left, const Polynom & right);
Polynom operator*(int num, const Polynom &mult);
Polynom operator*(const Polynom& mult, int num);
Polynom operator*(const Polynom& left, const Polynom &right);
Polynom operator/(const Polynom& left, GfNumber num);
std::pair<Polynom, Polynom> divide(Polynom left, const Polynom& right);
#endif // POLYNOM_H
