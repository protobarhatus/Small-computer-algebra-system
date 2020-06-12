#include "solving_differential_equations.h"
#include "polynomial.h"
#include "fractal.h"
#include "number.h"
#include "degree.h"
#include "solving_equations.h"
#include "variablesdistributor.h"
#include "some_algebra_expression_conversions.h"
#include "constant.h"
bool isDifferentialOf(abs_ex & expr, int x)
{
    return expr->getId() == DIFFERENTIAL && expr->hasVariable(x);
}
//в логарифмах считаем только x и y
bool isLogarithmicSum(Polynomial * pol, int x, int y)
{
    for (auto &it : pol->getMonomialsPointers())
    {
        if (pol->hasVariable(x))
            if (it->getFractalWithoutVariable(x)->getId() != LOGARITHM)
                return false;
        if (pol->hasVariable(y))
            if (it->getFractalWithoutVariable(y)->getId() != LOGARITHM)
                return false;
    }
    return true;
}
abs_ex simplifyEquationExpression(const abs_ex & expr, int x, int y)
{
    if (expr->getId() > 0)
        return copy(expr);
    switch (expr->getId()) {
    case NUMBER:
        return copy(expr);
        break;
    //case POLYNOMIAL:




    }
}





abs_ex tryToSolveDifurWithSeparableVariables(const std::unique_ptr<Polynomial> & difur, int x, int y)
{
    std::unique_ptr<Polynomial> right(new Polynomial(zero.get()));
    std::unique_ptr<Polynomial> left(new Polynomial(zero.get()));
    for (auto &it : difur->getMonomialsPointers())
    {
        if (it->getFractal().first->size() == 0)
            return nullptr;
        if (isDifferentialOf(it->getFractal().first->back(), x))
            *right = *right + it;
        else if (isDifferentialOf(it->getFractal().first->back(), y))
            *left = *left + it;
        else
            return nullptr;
    }
    abs_ex right_common_part = right->reduceCommonPart();
    std::unique_ptr<Fractal> right_frac(new Fractal(right_common_part * copy(right.get())));

    abs_ex left_common_part = left->reduceCommonPart();
    std::unique_ptr<Fractal> left_frac(new Fractal(left_common_part * copy(left.get())));

    right_frac->separatePolynomialsDegree();
    left_frac->separatePolynomialsDegree();

    auto takeMultipliersWithVariablesAwayToOtherFractal = [](fractal_argument * from_erase,
                                                        fractal_argument * to_insert, int var_take, int var_stay)
    {
        for (auto it = from_erase->begin(); it != from_erase->end();)
        {
            if (it->get()->hasVariable(var_take) && !it->get()->hasVariable(var_stay))
            {
                to_insert->push_back(copy(it->get()));
                it = from_erase->erase(it);
            }
            else
                ++it;
        }
    };

    takeMultipliersWithVariablesAwayToOtherFractal(right_frac->getFractal().first, left_frac->getFractal().second,
                                                   y, x);
    takeMultipliersWithVariablesAwayToOtherFractal(right_frac->getFractal().second, left_frac->getFractal().first,
                                                   y, x);
    takeMultipliersWithVariablesAwayToOtherFractal(left_frac->getFractal().first, right_frac->getFractal().second,
                                                   x, y);
    takeMultipliersWithVariablesAwayToOtherFractal(left_frac->getFractal().second, left_frac->getFractal().first,
                                                   x, y);
    left_frac->simplify();
    right_frac->simplify();
    if (left_frac->hasVariable(x) || right_frac->hasVariable(y))
        return nullptr;
    auto left_integr = integrate(toAbsEx(left_frac));
    auto right_integr = integrate(toAbsEx(right_frac));

    if (left_integr == nullptr || right_integr == nullptr)
        return nullptr;
    auto ic = integratingConstantExpr();
    return left_integr - right_integr + ic;
}
abs_ex solveDifur(const abs_ex & difur, int x, int y)
{
    if (difur->getId() == NUMBER || difur->getId() == DIFFERENTIAL)
        return nullptr;
    auto dif_pol = toPolynomialPointer(difur);
    auto separable_vars_res = tryToSolveDifurWithSeparableVariables(dif_pol, x, y);
    if (separable_vars_res != nullptr)
        return separable_vars_res;

    return nullptr;
}
