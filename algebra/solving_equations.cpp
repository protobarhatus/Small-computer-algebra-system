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
std::list<abs_ex> factorizePolynomOfOneVariable(const abs_ex & polynom, int var);
std::vector<abs_ex> solveEquation(const abs_ex & equation, int var)
{
    //is linear equation
    auto ln_f = checkIfItsLinearFunction(equation, var);
    if (ln_f.first != nullptr)
    {
        std::vector<abs_ex> res(1);
        res[0] = -ln_f.second / ln_f.first;
        return res;
    }
    //is quadratic equation
    auto qc_f = checkIfItsQuadraticFunction(equation, var);
    if (qc_f[0] != nullptr)
    {
        auto D = sqr(qc_f[1]) - four*qc_f[0]*qc_f[2];
        if (*D == *zero)
        {
            std::vector<abs_ex> res(1);
            res[0] = -qc_f[1]/two/qc_f[0];
            return res;
        }
        if (D->getPositionRelativelyZero() < 0)
        {
            std::vector<abs_ex> res(0);
            return res;
        }
        std::vector<abs_ex> res(2);
        res[0] = (-qc_f[1] + sqrt(D))/(two*qc_f[0]);
        res[1] = (-qc_f[1] - sqrt(D))/(two*qc_f[0]);
        return res;
    }
        //return { -ln_f.second / ln_f.first};
    assert(false);
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
std::vector<abs_ex> checkIfEquationIsLinearAnsGetCoefficients(const abs_ex & equation, const std::vector<int> & vars)
{
    std::vector<abs_ex> result(vars.size() + 1);
    for (auto &it : result)
        it = copy(zero);
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
       // qDebug() << "COEFS: ";
        //for (auto &it : pol)
         //   qDebug() << it->makeStringOfExpression();
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
            if (D->getPositionRelativelyZero() > 0)
            {

                auto w = (-qc_f[1] + sqrt(D))/two/qc_f[0];
                auto u = (-qc_f[1] - sqrt(D))/two/qc_f[0];
                abs_ex x (new Variable(getVariable(it)));
                auto res = qc_f[0]*(x - w)*(x - u);
                if (res->getId() == FRACTAL)
                {
                    auto fr = static_cast<Fractal*>(res.get());
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
std::pair<std::list<std::unique_ptr<AbstractExpression> >, Number> factorizePolynom(const std::unique_ptr<AbstractExpression> &polynom)
{
    //в полиноме уже должно было произойти reduce и takeCommonPart
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

    return {std::move(res), fr.getCoefficient()};

    //return {std::move(res), 1};
}
