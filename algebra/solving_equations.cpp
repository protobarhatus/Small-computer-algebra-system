#include <vector>
#include "abstractexpression.h"
#include "number.h"
#include "some_algebra_expression_conversions.h"
#include "fractal.h"
#include "polynomial.h"
#include "variablesdistributor.h"
std::vector<abs_ex> solveEquation(const abs_ex & equation, int var)
{
    //is linear equation
    auto ln_f = checkIfItsLinearFunction(equation, var);
    if (ln_f.first != nullptr)
        return std::vector<abs_ex>(1, -ln_f.second / ln_f.first);
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
std::pair<std::vector<abs_ex>, int> checkIfSystemOfEquationsLinearAndTryToSolveIt(const std::vector<abs_ex> & system, const std::vector<int>& vars)
{

}
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars)
{
    //возможность решать это с параметром, если необходимо, введу позже
    assert(equations.size() >= vars.size());

}
