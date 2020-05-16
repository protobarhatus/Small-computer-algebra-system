#ifndef ALGEXPR_H
#define ALGEXPR_H
#include "abstractexpression.h"
#include "degree.h"
#include "fractal.h"
#include "number.h"
#include "polynomial.h"
#include "variable.h"
#include "variablesdistributor.h"
#include "absolutevalue.h"
class AlgExpr
{
public:
    AlgExpr();
    AlgExpr(const AlgExpr & expr);
    AlgExpr(AlgExpr && expr);
    AlgExpr(Number num);
    AlgExpr(long long int num);
    AlgExpr(const abs_ex & expr);
    AlgExpr(abs_ex && expr);


    AlgExpr& operator=(const AlgExpr & expr);
    AlgExpr& operator=(AlgExpr && expr);
    AlgExpr& operator=(Number num);
    AlgExpr& operator=(long long int num);
    AlgExpr& operator=(const abs_ex & expr);
    AlgExpr& operator=(abs_ex&& expr);

    AlgExpr& operator+=(const AlgExpr & expr);
    AlgExpr& operator-=(const AlgExpr & expr);
    AlgExpr& operator*=(const AlgExpr & expr);
    AlgExpr& operator/=(const AlgExpr & expr);
    AlgExpr operator-() const;
    bool operator==(const AlgExpr & expr) const;
    bool operator==(Number num) const;
    bool operator==(long long int num) const;
    bool operator==(char letter) const;
    bool operator==(QString var_name) const;
    bool operator==(int num) const;
    bool operator!=(const AlgExpr & expr) const;
    bool operator!=(Number num) const;
    bool operator!=(long long int num) const;
    bool operator!=(char letter) const;
    bool operator!=(QString var_name) const;
    friend AlgExpr pow(const AlgExpr & arg, const AlgExpr & degree);
    friend AlgExpr sqrt(const AlgExpr & arg);
    friend AlgExpr operator/(long long int left, const AlgExpr & right);
    void out();
    QString toString();
    friend void testAlgMod();
    friend void _neuLData();
    friend AlgExpr var();
    friend AlgExpr positiveVar();
    friend AlgExpr var(VariablesDefinition );
    friend AlgExpr abs(AlgExpr & arg);
    friend AlgExpr abs(AlgExpr && arg);
    friend AlgExpr sin(AlgExpr & arg);
    friend AlgExpr sin(AlgExpr && arg);
    friend AlgExpr cos(AlgExpr & arg);
    friend AlgExpr cos(AlgExpr && arg);
    friend AlgExpr tan(AlgExpr & arg);
    friend AlgExpr tan(AlgExpr && arg);
    friend AlgExpr cot(AlgExpr & arg);
    friend AlgExpr cot(AlgExpr && arg);
    friend AlgExpr pi();
    friend AlgExpr euler();
    friend AlgExpr ln(AlgExpr & arg);
    friend AlgExpr ln(AlgExpr && arg);
    friend AlgExpr derivative(const AlgExpr & arg, AlgExpr arg_variable);
    friend AlgExpr D(const AlgExpr & arg);
    friend AlgExpr D(AlgExpr && arg);


    friend AlgExpr operator+(const AlgExpr & left, const AlgExpr & right);
    friend AlgExpr operator+(const AlgExpr & left, AlgExpr && right);
    friend AlgExpr operator+(AlgExpr && left, const AlgExpr & right);
    friend AlgExpr operator+(AlgExpr && left, AlgExpr && right);
    friend AlgExpr operator-(const AlgExpr & left, const AlgExpr & right);
    friend AlgExpr operator-(const AlgExpr & left, AlgExpr && right);
    friend AlgExpr operator-(AlgExpr && left, const AlgExpr & right);
    friend AlgExpr operator-(AlgExpr && left, AlgExpr && right);
    friend AlgExpr operator*(const AlgExpr & left, const AlgExpr & right);
    friend AlgExpr operator*(const AlgExpr & left, AlgExpr && right);
    friend AlgExpr operator*(AlgExpr && left, const AlgExpr & right);
    friend AlgExpr operator*(AlgExpr && left, AlgExpr && right);
    friend AlgExpr operator/(const AlgExpr & left, const AlgExpr & right);
    friend AlgExpr operator/(const AlgExpr & left, AlgExpr && right);
    friend AlgExpr operator/(AlgExpr && left, const AlgExpr & right);
    friend AlgExpr operator/(AlgExpr && left, AlgExpr && right);

