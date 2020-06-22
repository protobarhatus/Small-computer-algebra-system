#ifndef SOLVING_EQUATIONS_H
#define SOLVING_EQUATIONS_H
#include "abstractexpression.h"
#include "equationrootsconditions.h"
class Fractal;
bool isPolynomOfAllVariables(const abs_ex & equation);
std::list<abs_ex> solveEquation(const abs_ex & equation, int var);
std::list<abs_ex> solveEquation(const abs_ex & equation, int var, const EquationRootsConditions & conditions);
bool isIntegratingConstantAddictive(const std::unique_ptr<Fractal> & it);

//в полиноме p[x] (с возможными буквенными коэффициентами) попытается выделить часть
//(g[x])^n такую, что p[x] = (g[x])^n + a, где a - выражение, не зависящее от x, а x - переменная с id = var
//возвращает {g, n}
std::pair<abs_ex, int> tryToDistingushFullDegreeWithPrecisionOfCoefficientWithoutVariable(const abs_ex & polynom, int var);
std::pair<std::list<abs_ex >, Number> factorizePolynom(const abs_ex & polynom);
std::list<abs_ex> toFactorOfCoefs(const abs_ex & polynom);
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars);
#endif // SOLVING_EQUATIONS_H
