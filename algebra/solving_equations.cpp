#include <vector>
#include "abstractexpression.h"
#include "number.h"
#include "some_algebra_expression_conversions.h"
//#include "fractal.h"
#include "polynomial.h"
#include "variablesdistributor.h"
#include "Matrix.h"
#include "solving_equations.h"
#include "algexpr.h"
#include "polynom.h"
#include "polynomials_factorization.h"
#include "sinus.h"
#include "cosinus.h"
#include "tangent.h"
#include "cotangent.h"
#include "logarithm.h"
#include "arctangent.h"
#include "arcsinus.h"
#include "constant.h"
#include "mathsets.h"
std::list<abs_ex> factorizePolynomOfOneVariable(const abs_ex & polynom, int var);
std::list<abs_ex> _solveEquation(const abs_ex & equation, int var);
bool isPolynomOfAllVariables(const abs_ex & equation)
{
    auto set = equation->getSetOfVariables();
    for (auto &it : set)
    {
        auto chp = checkIfItsPolynom(equation, it);
        if (chp.size() == 0 || chp[0] == nullptr)
            return false;
    }
    return true;
}
//возвратное уравнение, хз как правильно на english
std::list<abs_ex> checkIfitsReturnEquationAndTryToSolve(abs_ex && equation, int var)
{
    std::list<abs_ex> res;
    auto pol = checkIfItsPolynom(equation, var);
    liftAllIntegratingConstants(equation);

    //если размер массива четный, то степень уравнения нечетная, а тогда оно само должно разложиться в факторизации
    //ну и если это не полином, то тоже отсюда выйдет
    if (pol.size() % 2 == 0)
        return res;
    int center = pol.size() / 2;
    if (*pol[center - 1] == *zero || *pol[center + 1] == *zero)
        return res;

    abs_ex ratio = std::move(pol[center - 1])/std::move(pol[center + 1]);

    for (int i = 2; i <= pol.size()/2; ++i)
    {
        if (isZero(pol[center + i]) && !isZero(pol[center + i]))
            return res;
        if (!subCompare(pol[center - i]/pol[center + i], pow(ratio, i)))
            return res;

    }
    abs_ex new_equation = std::move(pol[center]);
    abs_ex temp_var = systemVarExpr();
    //построение выражений вида x^n + a^n/x^n, выраженных через t, где t = x + a/x. индекс == n, нулевой индекс не рабочий
    std::vector<abs_ex> coefs_expressions(pol.size() / 2 + 1);
    coefs_expressions[1] = copy(temp_var);
    for (int i = 2; i <= pol.size() / 2; ++i)
    {

        abs_ex sub = copy(zero);
        auto coefs = MathSets::getBinomialCoefs(i);
        for (int j = 1; j < coefs.size()/2; ++j)
        {
            sub = std::move(sub) + numToAbs(coefs[j])*coefs_expressions[coefs.size() - 2*j - 1];
        }
        if (coefs.size() % 2 != 0)
            sub = std::move(sub) + numToAbs(coefs.size() / 2)*pow(ratio, coefs.size()/2);
        coefs_expressions[i] = pow(temp_var, i) - std::move(sub);
    }
    for (int i = 2; i <= pol.size()/2; ++i)
    {
        if (!isZero(pol[center + i]))
        {

            new_equation = std::move(new_equation) + std::move(pol[center + i])*coefs_expressions[i];
        }
    }
    res = solveEquation(equation, var);
    abs_ex t_expr = getVariableExpr(var) + ratio/getVariableExpr(var);
    std::list<abs_ex> actual_res;
    for (auto &it : res)
    {
        setUpExpressionIntoVariable(it, t_expr, temp_var->getId());
        actual_res.splice(actual_res.end(), solveEquation(it, var));
    }
    return res;

}
//имеются ввиду уравнения по типу 2^2x + 2^x + 2 == a
std::list<abs_ex> checkIfItsExponentialSumEquationAndTryToSolve(abs_ex && equation, abs_ex && right, int var)
{
  //  qDebug() << equation->toString();
    std::list<abs_ex> res;
    auto monoms = static_cast<Polynomial*>(equation.get())->getMonomialsPointers();

    auto var_degree = monoms.begin()._Ptr->_Myval->getPartWithVariable(var);
    abs_ex temp_var = systemVarExpr();
    abs_ex new_equation = monoms.begin()._Ptr->_Myval->getFractalWithoutVariable(var) * copy(temp_var);

    auto downcasted_frac = monoms.begin()._Ptr->_Myval->downcast();
    auto basis = copy(Degree::getArgumentOfDegree(var_degree.get()));
    if (basis->hasVariable(var))
        return res;
    if (basis->getPositionRelativelyZero() < 0)
    {
        downcasted_frac = -std::move(downcasted_frac);
        basis = -std::move(basis);
        var_degree = -std::move(var_degree);
        equation = -std::move(equation);
        right = -std::move(right);
    }
    auto deg = Degree::getDegreeOfExpression(var_degree.get());

    for (auto it = next(monoms.begin()); it != monoms.end(); ++it)
    {
        auto monom = it._Ptr->_Myval->getPartWithVariable(var);
        if (!subCompare(copy(Degree::getArgumentOfDegree(monom.get())), basis))
            return res;
        if ((Degree::getDegreeOfExpression(monom.get())/deg )->getId() != NUMBER)
            return res;
        new_equation = std::move(new_equation) +
                it._Ptr->_Myval->getFractalWithoutVariable(var) * pow(temp_var, (Degree::getDegreeOfExpression(monom.get())/deg ));
    }
    new_equation = std::move(new_equation) - std::move(right);
    auto temp_res = solveEquation(new_equation, temp_var->getId());
    //qDebug() << var_degree->makeStringOfExpression();
    for (auto &it : temp_res)
    {
        //qDebug() << it->makeStringOfExpression();
        if (it->getPositionRelativelyZero() >= 0)
        {
            res.splice(res.end(), solveEquation(var_degree - it, var));
        }
    }
    return res;

}
bool hasRadicalMultiplier(abs_ex & expr)
{
    auto isRadical = [](abs_ex & expr)->bool {
        if (expr->getId() == DEGREE)
        {
            auto deg = Degree::getDegreeOfExpression(expr.get());
            if (deg->getId() == NUMBER)
                if (static_cast<Number*>(deg.get())->getDenominator() > 1)
                    return true;
            if (deg->getId() == FRACTAL)
                if (static_cast<Fractal*>(deg.get())->getCoefficient().getDenominator() > 1 ||
                        !static_cast<Fractal*>(deg.get())->getFractal().second->empty())
                    return true;
            return false;
        }
        return false;
    };
    if (expr->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(expr.get())->getFractal();
        for (auto &it : *fr.first)
            if (isRadical(it))
                return true;
        for (auto &it : *fr.second)
            if (isRadical(it))
                return true;
        return false;
    }
    return isRadical(expr);

}
std::list<abs_ex> checkIfItsLogarithmicSumEquationAndTryToSolve(const abs_ex & equation, const abs_ex & right, int var)
{
   // qDebug() << equation->toString();
    std::list<abs_ex> res;
    auto monoms = static_cast<Polynomial*>(equation.get())->getMonomialsPointers();
    abs_ex new_equation = copy(one);
    EquationRootsConditions conditions;
    for (auto &it : monoms)
    {
        auto fr_without_var = toAbsEx(it->getFractalWithoutVariable(var));
        auto fr_with_var = copy(it)/fr_without_var;
     //   qDebug() << it->toString();
       // qDebug() << fr_without_var->toString();
      //  qDebug() << fr_with_var->toString();
        if (fr_with_var->getId() != LOGARITHM)
            return res;
        //думаю, этот иф бессмысленен, однако я не совсем уверен с тем, как степени там даункастятся
        if (it->getCoefficient().compareWith(0) > 0)
            new_equation = std::move(new_equation) * pow(getArgumentOfFunction(fr_with_var), fr_without_var);
        else
            new_equation = std::move(new_equation) / pow(getArgumentOfFunction(fr_with_var), -fr_without_var);
        conditions.addCondition(RootCondition(var, RootCondition::BIGGER_THAN_ZERO, getArgumentOfFunction(fr_with_var)));
        conditions.addCondition(RootCondition(var, RootCondition::DONT_EQUAL_ZERO, getArgumentOfFunction(fr_with_var)));
    }
   // qDebug() << right->toString();
   // qDebug() << pow(getEuler(), licCopy(right))->toString();
    //qDebug() << (new_equation - pow(getEuler(), licCopy(right)))->toString();
    //qDebug() << equation->toString();
   // qDebug() << right->toString();
   // qDebug() << new_equation->toString();
    //qDebug() << pow(getEuler(), licCopy(right))->toString();
   // qDebug() << (new_equation - pow(getEuler(), licCopy(right)))->toString();
    return solveEquation(new_equation - pow(getEuler(), licCopy(right)), var, conditions);

}
//вещи по типу sqrt(x) = a, или sqrt(x) = f(x)
std::list<abs_ex> solveEquationOfSpecialCases(const abs_ex & var_expr, abs_ex && right_expr, int var)
{
    std::list<abs_ex> res;
    if (var_expr->getId() == DEGREE)
    {

        auto arg = Degree::getArgumentOfDegree(var_expr.get());
        auto deg = Degree::getDegreeOfExpression(var_expr.get());
    //    qDebug() << var_expr->makeStringOfExpression();
      //  qDebug() << right_expr->makeStringOfExpression();
        if (arg->hasVariable(var) && !deg->hasVariable(var)) //условия
        {
            if (deg->getId() == NUMBER || deg->getId() == FRACTAL)
            {
                Number num;
                if (deg->getId() == NUMBER)
                    num = *static_cast<Number*>(deg.get());
                else
                    num = static_cast<Fractal*>(deg.get())->getCoefficient();
                if (num.getNumerator() % 2 == 0) //(f(x))^2 == g  <=> [f(x)=sqrt(g), f(x) =-sqrt(g)]
                {
                    if (right_expr->getPositionRelativelyZero() < 0)
                        return res;
                 //   qDebug() << right_expr->toString();
                 //   qDebug() << deg->toString();
                    auto right_pow = pow(licCopy(right_expr), one/deg);
                    //если с правой стороны появляется радикал, то такое преобразование
                    //бессмысленно, ибо потом его опять будем возводить в степень
                    //и в результате упадем в бесконечную рекурсию
                    if (hasRadicalMultiplier(right_pow))
                        return std::list<abs_ex>();
                    res.splice(res.end(), solveEquation(copy(arg) - licCopy(right_pow), var,
                                                        RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)));
                    res.splice(res.end(), solveEquation(copy(arg) + std::move(right_pow), var,
                                                        RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)));
                    return res;
                }
                if (num.getDenominator() % 2 == 0)
                {
                    if (right_expr->getPositionRelativelyZero() < 0)
                        return res;
                    auto right_pow = pow(licCopy(right_expr), one/deg);
                    if (hasRadicalMultiplier(right_pow))
                        return std::list<abs_ex>();
                    return solveEquation(copy(arg) - std::move(right_pow), var,
                    { RootCondition(var, RootCondition::BIGGER_THAN_ZERO, copy(arg)),
                      RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)});
                }

            }
            auto right_pow = pow(std::move(right_expr), one/deg);
            if (hasRadicalMultiplier(right_pow))
                return std::list<abs_ex>();
            return solveEquation(copy(arg) - std::move(right_pow), var);
        }
        if (!arg->hasVariable(var) && deg->hasVariable(var))
        {
            if (right_expr->getPositionRelativelyZero() < 0)
                return res;
            //иначе можем упасть в бесконечный цикл возведения в в степень
            if (Degree::getArgumentOfDegree(right_expr.get())->hasVariable(var))
                return res;
           // qDebug() << (deg - ln(right_expr)/ln(copy(arg)))->toString();

            return solveEquation(deg - ln(std::move(right_expr))/ln(copy(arg)), var);
        }
        return res;
    }
    if (var_expr->getId() == ABSOLUTE_VALUE)
    {
        if (right_expr->getPositionRelativelyZero() < 0)
            return res;
        auto arg = copy(static_cast<AbsoluteValue*>(var_expr.get())->getExpression());
      //  qDebug() << right_expr->toString();
      //  qDebug() << amountOfIntegratingConstant(6);
        res.splice(res.end(), solveEquation(arg - copyWithLiftingIntegrationConstantsThatCanBeChanged(right_expr), var,
        {RootCondition(var, RootCondition::BIGGER_THAN_ZERO, arg),
         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, copyWithLiftingIntegrationConstantsThatCanBeChanged(right_expr))}));
     //   qDebug() << AlgExpr(res.back()).toString();
        res.splice(res.end(), solveEquation(arg + licCopy(right_expr), var,
        {RootCondition(var, RootCondition::LESS_THAN_ZERO, arg),
         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, copyWithLiftingIntegrationConstantsThatCanBeChanged(right_expr))}));
       // qDebug() << AlgExpr(res.back()).toString();
        return res;
    }
    if (var_expr->getId() == SINUS)
    {
        if ((right_expr - one)->getPositionRelativelyZero() > 0)
            return res;
        if ((right_expr + one)->getPositionRelativelyZero() < 0)
            return res;
        if (right_expr->hasVariable(var))
        {
            //такие особые условия приходится делать из-за поведения аркфункций, которые не раскроют уравнение, потому
            //что оно в общем случае не попадает в границы области значений арков
            if (right_expr->getId() == SINUS)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                res.splice(res.end(), solveEquation(var_arg - licCopy(right_arg) - two*getPi()*systemVarExpr(), var));
                res.splice(res.end(), solveEquation(var_arg - getPi() + licCopy(right_arg) - two*getPi()*systemVarExpr(), var));
                return res;
            }
            /*if (right_expr->getId() == COSINUS)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(right_expr);
                auto int_var = systemVarExpr();
                res.splice(res.end(), solveEquation(var_arg - right_arg - two*getPi()*int_var, var,
                                                    RootCondition(var, RootCondition::EQUAL_TWO_PI_INTEGER, var_arg - getPi()/four)));
                res.splice(res.end(), solveEquation(var_arg - right_arg - two*getPi()*int_var, var,
                                                    RootCondition(var, RootCondition::EQUAL_TWO_PI_INTEGER, var_arg + getPi()/four)));

                return res;
            }*/
            //тангенсы и котангенсы должны были преобразоваться
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
        auto right_asin = asin(std::move(right_expr));
        res.splice(res.end(), solveEquation(var_arg - licCopy(right_asin) - two*getPi()*systemVarExpr(), var));
        res.splice(res.end(), solveEquation(var_arg - getPi() + std::move(right_asin) - two*getPi()*systemVarExpr(), var));
        return res;

    }
    if (var_expr->getId() == COSINUS)
    {
        if ((right_expr - one)->getPositionRelativelyZero() > 0)
            return res;
        if ((right_expr + one)->getPositionRelativelyZero() < 0)
            return res;
        if (right_expr->hasVariable(var))
        {
           // if (right_expr->getId() == SINUS)
           //     return solveEquationOfSpecialCases(right_expr, var_expr, var);
            if (right_expr->getId() == COSINUS)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                res.splice(res.end(), solveEquation(var_arg - licCopy(right_arg) - two*getPi()*systemVarExpr(), var));
                res.splice(res.end(), solveEquation(var_arg + licCopy(right_arg) - two*getPi()*systemVarExpr(), var));
                return res;
            }
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
        auto right_acos = acos(std::move(right_expr));
        res.splice(res.end(), solveEquation(var_arg - licCopy(right_acos) - two*getPi()*systemVarExpr(), var));
        res.splice(res.end(), solveEquation(var_arg + licCopy(right_acos) - two*getPi()*systemVarExpr(), var));
        return res;
    }
    if (var_expr->getId() == TANGENT)
    {
        if (right_expr->hasVariable(var))
        {
            if (right_expr->getId() == TANGENT)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                return solveEquation(var_arg - licCopy(right_arg) - getPi()*systemVarExpr(), var);
            }
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
        auto right_atan = atan(std::move(right_expr));
        return solveEquation(var_arg - std::move(right_atan) - getPi()*systemVarExpr(), var);
    }
    if (var_expr->getId() == COTANGENT)
    {
        if (right_expr->hasVariable(var))
        {
            if (right_expr->getId() == COTANGENT)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                return solveEquation(var_arg - std::move(right_arg) - getPi()*systemVarExpr(), var);
            }
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
        auto right_acot = acot(std::move(right_expr));
        return solveEquation(var_arg - std::move(right_acot) - getPi()*systemVarExpr(), var);
    }
    if (var_expr->getId() == LOGARITHM)
    {
        if (right_expr->hasVariable(var))
        {
            //вот так мы должны поставить условие потому что иначе будем туда сюда возводить в степень и логарифмировать
            if (right_expr->getId() == LOGARITHM)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                return solveEquation(var_arg - licCopy(right_arg), var, {
                                         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, var_arg),
                                         RootCondition(var, RootCondition::DONT_EQUAL_ZERO, var_arg),
                                         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_arg),
                                         RootCondition(var, RootCondition::DONT_EQUAL_ZERO, right_arg)
                                     });
            }
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
       // qDebug() << AlgExpr(right_expr).toString();
       // qDebug() << VariablesDistributor::amountOfVariable(1500000006);
    //    qDebug() << AlgExpr(pow(getEuler(), copyWithLiftingIntegrationConstantsThatCanBeChanged(right_expr))).toString();
       // qDebug() << amountOfIntegratingConstant(10);
        return solveEquation(var_arg - pow(getEuler(), std::move(right_expr)), var, {
                                 RootCondition(var, RootCondition::BIGGER_THAN_ZERO, var_arg),
                                 RootCondition(var, RootCondition::DONT_EQUAL_ZERO, var_arg)
                             });
    }
    if (var_expr->getId() == ARCSINUS)
    {
        if (right_expr->hasVariable(var))
        {
            if (right_expr->getId() == ARCSINUS)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                return solveEquation(var_arg - std::move(right_arg), var, {
                                         RootCondition(var, RootCondition::LESS_THAN_ZERO, var_arg - one),
                                         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, var_arg + one),
                                         RootCondition(var, RootCondition::LESS_THAN_ZERO, var_arg - one),
                                         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, var_arg + one)
                                     });
            }
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
        abs_ex right_asin = sin(std::move(right_expr));
        return solveEquation(var_arg - std::move(right_asin), var, {
                                 RootCondition(var, RootCondition::LESS_THAN_ZERO, var_arg - one),
                                 RootCondition(var, RootCondition::BIGGER_THAN_ZERO, var_arg + one),
                                 RootCondition(var, RootCondition::LESS_THAN_ZERO, licCopy(right_expr) - getPi()/two),
                                 RootCondition(var, RootCondition::BIGGER_THAN_ZERO, licCopy(right_expr) + getPi()/two)
                             });
    }
    if (right_expr->getId() == ARCTANGENT)
    {
        if (right_expr->hasVariable(var))
        {
            if (right_expr->getId() == ARCTANGENT)
            {
                auto var_arg = getArgumentOfFunction(var_expr);
                auto right_arg = getArgumentOfFunction(std::move(right_expr));
                return solveEquation(var_arg - std::move(right_arg), var);
            }
            return res;
        }
        auto var_arg = getArgumentOfFunction(var_expr);
        abs_ex right_atan = tan(std::move(right_expr));
        return solveEquation(var_arg - std::move(right_atan), var, {
                                 RootCondition(var, RootCondition::LESS_THAN_ZERO, licCopy(right_expr) - getPi()/two),
                                 RootCondition(var, RootCondition::BIGGER_THAN_ZERO, licCopy(right_expr) + getPi()/two)
                             });
    }
    return res;

}
std::list<abs_ex> checkIfItsMultiplicationOfDegreesAndTryToSolve(const abs_ex & var_part, const abs_ex & right_part, int var)
{
    assert(var_part->getId() == FRACTAL);
   // qDebug() << var_part->makeStringOfExpression();
  //  qDebug() << right_part->makeStringOfExpression();
    Fractal * fr = static_cast<Fractal*>(var_part.get());
    if (!(fr->getCoefficient().isOne() && fr->getFractal().second->empty()))
        return std::list<abs_ex>();
    assert(fr->getCoefficient().isOne() && fr->getFractal().second->empty());
    if (Degree::getDegreeOfExpression(fr->getFractal().first->begin()->get())->getId() != NUMBER)
        return std::list<abs_ex>();
    long long int lcm_of_denominators = static_cast<Number*>(Degree::getDegreeOfExpression(fr->getFractal().first->begin()->get()).get())
            ->getDenominator();
    for (auto it = next(fr->getFractal().first->begin()); it != fr->getFractal().first->end(); ++it)
    {
        if (Degree::getDegreeOfExpression(it->get())->getId() != NUMBER)
            return std::list<abs_ex>();
        lcm_of_denominators = lcm(lcm_of_denominators, static_cast<Number*>(Degree::getDegreeOfExpression(it->get()).get())
                                  ->getDenominator());
    }
    if (lcm_of_denominators == 1)
        return std::list<abs_ex>();
    return solveEquation(pow(var_part, lcm_of_denominators) - licCopy(right_part), var);
}
std::list<abs_ex> solveEquationOfPolynom(const std::unique_ptr<Polynomial> & equation, int var)
{
    std::list<abs_ex> res;
    res = checkIfitsReturnEquationAndTryToSolve(licCopy(equation.get()), var);
    if (res.size() > 0)
        return res;

    abs_ex var_expr = copy(zero);
    abs_ex right_expr = copy(zero);

    abs_ex equation_copy_expr = copy(equation.get());
    liftAllIntegratingConstants(equation_copy_expr);
    auto equation_copy = toPolynomialPointer(std::move(equation_copy_expr));

    auto monoms = equation_copy->getMonomialsPointers();
    //qDebug() << equation->makeStringOfExpression();
    //qDebug() << equation->toString();
    for (auto &it : monoms)
    {
        if (it->hasVariable(var))
            var_expr = std::move(var_expr) + toAbsEx(std::move(it));
        else
            right_expr = std::move(right_expr) - toAbsEx(std::move(it));
    }
    equation_copy.release();
    if (var_expr->getId() == POLYNOMIAL)
    {
        auto common_part = static_cast<Polynomial*>(var_expr.get())->reduceCommonPart();
        var_expr = std::move(common_part) * std::move(var_expr);
    }
    //qDebug() << equation->makeStringOfExpression();
   // qDebug() << var_expr->makeStringOfExpression();
   // qDebug() << right_expr->makeStringOfExpression();
    if (var_expr->getId() == FRACTAL)
    {
     //   qDebug() << var_expr->toString();
        auto mult = static_cast<Fractal*>(var_expr.get())->getFractalWithoutVariable(var);
       // qDebug() << mult->toString();
        var_expr = std::move(var_expr) / toAbsEx(mult);
        right_expr = std::move(right_expr) / toAbsEx(mult);
       // qDebug() << var_expr->toString();
        if (var_expr->getId() == FRACTAL)
        {
            res = checkIfItsMultiplicationOfDegreesAndTryToSolve(var_expr, right_expr, var);
            if (res.size() > 0)
                return res;
        }
    }
   // qDebug() << var_expr->makeStringOfExpression();
   // qDebug() << right_expr->makeStringOfExpression();
    //qDebug() << var_expr->makeStringOfExpression();;
    var_expr = var_expr + zero;
    if (var_expr->getId() == var)
    {
        res.push_back(std::move(right_expr));
        return res;
    }
    if (var_expr->getId() == POLYNOMIAL)
    {
        //если это все еще полином, то это либо уравнение общего вида,
        //либо уравнение по типу sqrt(x) = x + 3, которое будет разобрано после, либо
        //уравнение общего вида, которое хер пойми как решать, либо
        //уравнение по типу 2^2x + 2^x + 2 = a, либо сумма логарифмов, либо тригонометрическое уравнение,
        //либо полином
        //qDebug() << var_expr->toString();
        res = checkIfItsExponentialSumEquationAndTryToSolve(copy(var_expr), licCopy(right_expr), var);
        if (res.size() > 0)
            return res;
        res = checkIfItsLogarithmicSumEquationAndTryToSolve(var_expr, right_expr, var);
        if (res.size() > 0)
            return res;
        if (isPolynomOfAllVariables(var_expr))
        {
            auto deg = static_cast<Polynomial*>(var_expr.get())->tryToDistinguishFullDegree();
            if (deg != nullptr)
                var_expr = std::move(deg);
        }
    }

    res = solveEquationOfSpecialCases(var_expr, licCopy(right_expr), var);
    if (res.size() > 0)
        return res;
    var_expr = copy(zero);

    equation_copy_expr = copy(equation.get());
    liftAllIntegratingConstants(equation_copy_expr);
    equation_copy = toPolynomialPointer(std::move(equation_copy_expr));
    right_expr = copy(equation_copy.get());
    monoms = equation_copy->getMonomialsPointers();
  //  qDebug() << equation_copy->toString();
    for (auto &it : monoms)
    {
        auto var_monom = copy(it)/it->getFractalWithoutVariable(var);
        if (var_monom->getId() <= DEGREE)
        {
            var_expr = copy(it)->downcast();
            right_expr = right_expr - copy(it);

            break;
        }
    }
    equation_copy.release();
    right_expr = -right_expr;
    if (var_expr->getId() == FRACTAL)
    {
        auto expr_without_var = toAbsEx(static_cast<Fractal*>(var_expr.get())->getFractalWithoutVariable(var));
        var_expr = var_expr / expr_without_var;
        right_expr = right_expr / expr_without_var;
    }
    //qDebug() << var_expr->toString();
    //qDebug() << right_expr->toString();
    res = solveEquationOfSpecialCases(var_expr, std::move(right_expr), var);


    return res;

}
std::list<abs_ex> _solveEquation(const abs_ex & equation, int var)
{
  //  qDebug() << equation->toString();

    std::list<abs_ex> res;
    if (equation->getId() == NUMBER)
    {
        if (static_cast<Number*>(equation.get())->isZero())
            res.push_back(integratingConstantExpr());
        return res;
    }
    if (!equation->hasVariable(var))
        return res;
    if (equation->getId() == var)
    {
        res.push_back(copy(zero));
        return res;
    }
    if (equation->getId() > 0)
        return res;
    if (equation->getId() == DEGREE)
    {
        return solveEquation(copy(Degree::getArgumentOfDegree(equation.get())), var,
                              RootCondition(var, RootCondition::DONT_EQUAL_ZERO, Degree::getDegreeOfExpression(equation.get())));
    }
    if (equation->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(equation.get())->getFractal();
        EquationRootsConditions conditions;
        for (auto &it : *fr.second)
            conditions.addCondition(RootCondition(var, RootCondition::DONT_EQUAL_ZERO, it));
       // qDebug() << equation->toString();
       // qDebug() << VariablesDistributor::amountOfVariable(1500000004);
        for (auto &it : *fr.first)
            res.splice(res.end(), solveEquation(copyWithLiftingIntegrationConstantsThatCanBeChanged(it), var, conditions));
        return res;

    }
    if (equation->getId() == ABSOLUTE_VALUE)
        return _solveEquation(copy(static_cast<AbsoluteValue*>(equation.get())->getExpression()), var);
    if (equation->getId() == SINUS)
        return _solveEquation(static_cast<Sinus*>(equation.get())->getArgumentsCopy() - getPi()*systemVarExpr(), var);
    if (equation->getId() == COSINUS)
        return _solveEquation(static_cast<Cosinus*>(equation.get())->getArgumentsCopy() - getPi()/two - getPi()*systemVarExpr(), var);
    if (equation->getId() == TANGENT)
        return _solveEquation(static_cast<Tangent*>(equation.get())->getArgumentsCopy() - getPi()*systemVarExpr(), var);
    if (equation->getId() == COTANGENT)
        return _solveEquation(static_cast<Cotangent*>(equation.get())->getArgumentsCopy() - getPi()*systemVarExpr(), var);
    if (equation->getId() == LOGARITHM)
        return _solveEquation(static_cast<Logarithm*>(equation.get())->getArgumentsCopy() - one, var);
    if (equation->getId() == CONSTANT)
        return res;
    if (equation->getId() == ARCTANGENT)
        return _solveEquation(static_cast<ArcTangent*>(equation.get())->getArgumentsCopy(), var);
    if (equation->getId() == ARCSINUS)
        return _solveEquation(static_cast<ArcSinus*>(equation.get())->getArgumentsCopy(), var);

    //is linear equation
    auto ln_f = checkIfItsLinearFunction(equation, var);
    if (ln_f.first != nullptr)
    {
        res.push_back( -ln_f.second / ln_f.first);
        return res;
    }
    //is quadratic equation
    auto qc_f = checkIfItsQuadraticFunction(equation, var);
    if (qc_f[0] != nullptr)
    {
        //qDebug() << equation->toString();
        auto D = sqr(qc_f[1]) - four*qc_f[0]*qc_f[2];
        if (*D == *zero)
        {
            res.push_back( -qc_f[1]/two/qc_f[0]);
            return res;
        }
        if (D->getPositionRelativelyZero() < 0)
        {
            return res;
        }
        auto root_D = sqrt(D);
        if (root_D->getId() == ABSOLUTE_VALUE)
            root_D = static_cast<AbsoluteValue*>(root_D.get())->getExpressionCopy();
        res.push_back( (-qc_f[1] + root_D)/(two*qc_f[0]));
        res.push_back( (-qc_f[1] - root_D)/(two*qc_f[0]));
        return res;
    }
    if (isPolynomOfAllVariables(equation))
    {
        auto facts = factorizePolynom(equation);
        if (facts.first.size() > 1 || (facts.first.size() == 1 && facts.first.begin()->get()->getId() == DEGREE))
        {
            for (auto &it : facts.first)
            {
                res.splice(res.end(), _solveEquation(copy(Degree::getArgumentOfDegree(it.get())), var));
            }
        }
        if (res.size() > 0)
            return res;
        auto full_deg_dist_res = tryToDistingushFullDegreeWithPrecisionOfCoefficientWithoutVariable(equation, var);
        if (full_deg_dist_res.first != nullptr)
        {
            abs_ex remaining_part = -(equation - pow(full_deg_dist_res.first, full_deg_dist_res.second));
            //qDebug() << full_deg_dist_res.first->makeStringOfExpression();
            //qDebug() << remaining_part->makeStringOfExpression();
            if (full_deg_dist_res.second % 2 == 0 && remaining_part->getPositionRelativelyZero() < 0)
                return res;
            res.splice(res.end(), solveEquation(full_deg_dist_res.first + pow(remaining_part, one/numToAbs(full_deg_dist_res.second)), var));
            if (full_deg_dist_res.second % 2 == 0)
                res.splice(res.end(), solveEquation(full_deg_dist_res.first - pow(remaining_part, one/numToAbs(full_deg_dist_res.second)), var));
            return res;
        }
    }
    if (equation->getId() == POLYNOMIAL)
    {
        auto common_part = static_cast<Polynomial*>(equation.get())->reduceCommonPart();
        //qDebug() << common_part->makeStringOfExpression();
        //qDebug() << equation->makeStringOfExpression();
        if (common_part != nullptr && *common_part != *one)
        {
            res.splice(res.end(), _solveEquation(common_part, var));
            res.splice(res.end(), _solveEquation(equation, var));
            return res;
        }
        //qDebug() << VariablesDistributor::amountOfVariable(1500000003);
        //equation->setSimplified(false);
       // equation->simplify();
     //   qDebug() << equation->toString();
      //  qDebug() << VariablesDistributor::amountOfVariable(1500000004);
        return solveEquationOfPolynom(toPolynomialPointer(copyWithLiftingIntegrationConstantsThatCanBeChanged(equation)), var);
    }
        //return { -ln_f.second / ln_f.first};
    return res;
}
std::list<abs_ex > solveEquation(const abs_ex &equation, int var)
{
    //при наличии синусов и косинусов, тангенс и котангенс любых аргументов должны раскрыться, а не как сейчас
    //также и котангенс при наличии тангенса

    int lcm_of_dens = equation->getLcmOfDenominatorsOfDegreesOfVariable(var);
    if (lcm_of_dens > 1)
    {
        abs_ex temp_var;
        if (lcm_of_dens % 2 == 0)
            temp_var = abs_ex(new Variable(systemVar(0, std::numeric_limits<int>::max())));
        else
            temp_var = systemVarExpr();
       abs_ex repl_equation = copy(equation);
       //сделать условие что t > 0 если lcm_of_dens % 2 == 0
        setUpExpressionIntoVariable(repl_equation, pow(temp_var, lcm_of_dens), var);
        auto res = _solveEquation(repl_equation, temp_var->getId());
        std::list<abs_ex> actual_res;
        for (auto &it : res)
            if (lcm_of_dens % 2 != 0 || it->getPositionRelativelyZero() >= 0)
                actual_res.push_back( pow(it, lcm_of_dens));
        return actual_res;

    }
    int gcd_of_nums = equation->getGcdOfNumeratorsOfDegrees(var);
    if (gcd_of_nums > 1)
    {
        abs_ex temp_var;
        if (gcd_of_nums % 2 == 0)
            temp_var = abs_ex(new Variable(systemVar(0, std::numeric_limits<int>::max())));
        else
            temp_var = systemVarExpr();
        abs_ex repl_equation = copy(equation);
        setUpExpressionIntoVariable(repl_equation, pow(temp_var, Number(1, gcd_of_nums)), var);
        //аналогично условие t > 0 если gcd_of_nums % 2 == 0
        auto res = _solveEquation(repl_equation, temp_var->getId());
        if (gcd_of_nums % 2 == 0)
        {
            std::list<abs_ex> actual_res;
            for (auto &it : res)
                if (it->getPositionRelativelyZero() >= 0)
                {
                    actual_res.push_back(pow(it, one/numToAbs(gcd_of_nums)));
                    actual_res.push_back(-pow(it, one/numToAbs(gcd_of_nums)));
                }
            return actual_res;
        }
        for (auto &it : res)
            it = pow(it, one/numToAbs(gcd_of_nums));
        return res;
    }
  //  qDebug() << AlgExpr(equation).toString();
    return _solveEquation(equation, var);
}

