    abs_ex& getExpr();
    const abs_ex& getExpr() const;

private:
    bool isEqualTo(const AlgExpr & sec) const;
    std::unique_ptr<AbstractExpression> expression;
};

AlgExpr operator+(const AlgExpr & left, const AlgExpr & right);
AlgExpr operator+(const AlgExpr & left, AlgExpr && right);
AlgExpr operator+(AlgExpr && left, const AlgExpr & right);
AlgExpr operator+(AlgExpr && left, AlgExpr && right);
AlgExpr operator-(const AlgExpr & left, const AlgExpr & right);
AlgExpr operator-(const AlgExpr & left, AlgExpr && right);
AlgExpr operator-(AlgExpr && left, const AlgExpr & right);
AlgExpr operator-(AlgExpr && left, AlgExpr && right);
AlgExpr operator*(const AlgExpr & left, const AlgExpr & right);
AlgExpr operator*(const AlgExpr & left, AlgExpr && right);
AlgExpr operator*(AlgExpr && left, const AlgExpr & right);
AlgExpr operator*(AlgExpr && left, AlgExpr && right);
AlgExpr operator/(const AlgExpr & left, const AlgExpr & right);
AlgExpr operator/(const AlgExpr & left, AlgExpr && right);
AlgExpr operator/(AlgExpr && left, const AlgExpr & right);
AlgExpr operator/(AlgExpr && left, AlgExpr && right);
//creates new variable with empty definition
AlgExpr var();
AlgExpr positiveVar();
AlgExpr var(VariablesDefinition );
AlgExpr var(double min, double max);
AlgExpr operator+(long long int left, const AlgExpr & expr);
AlgExpr operator-(long long int left, const AlgExpr & right);
AlgExpr operator*(long long int left, const AlgExpr & right);
AlgExpr operator/(long long int left, const AlgExpr & right);
AlgExpr operator+(long long int left, AlgExpr && expr);
AlgExpr operator-(long long int left, AlgExpr && right);
AlgExpr operator*(long long int left, AlgExpr && right);
AlgExpr operator/(long long int left, AlgExpr && right);
bool operator==(long long int left, const AlgExpr & right);
bool operator!=(long long int left, const AlgExpr & right);
AlgExpr pow(const AlgExpr & arg, const AlgExpr & degree);
AlgExpr root(const AlgExpr & arg, const AlgExpr & degree);
AlgExpr sqrt(const AlgExpr & arg);
AlgExpr sqrt(int arg);
AlgExpr abs(AlgExpr & arg);
AlgExpr abs(AlgExpr && arg);
AlgExpr sin(AlgExpr & arg);
AlgExpr sin(AlgExpr && arg);
AlgExpr sin(int arg);
AlgExpr pi();
AlgExpr euler();
AlgExpr e();
AlgExpr cos(AlgExpr & arg);
AlgExpr cos(AlgExpr && arg);
AlgExpr cos(int arg);
AlgExpr tan(AlgExpr & arg);
AlgExpr tan(AlgExpr && arg);
AlgExpr tan(int arg);
AlgExpr cot(AlgExpr & arg);
AlgExpr cot(AlgExpr && arg);
AlgExpr cot(int arg);
AlgExpr ln(AlgExpr & arg);
AlgExpr ln(AlgExpr && arg);
AlgExpr ln(int arg);
AlgExpr log(AlgExpr & arg, AlgExpr & base);
AlgExpr log(AlgExpr && arg, AlgExpr & base);
AlgExpr log(AlgExpr & arg, AlgExpr && base);
AlgExpr log(AlgExpr && arg, AlgExpr && base);
AlgExpr derivative(const AlgExpr & arg, AlgExpr arg_variable);
//да, с большой буквы, но так надо. не буду же я называть ее dif или differential
AlgExpr D(const AlgExpr & arg);
AlgExpr D(AlgExpr && arg);
#endif // ALGEXPR_H
