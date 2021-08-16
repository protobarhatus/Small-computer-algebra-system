#ifndef SOLVING_EQUATIONS_H
#define SOLVING_EQUATIONS_H
#include "abstractexpression.h"
#include "equationrootsconditions.h"
class Fractal;
bool isPolynomOfAllVariables(const abs_ex & equation);
std::list<abs_ex> solveEquation(const abs_ex & equation, int var);
std::list<abs_ex> solveEquation(const abs_ex & equation, int var, const EquationRootsConditions & conditions);
//то есть несколько уравнений на одну переменную: это для того чтобы найти t когда r(t) == v(t), r, v - вектора
std::list<abs_ex> solveEquationsForOneVariable(const std::list<abs_ex> & equations, int var, const EquationRootsConditions & conditions);

bool isIntegratingConstantAddictiveThatCanBeChanged(const Fractal *it);
bool isIntegratingConstantAddictiveThatCanBeChanged(const std::unique_ptr<Fractal> & it);
bool isIntegratingConstantAddictive(const std::unique_ptr<Fractal> & it);
bool isIntegratingConstantAddictive(const Fractal * it);

//в полиноме p[x] (с возможными буквенными коэффициентами) попытается выделить часть
//(g[x])^n такую, что p[x] = (g[x])^n + a, где a - выражение, не зависящее от x, а x - переменная с id = var
//возвращает {g, n}
std::pair<abs_ex, int> tryToDistingushFullDegreeWithPrecisionOfCoefficientWithoutVariable(const abs_ex & polynom, int var);
std::pair<std::list<abs_ex >, Number> factorizePolynom(const abs_ex & polynom);
std::list<abs_ex> toFactorOfCoefs(const abs_ex & polynom);
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars);



//вот это делаем для простоты для однородных дифуров высших порядков
class ComplexNum
{
    abs_ex _a;
    abs_ex _b;

public:
    ComplexNum(const abs_ex & _a, const abs_ex & _b);
    ComplexNum(abs_ex && _a, abs_ex && _b);
    ComplexNum(const abs_ex & _a);
    ComplexNum(abs_ex && _a);
    bool isReal() const;
    ComplexNum(const ComplexNum & num);
    ComplexNum(ComplexNum && num);
    ComplexNum& operator=(const ComplexNum & num);
    ComplexNum& operator=(ComplexNum && num);

    QString toString() const;

    const abs_ex& a() const;
    const abs_ex& b() const;
};
bool operator==(const ComplexNum & a, const ComplexNum & b);
//не решает линейные уравнения, возвращает всегда n корней. в листе первое - корень, второе - кратность корня
//булевое значение - удалось ли полностью решить уравнение (найти все n комплексных корней)
//при этом, если корень комлексный, то один объект complexnum идет за два сопряженных корня
//число рядом с ним будет означать количество таких комплексно сопряженных ПАР
std::pair<std::list<std::pair<ComplexNum, int> >, bool> solvePolynomialEquationInComplexNumber(const abs_ex & difur, int var);
#endif // SOLVING_EQUATIONS_H
