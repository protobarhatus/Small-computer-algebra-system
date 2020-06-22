#include "solving_differential_equations.h"
#include "polynomial.h"
#include "fractal.h"
#include "number.h"
#include "degree.h"
#include "solving_equations.h"
#include "variablesdistributor.h"
#include "some_algebra_expression_conversions.h"
#include "constant.h"
#include "difequationrootconditions.h"
std::list<abs_ex> solveDifurInCommonIntegral(const abs_ex & difur, int x, int y,
                                             const DifursRootConditions & conditions);
bool isDifferentialOf(abs_ex & expr, int x)
{
    return expr->getId() == DIFFERENTIAL && expr->hasVariable(x);
}

bool isRootOfEquation(abs_ex && equation, const abs_ex & root, int var)
{
    qDebug() << root->makeStringOfExpression();
    qDebug() << equation->makeStringOfExpression();
    setUpExpressionIntoVariable(equation, root, var);
    qDebug() << equation->makeStringOfExpression();
    return isZero(equation);
}

std::list<abs_ex>  tryToSolveDifurWithSeparableVariables(const std::unique_ptr<Polynomial> & difur, int x, int y)
{
    std::unique_ptr<Polynomial> right(new Polynomial(zero.get()));
    std::unique_ptr<Polynomial> left(new Polynomial(zero.get()));
    for (auto &it : difur->getMonomialsPointers())
    {
        if (it->getFractal().first->size() == 0)
            return std::list<abs_ex> ();
        if (isDifferentialOf(it->getFractal().first->back(), x))
            *right = *right - it;
        else if (isDifferentialOf(it->getFractal().first->back(), y))
            *left = *left + it;
        else
            return std::list<abs_ex> ();
    }
    abs_ex right_common_part = right->reduceCommonPart();
    std::unique_ptr<Fractal> right_frac(new Fractal(right_common_part * copy(right.get())));

    abs_ex left_common_part = left->reduceCommonPart();
    std::unique_ptr<Fractal> left_frac(new Fractal(left_common_part * copy(left.get())));

    right_frac->separatePolynomialsDegree();
    left_frac->separatePolynomialsDegree();
    std::list<abs_ex>  res;

    auto takeMultipliersWithVariablesAwayToOtherFractal = [&res, &difur](fractal_argument * from_erase,
                                                        fractal_argument * to_insert, int var_take, int var_stay,
            bool is_from_erase_numerator)
    {
        for (auto it = from_erase->begin(); it != from_erase->end();)
        {
            if (it->get()->hasVariable(var_take) && !it->get()->hasVariable(var_stay))
            {
                if (is_from_erase_numerator)
                {
                    auto roots = solveEquation(*it, var_take);
                    for (auto &it1 : roots)
                        if (isRootOfEquation(copy(difur.get()), it1, var_take))
                            res.push_back(getVariableExpr(var_take) - std::move(it1));
                }
                to_insert->push_back(copy(it->get()));
                it = from_erase->erase(it);
            }
            else
                ++it;
        }
    };

    takeMultipliersWithVariablesAwayToOtherFractal(right_frac->getFractal().first, left_frac->getFractal().second,
                                                   y, x, true);
    takeMultipliersWithVariablesAwayToOtherFractal(right_frac->getFractal().second, left_frac->getFractal().first,
                                                   y, x, false);
    takeMultipliersWithVariablesAwayToOtherFractal(left_frac->getFractal().first, right_frac->getFractal().second,
                                                   x, y, true);
    takeMultipliersWithVariablesAwayToOtherFractal(left_frac->getFractal().second, left_frac->getFractal().first,
                                                   x, y, false);
    left_frac->simplify();
    right_frac->simplify();
    if (left_frac->hasVariable(x) || right_frac->hasVariable(y))
        return std::list<abs_ex> ();
    auto left_integr = integrate(toAbsEx(left_frac));
    auto right_integr = integrate(toAbsEx(right_frac));

    if (left_integr == nullptr || right_integr == nullptr)
        return std::list<abs_ex> ();
    auto ic = integratingConstantExpr();

    res.push_back(left_integr - right_integr + ic);
    return res;
}
std::list<abs_ex> solveDifurByMethods(const abs_ex & difur, int x, int y)
{
    auto dif_pol = toPolynomialPointer(difur);
    auto separable_vars_res = tryToSolveDifurWithSeparableVariables(dif_pol, x, y);
    if (separable_vars_res.size() > 0)
        return separable_vars_res;

    return std::list<abs_ex> ();
}
std::list<abs_ex>  solveDifurInCommonIntegral(const abs_ex & difur, int x, int y)
{
    std::list<abs_ex> res;
    if (isZero(difur))
    {
        res.push_back(copy(zero));
        return res;
    }
    if (difur->getId() == NUMBER || difur->getId() == CONSTANT)
        return std::list<abs_ex> ();
    if (difur->getId() == DIFFERENTIAL)
    {
        assert(difur->getSetOfVariables().size() == 1);
        int var = *difur->getSetOfVariables().begin();
        if (var != x && var != y)
        {
            res.push_back(copy(zero));
            return res;
        }
        res.push_back(getVariableExpr(var) - integratingConstantExpr());
        return res;
    }
    if (difur->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(difur.get())->getFractal();
        DifursRootConditions conditions;
        for (auto &it : *fr.second)
            conditions.addCondition(DifEquationRootCondition(it, x, y));
        for (auto &it : *fr.first)
            res.splice(res.end(), solveDifurInCommonIntegral(it, x, y, conditions));
        return res;
    }
    if (difur->getId() == DEGREE)
        return solveDifurInCommonIntegral(copy(Degree::getArgumentOfDegree(difur.get())), x, y,
                                          DifEquationRootCondition(Degree::getDegreeOfExpression(difur.get()),
                                                                   x, y));
    if (difur->getId() == ABSOLUTE_VALUE || difur->getId() == ARCTANGENT || difur->getId() == ARCSINUS)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur), x, y);
    if (difur->getId() == SINUS)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()*systemVarExpr(), x, y);
    if (difur->getId() == COSINUS)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()/two - getPi()*systemVarExpr(), x, y);
    if (difur->getId() == TANGENT)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()*systemVarExpr(), x, y);
    if (difur->getId() == COTANGENT)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - getPi()/two - getPi()*systemVarExpr(), x, y);
    if (difur->getId() == LOGARITHM)
        return solveDifurInCommonIntegral(getArgumentOfFunction(difur) - one, x, y);
    return solveDifurByMethods(difur, x, y);
}
std::list<abs_ex> solveDifurInCommonIntegral(const abs_ex & difur, int x, int y,
                                             const DifursRootConditions & conditions)
{
    auto preres = solveDifur(difur, x, y);
    std::list<abs_ex> res;
  //  qDebug() << difur->makeStringOfExpression();
    for (auto &it : preres)
    {

     //   qDebug() << it.toString();
        if (conditions.check(it))
            res.push_back(it.toCommonIntegral(x, y));
    }
    return res;
}
FunctionRange getRangeOfConstantAddictivesAndTakeThemAway(abs_ex & expr)
{
    if (isIntegratingConstant(expr->getId()) || expr->getSetOfVariables().empty())
    {
        auto res = expr->getRange();
        expr = copy(zero);
        return res;
    }
    expr = expr + zero;
    if (expr->getId() == POLYNOMIAL)
    {
        auto monoms = static_cast<Polynomial*>(expr.get())->getMonomialsPointers();

        if (isIntegratingConstant(monoms.back()->getId()))
        {
            auto res = monoms.back()->getRange();
            static_cast<Polynomial*>(expr.get())->getMonoms()->erase(--static_cast<Polynomial*>(expr.get())->getMonoms()->end());
            return res;

        }
        FunctionRange range;
        bool initialized = false;
        auto monoms_p = static_cast<Polynomial*>(expr.get())->getMonoms();
        for (auto it = monoms_p->begin(); it != monoms_p->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfSum(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = monoms_p->erase(it);
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRangeSegment(zero, zero, true, true);

    }
    return FunctionRangeSegment(zero, zero, true, true);
}
FunctionRange getRangeOfConstantMultipliersAndTakeThemAway(abs_ex & expr)
{
    if (isIntegratingConstant(expr->getId()) || expr->getSetOfVariables().empty())
    {
        auto res = expr->getRange();
        expr = copy(zero);
        return res;
    }
    if (expr->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(expr.get())->getFractal();
        if (isIntegratingConstant(fr.first->back()->getId()))
        {
            auto res = fr.first->back()->getRange();
            fr.first->erase(--fr.first->end());
            return res;
        }
        bool initialized = false;
        FunctionRange range;
        for (auto it = fr.first->begin(); it != fr.first->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfMultiplication(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = fr.first->erase(it);
            }
            else
                ++it;
        }
        for (auto it = fr.second->begin(); it != fr.second->end();)
        {
            if (it->get()->getSetOfVariables().empty())
            {
                if (initialized)
                    range = rangeOfDivision(range, it->get()->getRange());
                else
                {
                    range = it->get()->getRange();
                    initialized = true;
                }
                it = fr.second->erase(it);
            }
            else
                ++it;
        }
        if (initialized)
            return range;
        else
            return FunctionRange(one, one, true, true);
    }
    return FunctionRange(one, one, true, true);
}

void uniteSameResults(std::list<abs_ex> & list)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        for (auto it1 = next(it); it1 != list.end();)
        {
            auto sub = *it - *it1;
            if (isZero(sub))
            {
                it1 = list.erase(it1);
                continue;
            }
            if (isIntegratingConstant(sub->getId()))
            {
                FunctionRange range1 = getRangeOfConstantAddictivesAndTakeThemAway(*it);
                FunctionRange range2 = getRangeOfConstantAddictivesAndTakeThemAway(*it1);
                *it = *it + integratingConstantExpr(unification(range1, range2));
                it1 = list.erase(it1);
            }//они не могут быть одновременно нулями, это сверху раскрывается
            else if (isZero(*it) || isZero(*it1))
            {
                if (isZero(*it))
                    std::swap(*it, *it1);
                if (it->get()->getId() == FRACTAL && !static_cast<Fractal*>(it->get())->getFractal().first->empty() &&
                        isIntegratingConstant(static_cast<Fractal*>(it->get())->getFractal().first->back()->getId()))
                {
                    VariablesDefinition * def = VariablesDistributor::get().getVariablesDefinition(
                                static_cast<Variable*>(static_cast<Fractal*>(it->get())->getFractal().first->back().get())->getId());
                    auto range = def->getRange();
                    range.addSegmentWithoutNormilizing(zero, zero, true, true);
                    range.normalize();
                    def->setRange(range);
                    it1 = list.erase(it1);
                }
                else
                    ++it1;
            }
            else if (isIntegratingConstant((*it / *it1)->getId()))
            {
                FunctionRange range1 = getRangeOfConstantMultipliersAndTakeThemAway(*it);
                FunctionRange range2 = getRangeOfConstantMultipliersAndTakeThemAway(*it1);
                *it = *it * integratingConstantExpr(unification(range1, range2));
                it1 = list.erase(it1);
            }
            else
                ++it1;
        }
    }
}
std::list<DifurResult> solveDifur(const abs_ex &difur, int x, int y)
{
    auto res = solveDifurInCommonIntegral(difur, x, y);
    std::list<abs_ex> solved_for_y;
    std::list<abs_ex> solved_for_x;
    for (auto it = res.begin(); it != res.end();)
    {
        std::list<abs_ex> equation_res;
        bool for_y = false;
        if (has(it->get()->getSetOfVariables(), y))
        {
            equation_res = solveEquation(*it, y);
            for_y = true;
        }
        else if (has(it->get()->getSetOfVariables(), x))
            equation_res = solveEquation(*it, x);
        else
        {
            ++it;
            continue;
        }
        if (equation_res.size() > 0)
        {
            it = res.erase(it);
            if (for_y)
                solved_for_y.splice(solved_for_y.begin(), std::move(equation_res));
            else
                solved_for_x.splice(solved_for_x.begin(), std::move(equation_res));
        }
        else
            ++it;
    }
    uniteSameResults(res);
    uniteSameResults(solved_for_x);
    uniteSameResults(solved_for_y);
    std::list<DifurResult> result;
    for (auto &it : res)
        result.push_back(DifurResult(std::move(it), DifurResult::COMMON_INTEGRAL));
    for (auto &it : solved_for_x)
        result.push_back(DifurResult(std::move(it), DifurResult::SOLVED_FOR_X));
    for (auto &it : solved_for_y)
        result.push_back(DifurResult(std::move(it), DifurResult::SOLVED_FOR_Y));
    return result;
}