//возвращает коэффициент и индекс переменной в векторе, к которой этот коэффициент принадлежит. Если там есть другие переменные из вектора или оно нелинейно относительно
//какой-л переменной то возвразщает {nullptr, }
//индекс vars.size() означает что это свободный член
std::pair<abs_ex, int> getLinearCoeffientOfVariable(AbstractExpression * monom, const std::vector<int> & vars)
{
    assert(monom->getId() != POLYNOMIAL);
    int variable_id_in_monom = -1;
    auto set_of_vars = monom->getSetOfVariables();
    for (int i = 0; i < vars.size(); ++i)
    {
        if (set_of_vars.find(vars[i]) != set_of_vars.end())
        {
            if (variable_id_in_monom == -1)
                variable_id_in_monom = i;
            else
                variable_id_in_monom = -2;
        }
    }
    if (variable_id_in_monom == -2)
        return {nullptr, -1};
    if (variable_id_in_monom == -1)
        return {copy(monom), vars.size()};
    auto ln_f = checkIfItsLinearFunction(monom, vars[variable_id_in_monom]);
    return {std::move(ln_f.first), variable_id_in_monom};

}
//последний коэффициент - свободный член. result[0] == nullptr значит что уравнение не линейно относительно этого набора переменных
std::vector<abs_ex> checkIfEquationIsLinearAnsGetCoefficients(const abs_ex & in_equation, const std::vector<int> & vars)
{
    std::vector<abs_ex> result(vars.size() + 1);
    for (auto &it : result)
        it = copy(zero);
    abs_ex equation;
    if (in_equation->getId() == FRACTAL &&
             static_cast<Fractal*>(in_equation.get())->canTurnIntoPolynomWithOpeningParentheses(true))
        equation = toAbsEx(static_cast<Fractal*>(in_equation.get())->turnIntoPolynomWithOpeningParentheses(true).release());
    else
        equation = copy(in_equation);
    if (equation->getId() == POLYNOMIAL)
    {
        auto pol = static_cast<Polynomial*>(equation.get())->getMonomialsPointers();
        for (auto &it : pol)
        {
            auto coefficient = getLinearCoeffientOfVariable(it, vars);
            if (coefficient.first == nullptr)
            {
                result[0] = nullptr;
                return result;
            }
            result[coefficient.second] = std::move(result[coefficient.second]) + std::move(coefficient.first);
        }
        return result;
    }
    auto coefficient = getLinearCoeffientOfVariable(equation.get(), vars);
    if (coefficient.first == nullptr)
    {
        result[0] = nullptr;
        return result;
    }
    result[coefficient.second] =  std::move(coefficient.first);
    return result;
}
//возвращает корни в том же порядке, что и vars. если система нелинейна, то возвращает { , -2}. Если система линейна, но не имеет решений, то возвращает { , -1}
//если система имеет единственное решение, то возвращает { , 0}. если бесконечное множество решений, вернет { , > 0},
//теоретически здесь я бы мог продумать вариант с параметром, но для текущих целей - метода неопределенных
//коэффициентов, это не нужно.
std::pair<std::vector<abs_ex>, int> checkIfSystemOfEquationsLinearAndTryToSolveIt(const std::vector<abs_ex> & system, const std::vector<int>& vars)
{
    Matrix<AlgExpr> system_matrix(system.size(), vars.size() + 1);
    for (int i = 0; i < system.size(); ++i)
    {
        auto coefs = checkIfEquationIsLinearAnsGetCoefficients(system[i], vars);
        if (coefs[0] == nullptr)
            return {std::vector<abs_ex>(0), -2};
        coefs.back() = -coefs.back();
        std::vector<AlgExpr> algexpr_coefs(coefs.size());
        for (int i = 0; i < coefs.size(); ++i)
            algexpr_coefs[i] = AlgExpr(std::move(coefs[i]));
        system_matrix[i] = std::move(algexpr_coefs);
    }
    auto result = gauss(std::move(system_matrix));
    if (result.size() == 0)
        return {std::vector<abs_ex>(0), -2};

    //при решении линейных уравнений в результате может быть пареметр, однако сейчас мы упрощаем и рассматриваем
    //без него
    std::vector<abs_ex> result_vector(result.size());
    for (int i = 0; i < result.size(); ++i)
        result_vector[i] = std::move(result[i].back().getExpr());
    return {std::move(result_vector), 0};
}
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars)
{
    //возможность решать это с параметром, если необходимо, введу позже
    assert(equations.size() >= vars.size());
    auto linear_system_res = checkIfSystemOfEquationsLinearAndTryToSolveIt(equations, vars);
    if (linear_system_res.second >= 0)
    {
        std::vector<std::vector<abs_ex>> res(vars.size());
        for (int i = 0; i < linear_system_res.first.size(); ++i)
        {
            res[i].resize(1);
            res[i][0] = std::move(linear_system_res.first[i]);
        }
        return res;
    }
    std::vector<std::vector<abs_ex>> res(0);
    return res;
}
std::list<abs_ex> toFactorOfCoefs(const abs_ex & polynom)
{
    auto set = polynom->getSetOfPolyVariables();

    std::list<abs_ex> result;
    if (set.size() <= 1)
    {
        result.push_back(copy(polynom));
        return result;
    }
    for (auto &it : set)
    {
        auto pol = checkIfItsPolynom(polynom, it);
       // //qDebug() << "COEFS: ";
        //for (auto &it : pol)
         //   //qDebug() << it->makeStringOfExpression();
        if (pol.size() > 0 && pol[0] != nullptr)
        {
            auto gcf = std::move(pol[0]);
            if (gcf->getSetOfVariables().empty())
            {
                result.push_back(copy(polynom));
                return result;
            }
            for (int i = 1; i < pol.size(); ++i)
            {
                Polynomial gcf_pol(gcf.get());
                Polynomial coef_pol(pol[i].get());
                if (coef_pol.getSetOfPolyVariables().empty())
                {
                    result.push_back(copy(polynom));
                    return result;
                }
                if (gcf->getSetOfVariables().empty())
                {
                    result.push_back(copy(polynom));
                    return result;
                }
                gcf_pol.reduce();
                coef_pol.reduce();
                gcf = gcd(&gcf_pol, &coef_pol);
            }
            if (!gcf->getSetOfVariables().empty())
            {
                auto second = polynom / gcf;
                result.splice(result.end(), toFactorOfCoefs(gcf));
                result.splice(result.end(), toFactorOfCoefs(second));
                return result;
            }
        }
    }
    result.push_back(copy(polynom));
    return result;
}
std::list<abs_ex> factorizeAsQuadraticFunction(const abs_ex & polynom)
{
    auto set = polynom->getSetOfVariables();
    std::list<abs_ex> result;
    for (auto &it : set)
    {
        auto qc_f = checkIfItsQuadraticFunction(polynom, it);
        if (qc_f[0] != nullptr)
        {
            auto D = sqr(qc_f[1]) - four*qc_f[0]*qc_f[2];
            //полная степень должна выделиться раньше
            if (::isZero(D))
            {
                assert(false);
            }
            if (D->getPositionRelativelyZero() >= 0)
            {
                auto root_D = sqrt(D);
                if (root_D->getId() == ABSOLUTE_VALUE)
                    root_D = static_cast<AbsoluteValue*>(root_D.get())->getExpressionCopy();
                auto w = (-qc_f[1] + root_D)/two/qc_f[0];
                auto u = (-qc_f[1] - root_D)/two/qc_f[0];
                abs_ex x (new Variable(getVariable(it)));
                auto res = qc_f[0]*(x - w)*(x - u);
                if (res->getId() == FRACTAL)
                {
                    auto fr = static_cast<Fractal*>(res.get());
                    if (!fr->getCoefficient().isOne())
                    {
                        if (fr->getCoefficient().getNumerator() == 1)
                        {
                            result.push_back(toAbsEx(fr->getCoefficient()));
                            fr->setCoefficinet(1);
                        }

                    }
                    if (fr->getCoefficient().isOne() && fr->getFractal().second->size() == 0 &&
                            fr->getFractal().first->size() == 2 && fr->getFractal().first->begin()->get()->getId() == POLYNOMIAL &&
                            next(fr->getFractal().first->begin())->get()->getId() == POLYNOMIAL)
                    {
                        result.push_back(std::move(*fr->getFractal().first->begin()));
                        result.push_back(std::move(*next(fr->getFractal().first->begin())));
                        return result;
                    }
                }
            }
        }
    }
    result.push_back(copy(polynom));
    return result;
}
//это можно расширять, но пока только так
std::list<abs_ex> factorizePolynomOfSeveralVariablesByAlgorithmOfCoefsFacts(const abs_ex & polynom)
{
    std::list<abs_ex> res = toFactorOfCoefs(polynom);
    if (res.size() == 1)
        return res;
    for (auto it = res.begin(); it != res.end(); ++it)
    {
        if (it->get()->getSetOfVariables().size() == 1)
        {
            res.splice(res.end(), factorizePolynomOfOneVariable(*it, *it->get()->getSetOfVariables().begin()));
            it = res.erase(it);
        }
        else
        {
            if (it->get()->getId() == POLYNOMIAL)
            {
                auto degr = static_cast<Polynomial*>(it->get())->tryToDistinguishFullDegree();
                if (degr != nullptr)
                    *it = std::move(degr);
            }
        }
    }
    return res;
}
std::list<abs_ex> factorizePolynomOfSeveralVariables(const abs_ex & polynom)
{
    auto full_deg_res = static_cast<Polynomial*>(polynom.get())->tryToDistinguishFullDegree();
    if (full_deg_res != nullptr && full_deg_res->getId() == DEGREE)
    {
        auto deg = Degree::getDegreeOfExpression(full_deg_res.get());
        auto res = factorizePolynomOfSeveralVariables(copy(Degree::getArgumentOfDegree(full_deg_res.get())));
        for (auto &it : res)
            it = pow(std::move(it), deg);
        return res;
    }
    auto deg_res = static_cast<Polynomial*>(polynom.get())->tryToFactorizeByDistingushesOfFullDegree();
    if (deg_res.first.size() > 1)
    {
        return std::move(deg_res.first);
    }
    auto res = factorizePolynomOfSeveralVariablesByAlgorithmOfCoefsFacts(polynom);

    for (auto it = res.begin(); it != res.end(); )
    {
        auto facts = factorizeAsQuadraticFunction(*it);
        if (facts.size() > 1)
        {
            it = res.erase(it);
            res.splice(res.begin(), std::move(facts));
        }
        else
            ++it;
    }
    if (res.size() == 1)
        return res;
    for (auto it = res.begin(); it != res.end(); )
    {
        if (it->get()->getId() != POLYNOMIAL)
        {
            ++it;
            continue;
        }
        auto facts = factorizePolynomOfSeveralVariables(*it);
        if (facts.size() > 1)
        {
            it = res.erase(it);
            res.splice(res.begin(), std::move(facts));

        }
        else
            ++it;
    }
    return res;
}
std::list<abs_ex> factorizePolynomOfOneVariable(const abs_ex & polynom, int var)
{
    auto pol = checkIfItsIntegerPolynom(polynom, var);
    if (pol.empty())
    {
        std::list<abs_ex> res;
        res.push_back(copy(polynom));
        return res;
    }
    Polynom polynom_to_factor(pol.size() - 1);
    for (int i = 0; i < pol.size(); ++i)
        polynom_to_factor[i] = static_cast<Number*>(pol[i].get())->getNumerator();
    auto res = factorize(polynom_to_factor);

    std::list<abs_ex> result;
    abs_ex x(new Variable(getVariable(var)));
    for (auto &it : res)
    {
        abs_ex expr = copy(zero);
        for (int i = 0; i < it.size(); ++i)
            expr = expr + abs_ex(new Number(it[i].toInt())) * pow(x, i);
        result.push_back(std::move(expr));
    }
    return result;
}
std::pair<std::list<abs_ex >, Number> factorizePolynom(const abs_ex &polynom)
{
    if (polynom->getId() > 0)
    {
        std::list<abs_ex> res;
        res.push_back(copy(polynom));
        return {std::move(res), 1};
    }
    //в полиноме уже должно было произойти reduce и takeCommonPart
    Number reduced = static_cast<Polynomial*>(polynom.get())->reduce();
    auto set = polynom->getSetOfVariables();
    std::list<abs_ex> res;
    if (set.size() == 1)
    {
        res = factorizePolynomOfOneVariable(polynom, *set.begin());
    }
    else
        res = factorizePolynomOfSeveralVariables(polynom);

    //это для того, чтобы одинаковые множители свернуть в степень

    Fractal fr(res);
    res = std::move(*fr.getFractal().first);

    return {std::move(res), fr.getCoefficient()*reduced};

    //return {std::move(res), 1};
}



