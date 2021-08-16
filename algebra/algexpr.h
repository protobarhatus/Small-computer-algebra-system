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
#include "solving_differential_equations.h"
#include "equationrootsconditions.h"
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

    AlgExpr(nullptr_t);
    bool operator==(nullptr_t);


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

    AlgExpr& operator+=(AlgExpr && expr);
    AlgExpr& operator-=(AlgExpr && expr);
    AlgExpr& operator*=(AlgExpr && expr);
    AlgExpr& operator/=(AlgExpr && expr);
    AlgExpr operator-() const;
    bool operator==(const AlgExpr & expr) const;
    bool operator==(Number num) const;
    bool operator==(long long int num) const;
    //bool operator==(char letter) const;
    bool operator==(QString var_name) const;
    bool operator==(int num) const;
    bool operator!=(const AlgExpr & expr) const;
    bool operator!=(Number num) const;
    bool operator!=(long long int num) const;
    //bool operator!=(char letter) const;
    bool operator!=(QString var_name) const;
    friend AlgExpr pow(const AlgExpr & arg, const AlgExpr & degree);
    friend AlgExpr sqrt(const AlgExpr & arg);
    friend AlgExpr operator/(long long int left, const AlgExpr & right);
    void out();
    QString toString() const;
    QString toWolframString() const;
    friend void testAlgMod();
    friend void _neuLData();
    friend AlgExpr var();
    friend AlgExpr positiveVar();
    friend AlgExpr var(VariablesDefinition );
    friend AlgExpr abs(const AlgExpr & arg);
    friend AlgExpr abs(AlgExpr && arg);
    friend AlgExpr sin(const AlgExpr & arg);
    friend AlgExpr sin(AlgExpr && arg);
    friend AlgExpr cos(const AlgExpr & arg);
    friend AlgExpr cos(AlgExpr && arg);
    friend AlgExpr tan(const AlgExpr & arg);
    friend AlgExpr tan(AlgExpr && arg);
    friend AlgExpr cot(const AlgExpr & arg);
    friend AlgExpr cot(AlgExpr && arg);
    friend AlgExpr pi();
    friend AlgExpr euler();
    friend AlgExpr ln(const AlgExpr & arg);
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
    friend AlgExpr atan(const AlgExpr & arg);
    friend AlgExpr atan(AlgExpr && arg);
    friend AlgExpr asin(const AlgExpr & arg);
    friend AlgExpr asin(AlgExpr && arg);
    abs_ex& getExpr();
    const abs_ex& getExpr() const;

private:
    bool isEqualTo(const AlgExpr & sec) const;
    abs_ex expression;
};

bool operator>(const AlgExpr & a, const AlgExpr & b);
bool operator<(const AlgExpr & a, const AlgExpr & b);
bool operator>=(const AlgExpr & a, const AlgExpr & b);
bool operator<=(const AlgExpr & a, const AlgExpr & b);

bool isIntegerNumber(const AlgExpr & expr);
int toInt(const AlgExpr & expr);
//после преобразований, в получившимся выражении константы интегрирования обладают большими индексами, в то время, как C, C1, C2... не присутствуют в выражении
//данная функция изменит имена этих констант, добавив при этом в имя html тег
void downgradeIntegratingConstantsIndexes(AlgExpr & expr);
void downgradeIntegratingConstantsIndexes(abs_ex & expr);

std::pair<std::vector<AlgExpr>, std::vector<RootCondition>> selectRootsAndConditions(const std::vector<AlgExpr> & roots, const std::vector<RootCondition> & conditions, int var);
std::pair<std::vector<DifurResult>, std::vector<RootCondition>> selectRootsAndConditions(const std::vector<DifurResult> & roots, const std::vector<RootCondition> & conditions, int y);

bool isDifferentialEquation(const AlgExpr & eq);
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
AlgExpr positiveVar(const QString & name);
AlgExpr var(VariablesDefinition );
AlgExpr var(int min, int max);
AlgExpr var(const QString & name);
AlgExpr var(const VariablesDefinition & def, const QString & name);
AlgExpr undefinedVar(const QString & name);
AlgExpr undefinedVar();
AlgExpr undefinedVar(VariablesDefinition def, const QString & name);
AlgExpr positiveUndefinedVar(const QString & name);
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
AlgExpr root(const AlgExpr & arg, int deg);
AlgExpr sqrt(const AlgExpr & arg);
AlgExpr sqrt(int arg);
AlgExpr sqr(const AlgExpr & arg);
AlgExpr sqr(AlgExpr && arg);
AlgExpr abs(const AlgExpr & arg);
AlgExpr abs(AlgExpr && arg);
AlgExpr sin(const AlgExpr & arg);
AlgExpr sin(AlgExpr && arg);
AlgExpr sin(int arg);
AlgExpr pi();
AlgExpr euler();
AlgExpr inf();
AlgExpr minusInf();
AlgExpr e();
AlgExpr cos(const AlgExpr & arg);
AlgExpr cos(AlgExpr && arg);
AlgExpr cos(int arg);
AlgExpr tan(const AlgExpr & arg);
AlgExpr tan(AlgExpr && arg);
AlgExpr tan(int arg);
AlgExpr cot(const AlgExpr & arg);
AlgExpr cot(AlgExpr && arg);
AlgExpr cot(int arg);
AlgExpr ln(const AlgExpr & arg);
AlgExpr ln(AlgExpr && arg);
AlgExpr ln(int arg);
AlgExpr log(const AlgExpr & arg, const AlgExpr & base);
AlgExpr log(AlgExpr && arg, const AlgExpr & base);
AlgExpr log(const AlgExpr & arg, AlgExpr && base);
AlgExpr log(AlgExpr && arg, AlgExpr && base);
AlgExpr derivative(const AlgExpr & arg, AlgExpr arg_variable);
//да, с большой буквы, но так надо. не буду же я называть ее dif или differential
AlgExpr D(const AlgExpr & arg);
AlgExpr D(AlgExpr && arg);
AlgExpr integral(const AlgExpr & arg);
AlgExpr integral(const AlgExpr & arg, AlgExpr var);
AlgExpr definiteIntegral(const AlgExpr & arg, const AlgExpr & from, const AlgExpr & to);
AlgExpr atan(const AlgExpr & arg);
AlgExpr atan(AlgExpr && arg);
AlgExpr asin(const AlgExpr & arg);
AlgExpr asin(AlgExpr && arg);
AlgExpr acos(const AlgExpr & arg);
AlgExpr acos(AlgExpr && arg);
AlgExpr acot(const AlgExpr & arg);
AlgExpr acot(AlgExpr && arg);
AlgExpr cbrt(const AlgExpr & arg);
AlgExpr cbrt(AlgExpr && arg);
AlgExpr deriv(const AlgExpr & arg, AlgExpr arg_var, int order);
AlgExpr derivObj(const AlgExpr & func_var, int arg_var, int order);
AlgExpr derivObj(const AlgExpr & func_var, const AlgExpr & arg_var, int order);
AlgExpr factorize(const AlgExpr & expr);
AlgExpr expand(const AlgExpr & expr);
std::list<AlgExpr> solveEquation(const AlgExpr & equation, const AlgExpr & var);
std::pair<std::list<DifurResult>, std::vector<QString>> solveDifur(const AlgExpr & difur, const AlgExpr & x, const AlgExpr & y);
AlgExpr degToRad(const AlgExpr & deg);

template <typename T>
class Vector;
void replaceSystemVariablesToExpressions(Vector<AlgExpr> & expression, const std::map<int, abs_ex> & vars);
#endif // ALGEXPR_H
