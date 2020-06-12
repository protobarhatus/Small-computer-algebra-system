#ifndef SOLVING_EQUATIONS_H
#define SOLVING_EQUATIONS_H
#include "abstractexpression.h"
#include "equationrootsconditions.h"

std::list<abs_ex> solveEquation(const abs_ex & equation, int var);
std::list<abs_ex> solveEquation(const abs_ex & equation, int var, const EquationRootsConditions & conditions);
std::pair<std::list<abs_ex >, Number> factorizePolynom(const abs_ex & polynom);
std::list<abs_ex> toFactorOfCoefs(const abs_ex & polynom);
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars);
#endif // SOLVING_EQUATIONS_H