std::list<abs_ex> solveEquation(const abs_ex &equation, int var, const EquationRootsConditions &conditions)
{
    return conditions.selectRoots(solveEquation(equation, var));
}

std::pair<abs_ex, int> tryToDistingushFullDegreeWithPrecisionOfCoefficientWithoutVariable(const abs_ex &polynom, int var)
{
    auto its_pol = checkIfItsPolynom(polynom, var);
    auto deriv = polynom->derivative(var) + zero;
    if (deriv->getId() != POLYNOMIAL)
        return {nullptr, 0};
    auto facts = factorizePolynom(deriv);
    //если p[x] = (g[x])^n + a, то (p[x])' = n*(g[x])^(n-1)*(g[x])'
    //если g[x] = w1[x] * w2[x] * w3[x] * ... * wk[x], то
    //(p[x])' = n*w1[x]^(n-1) * w2[x]^(n-1)*w3[x]^(n-1) * .... * wk[x]^(n - 1) * (g[x])'
    int d = polynom->getMaxDegreeOfVariable(var).getNumerator();
    for (auto it = facts.first.begin(); it != facts.first.end(); ++it)
    {
        int n = static_cast<Number*>(Degree::getDegreeOfExpression(it->get()).get())->getNumerator() + 1;
        if (d % n != 0)
            continue;
        abs_ex g = copy(one);
        for (auto it1 = it; it1 != facts.first.end(); ++it1)
        {
            if (static_cast<Number*>(Degree::getDegreeOfExpression(it->get()).get())->getNumerator() == n - 1)
            {
                g = std::move(g) * copy(Degree::getArgumentOfDegree(it1->get()));
            }
        }
        auto g_polynom = pow(g, n) + zero;
       // qDebug() << g_polynom->toString();
        auto pol = checkIfItsPolynom(g_polynom, var);
        if (its_pol.size() == pol.size())
        {
            auto multiplier = its_pol.back() / pol.back();
            if  (multiplier->getId() == NUMBER)
            {
                auto sub_res = polynom - g_polynom * multiplier;
                if (!sub_res->hasVariable(var))
                    return {g * pow(multiplier, one/numToAbs(n)), n};
            }
        }
    }
    return {nullptr, 0};

}
bool isIntegratingConstantAddictiveThatCanBeChanged(const std::unique_ptr<Fractal> & it)
{
    if (it->getCoefficient() == 1 && it->getFractal().second->empty() &&
            it->getFractal().first->size() == 1 && isIntegratingConstantAndCanChangeIt(it->getFractal().first->begin()->get()->getId()))
        return true;
    return false;
}
bool isIntegratingConstantAddictiveThatCanBeChanged(const Fractal * it)
{
    if (it->getCoefficient() == 1 && it->getFractal().second->empty() &&
            it->getFractal().first->size() == 1 && isIntegratingConstantAndCanChangeIt(it->getFractal().first->begin()->get()->getId()))
        return true;
    return false;
}
bool isIntegratingConstantAddictive(const std::unique_ptr<Fractal> & it)
{
    if (it->getCoefficient() == 1 && it->getFractal().second->empty() &&
            it->getFractal().first->size() == 1 && isIntegratingConstant(it->getFractal().first->begin()->get()->getId()))
        return true;
    return false;
}
bool isIntegratingConstantAddictive(const Fractal * it)
{
    if (it->getCoefficient() == 1 && it->getFractal().second->empty() &&
            it->getFractal().first->size() == 1 && isIntegratingConstant(it->getFractal().first->begin()->get()->getId()))
        return true;
    return false;
}

