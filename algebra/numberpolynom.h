#ifndef NUMBERNumberPolynom_H
#define NUMBERNumberPolynom_H
#include <vector>
#include "Vector.h"
#include "number.h"
#include "polynom.h"
//по сути, NumberPolynom с другим типом по-хорошему, мне бы стоило воспользоваться шаблонами, но я так не хочу: в данной ситуации проще сделать так
class NumberPolynom
{
    int degree;
    //индекс соответствует степени переменной, индекс 0 - свободный член
    std::vector<Number> polynom;
public:
    NumberPolynom();
    NumberPolynom(const std::vector<long long int>& coefs);
    NumberPolynom(const std::vector<int>& coefs);
    NumberPolynom(const std::vector<Number>& coefs);
    NumberPolynom(std::vector<Number> && coefs);
    NumberPolynom(const std::initializer_list<int> & list);
    NumberPolynom(int deg);
    NumberPolynom(const NumberPolynom & second);
    NumberPolynom(NumberPolynom && second);
    NumberPolynom(const Polynom & pol);

    NumberPolynom& operator=(const NumberPolynom & second);
    NumberPolynom& operator=(NumberPolynom && second);
    int deg() const;
    int size() const;
    Number& operator[](int i);
    const Number& operator[](int i) const;

    NumberPolynom& operator+=(const NumberPolynom& right);
    NumberPolynom& operator-=(const NumberPolynom& right);
    NumberPolynom& operator*=(int num);
    NumberPolynom& operator*=(const NumberPolynom& right);
    NumberPolynom& operator/=(Number num);

    Polynom toPolynom();
    Vector<Number> getVectorOfCoefficients();
    void cutZeroDegrees();
    void extendToDegree(int deg);
    void normalize();
    bool isNull();
};

NumberPolynom operator+(const NumberPolynom& left, const NumberPolynom & right);
NumberPolynom operator-(const NumberPolynom& left, const NumberPolynom & right);
NumberPolynom operator*(long long int num, const NumberPolynom &mult);
NumberPolynom operator*(const NumberPolynom& mult, long long int num);
NumberPolynom operator*(const NumberPolynom& left, const NumberPolynom &right);
NumberPolynom operator/(const NumberPolynom& left, Number num);
NumberPolynom operator/(const NumberPolynom& left, long long int num);
NumberPolynom operator/(const NumberPolynom& left, const NumberPolynom& right);
NumberPolynom operator%(const NumberPolynom& left, const NumberPolynom& right);

#endif // NUMBERNumberPolynom_H