DifurResult::DifurResult(abs_ex &&expr, DifurResult::Type type) : result(std::move(expr)), type(type)
{

}

DifurResult::DifurResult(const DifurResult &res) : result(copy(res.result)), type(res.type)
{

}

DifurResult::DifurResult(DifurResult &&res) : result(std::move(res.result)), type(res.type)
{

}

QString DifurResult::toString() const
{
    QString res;
    if (this->type == COMMON_INTEGRAL)
        res = this->result->makeStringOfExpression() + "  = 0";
    else if (this->type == SOLVED_FOR_X)
        res = "x = " + this->result->makeStringOfExpression();
    else
        res = "y = " + this->result->makeStringOfExpression();
    auto set = this->result->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it))
            res += "  and  " + makeIntegratingConstantName(it) + QString::fromStdWString(L" is in  ")
                    + VariablesDistributor::getVariablesDefinition(it)->getRange().toString();
    return res;
}

const abs_ex &DifurResult::expr() const
{
    return this->result;
}

DifurResult::Type DifurResult::getType() const
{
    return this->type;
}

abs_ex DifurResult::toCommonIntegral(int x, int y)
{
    switch (this->type)
    {
    case SOLVED_FOR_X:
        return getVariableExpr(x) - this->expr();
    case SOLVED_FOR_Y:
        return getVariableExpr(y) - this->expr();
    case COMMON_INTEGRAL:
        return copy(this->expr());
    default:
        assert(false);
    }
    return nullptr;
}

abs_ex &DifurResult::expr()
{
    return this->result;
}