ComplexNum::ComplexNum(const abs_ex &a, const abs_ex &b)
{
    this->_a = copy(a);
    this->_b = copy(b);
}

ComplexNum::ComplexNum(abs_ex &&a, abs_ex &&b)
{
    this->_a = std::move(a);
    this->_b = std::move(b);
}

ComplexNum::ComplexNum(const abs_ex &a)
{
    this->_a = copy(a);
    this->_b = copy(zero);
}

ComplexNum::ComplexNum(abs_ex &&a)
{
    this->_a = std::move(a);
    this->_b = copy(zero);
}

bool ComplexNum::isReal() const
{
    return isZero(_b);
}

ComplexNum::ComplexNum(const ComplexNum &num)
{
    this->_a = copy(num._a);
    this->_b = copy(num._b);
}

ComplexNum::ComplexNum(ComplexNum &&num)
{
    this->_a = std::move(num._a);
    this->_b = std::move(num._b);
}

ComplexNum &ComplexNum::operator=(const ComplexNum &num)
{
    this->_a = copy(num._a);
    this->_b = copy(num._b);
    return *this;
}

ComplexNum &ComplexNum::operator=(ComplexNum &&num)
{
    this->_a = std::move(num._a);
    this->_b = std::move(num._b);
    return *this;
}

