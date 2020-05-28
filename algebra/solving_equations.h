#ifndef SOLVING_EQUATIONS_H
#define SOLVING_EQUATIONS_H
#include "abstractexpression.h"

std::list<abs_ex> factorizePolynom(const abs_ex & polynom, int var);
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars);
#endif // SOLVING_EQUATIONS_H