QString ComplexNum::toString() const
{
    if (isZero(this->b()))
        return this->a()->toString();
    return  this->a()->toString()  + "  +  i * (" + this->b()->toString() + ")";
}

const abs_ex &ComplexNum::a() const
{
    return _a;
}

const abs_ex &ComplexNum::b() const
{
    return _b;
}
std::list<std::pair<ComplexNum, int> > solveQuadraticEquationInComplexNumbers(std::vector<abs_ex> && polynom)
{
    std::list<std::pair<ComplexNum, int>> res;
    auto D = sqr(polynom[1]) - four*polynom[0]*polynom[2];
    if (D->getPositionRelativelyZero() >= 0)
    {
        if (isZero(D))
            res.push_back({(-polynom[1])/two/polynom[2], 2});
        else
        {
            res.push_back({(-polynom[1] + sqrt(D))/two/polynom[2], 1});
            res.push_back({(-polynom[1] - sqrt(D))/two/polynom[2], 1});
        }
    }
    else
        //опять же, суем только один корень, так как второй - комплексно сопряженный, присутствует неявно
        res.push_back({ComplexNum(-polynom[1]/two/polynom[2], sqrt(-D)/two/polynom[2]), 1});
    return res;
}
std::list<std::pair<ComplexNum, int> > solveCubicEquationInComplexNumbers(std::vector<abs_ex> &&polynom)
{
    auto a = std::move(polynom[3]);
    auto b = std::move(polynom[2]);
    auto c = std::move(polynom[1]);
    auto d = std::move(polynom[0]);

    auto p = (three * a * c - sqr(b))/three/sqr(a);
    auto q = (two * pow(b, 3) - numToAbs(9)*a*b*c + numToAbs(27)*a*a*d)/numToAbs(27)/pow(a, 3);

    auto Q = pow(p, 3)/numToAbs(27) + sqr(q) / numToAbs(4);

    std::list<std::pair<ComplexNum, int> > preres;
    if (isZero(Q))
    {
        if (isZero(p) && isZero(q))
        {
            preres.push_back({copy(zero), 3});
        }
        else
        {
            preres.push_back({two*pow(-q/two, one/three), 1});
            preres.push_back({-pow(-q/two, one/three), 2});
        }
    }
    else if (Q->getPositionRelativelyZero() >= 0)
    {
        auto alpha = pow(-q/two + sqrt(Q), one/three);
        auto beta = pow(-q/two - sqrt(Q), one/three);

        preres.push_back({alpha + beta, 1});
        preres.push_back({ComplexNum(-(alpha + beta)/two, (alpha - beta)/two * sqrt(three)), 1});
    }
    else
    {
        abs_ex angle_arg = acos(three * q / two / p * sqrt(-three / p)) / three;
        abs_ex mult = two * sqrt(-p/three);
        for (int i = 0; i < 3; ++i)
            preres.push_back({mult * cos(angle_arg - numToAbs(i) * two*getPi()/three), 1});
    }

    for (auto &it : preres)
    {
        it = {ComplexNum(it.first.a() - b/three/a, copy(it.first.b())), it.second};
    }
    return std::move(preres);
}
//https://studwork.org/spravochnik/matematika/reshenie-uravneniy-3-y-i-4-y-stepeni
std::list<std::pair<ComplexNum, int> > solveForthDegreeEquationInComplexNumbers(std::vector<abs_ex> && polynom)
{
    auto a1 = polynom[3] / polynom[4];
    auto a2 = polynom[2] / polynom[4];
    auto a3 = polynom[1] / polynom[4];
    auto a4 = polynom[0] / polynom[4];

    auto A = a2 - three * sqr(a1)/numToAbs(8);
    auto B = a3 - a1*a2/two + pow(a1, 3)/numToAbs(8);
    auto C = a4 - a1*a3/four + sqr(a1)*a2/numToAbs(16) - three*pow(a1, 4)/numToAbs(256);

    std::vector<abs_ex> resolventa(4);
    resolventa[3] = copy(one);
    resolventa[2] = two * A;
    resolventa[1] = sqr(A) - four * C;
    resolventa[0] = -sqr(B);

    auto resolventa_roots = solveCubicEquationInComplexNumbers(std::move(resolventa));

    abs_ex t = nullptr;
    for (auto &it : resolventa_roots)
        if (it.first.isReal() && it.first.a()->getPositionRelativelyZero() >= 0)
            t = copy(it.first.a());
    assert(t != nullptr);

    auto t_sqrt = sqrt(t);
    auto a = copy(t_sqrt);
    auto b = (A + t - B/t_sqrt)/two;
    auto c = (A + t + B/t_sqrt)/two;

    std::list<std::pair<ComplexNum, int> > preres;

    std::vector<abs_ex> mult1(3);
    mult1[2] = copy(one);
    mult1[1] = copy(a);
    mult1[0] = copy(b);

    preres.splice(preres.end(),  solveQuadraticEquationInComplexNumbers(std::move(polynom)));

    std::vector<abs_ex> mult2(3);
    mult2[2] = copy(one);
    mult2[1] = -a;
    mult2[0] = copy(c);

    preres.splice(preres.end(),  solveQuadraticEquationInComplexNumbers(std::move(polynom)));

    std::list<std::pair<ComplexNum, int> > res;
    for (auto &it : preres)
    {
        bool found_match = false;
        for (auto &it1 : res)
        {
            if (it1.first == it.first)
            {
                found_match = true;
                it1.second += it.second;
                break;
            }
        }
        if (!found_match)
            res.push_back(std::move(it));
    }
    for (auto &it : res)
        it.first = ComplexNum(it.first.a() - a1/four, copy(it.first.b()));
    return res;

}
bool isNonHighestDegreesZeroes(const std::vector<abs_ex> & polynom)
{
    for (int i = 1; i < polynom.size() - 1; ++i)
        if (!isZero(polynom[i]))
            return false;
    return true;
}
//полиномиальность должна гарантироваться
//не решает кубические уравнения, возвращает всегда n корней. в листе первое - корень, второе - кратность корня
//булевое значение - удалось ли полностью решить уравнение (найти все n комплексных корней)
//при этом, если корень комлексный, то один объект complexnum идет за два сопряженных корня
//число рядом с ним будет означать количество таких комплексно сопряженных ПАР
std::pair<std::list<std::pair<ComplexNum, int> >, bool> solvePolynomialEquationInComplexNumber(const abs_ex &equation, int var)
{
    int gcd_of_nums = equation->getGcdOfNumeratorsOfDegrees(var);
    if (gcd_of_nums > 1)
    {
        abs_ex temp_var;
        if (gcd_of_nums % 2 == 0)
            temp_var = abs_ex(new Variable(systemVar(0, std::numeric_limits<int>::max())));
        else
            temp_var = systemVarExpr();
        abs_ex repl_equation = copy(equation);
        setUpExpressionIntoVariable(repl_equation, pow(temp_var, Number(1, gcd_of_nums)), var);

        auto preres = solvePolynomialEquationInComplexNumber(repl_equation, temp_var->getId());
        if (!preres.second)
            return {std::list<std::pair<ComplexNum, int>>(), false};
        for (auto &it : preres.first)
            if (!it.first.isReal())
                return {std::list<std::pair<ComplexNum, int>>(), false};
        if (gcd_of_nums % 2 == 1)
        {
            std::list<std::pair<ComplexNum, int>> res;
            for (auto &it : preres.first)
                res.push_back({pow(it.first.a(), Number(1)/Number(gcd_of_nums)), it.second * gcd_of_nums});
            return {std::move(res), true};
        }

        std::list<std::pair<ComplexNum, int>> res;
        for (auto &it : preres.first)
        {
            if (it.first.a()->getPositionRelativelyZero() >= 0)
            {
                res.push_back({pow(it.first.a(), Number(1) / Number(gcd_of_nums)), it.second * (gcd_of_nums / 2)});
                res.push_back({-pow(it.first.a(), Number(1)/Number(gcd_of_nums)), it.second * (gcd_of_nums / 2)});
            }
            else
            {
                //суем только один такой корень, потому что второй - комплексно сопряженный, будет присутствовать неявно
                res.push_back({ComplexNum(zero, pow(-it.first.a(), Number(1) / Number(gcd_of_nums))), it.second * (gcd_of_nums / 2)});
            }
        }
        return {std::move(res), true};
    }
    auto polynom = checkIfItsPolynom(equation, var);
    assert(polynom.size() >= 2);
    if (isNonHighestDegreesZeroes(polynom))
    {//если размер вектора четен, то степень уравнения - нечетна
        std::list<std::pair<ComplexNum, int>> res;
        if (polynom.size() % 2 == 0)
        {
            res.push_back({pow(-polynom[0], Number(1)/Number(polynom.size() - 1)), polynom.size() - 1});
        }
        else
        {
            auto right = -polynom[0];
            if (right->getPositionRelativelyZero() >= 0)
            {
                res.push_back({pow(right, Number(1)/Number(polynom.size() - 1)), (polynom.size() - 1)/2});
                res.push_back({-pow(right, Number(1)/Number(polynom.size() - 1)), (polynom.size() - 1)/2});
            }
            else
                res.push_back({ComplexNum(zero, pow(-right, Number(1)/Number(polynom.size() - 1))), (polynom.size() - 1)/2});
        }
        return {std::move(res), true};
    }
    if (polynom.size() == 3)
    {
        return {solveQuadraticEquationInComplexNumbers(std::move(polynom)), true};
    }
    auto factors = factorizePolynom(equation);
    if (factors.first.size() > 1 || factors.first.begin()->get()->getId() == DEGREE)
    {
        std::list<std::pair<ComplexNum, int> > res;
        for (auto &it : factors.first)
        {
            int deg = static_cast<Number*>(Degree::getDegreeOfExpression(it).get())->getNumerator();

            auto preres = solvePolynomialEquationInComplexNumber(Degree::getArgumentOfDegree(it), var);
            if (!preres.second)
                return {std::list<std::pair<ComplexNum, int> >(), false};

            for (auto &it1: preres.first)
            {
                bool found_match = false;
                for (auto &it2 : res)
                    if (it2 == it1)
                    {
                        found_match = true;
                        it2.second += it1.second * deg;
                        break;
                    }
                if (!found_match)
                    res.push_back(std::move(it1));
            }

        }
        return {std::move(res), true};
    }
    if (polynom.size() == 4)
        return {solveCubicEquationInComplexNumbers(std::move(polynom)), true};
    if (polynom.size() == 5)
        return {solveForthDegreeEquationInComplexNumbers(std::move(polynom)), true};

    return {std::list<std::pair<ComplexNum, int> >(), false};
}

bool operator==(const ComplexNum &a, const ComplexNum &b)
{
    return subCompare(a.a(), b.a()) && (subCompare(a.b(), b.b()) || subCompare(a.b(), -b.b()));
}

std::list<abs_ex> solveEquationsForOneVariable(const std::list<abs_ex> &equations, int var, const EquationRootsConditions &conditions)
{
    std::list<abs_ex> result;
    for (auto &it : equations)
    {
        std::list<abs_ex> this_result = solveEquation(it, var, conditions);
        for (auto &it1 : this_result)
        {
            bool is_uniq = true;
            for (auto &it2 : result)
                if (subCompare(it2, it1))
                {
                    is_uniq = false;
                    break;
                }
            bool is_of_all_eqs = true;
            for (auto &it2 : equations)
            {
                abs_ex check = copy(it2);
                setUpExpressionIntoVariable(check, it1, var);
                if (!isZero(check))
                {
                    is_of_all_eqs = false;
                    break;
                }
            }
            if (is_uniq && is_of_all_eqs)
                result.push_back(std::move(it1));
        }
    }
    return result;
}
