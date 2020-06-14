#include "abstractexpression.h"
#include "assert.h"
#include "fractal.h"
#include "some_algebra_expression_conversions.h"
#include "polynomial.h"
#include "degree.h"
#include "sinus.h"
#include "cosinus.h"
#include "tangent.h"
#include "cotangent.h"
#include "variablesdistributor.h"
#include "logarithm.h"
#include "arcsinus.h"
#include "arctangent.h"
AbstractExpression::AbstractExpression()
{

}

AbstractExpression &AbstractExpression::operator=(const AbstractExpression &expr)
{
    return *this;
}

AbstractExpression::AbstractExpression(const AbstractExpression &expr)
{

}


AbstractExpression::~AbstractExpression()
{

}
bool AbstractExpression::less(const AbstractExpression * left, const AbstractExpression * right)
{
    if (left->getId() != DIFFERENTIAL && right->getId() == DIFFERENTIAL)
        return true;
    if (left->getId() == DIFFERENTIAL && right->getId() != DIFFERENTIAL)
        return false;
    if (left->getId() != right->getId())
    {

        if ((left->getId() <= 0 && right->getId() <= 0) || (left->getId() > 0 && right->getId() > 0))
            return left->getId() < right->getId();
        else
        {
            return left->getId() > right->getId();
        }
    }
    else
        return *left < *right;
}
bool AbstractExpression::lessToSort(const abs_ex &left, const abs_ex &right)
{
    if (left->getId() != DIFFERENTIAL && right->getId() == DIFFERENTIAL)
        return true;
    if (left->getId() == DIFFERENTIAL && right->getId() != DIFFERENTIAL)
        return false;
    if (left->getId() != right->getId())
    {
        if ((left->getId() < 0 && right->getId() < 0) || (left->getId() > 0 && right->getId() > 0))
            return left->getId() < right->getId();
        else
        {
            return left->getId() > right->getId();
        }
    }
    else
        return *left < *right;
}
bool AbstractExpression::operator!=(AbstractExpression &right)
{
    return !(*this == right);
}
abs_ex AbstractExpression::operator*(AbstractExpression &expr)
{
    //den is empty
    fractal_argument arg, den;
    arg.push_back(makeAbstractExpression(this->getId(), this));
    arg.push_back(makeAbstractExpression(expr.getId(), &expr));
    abs_ex frac = abs_ex(new Fractal(&arg, &den));
    return frac->downcast();

}

abs_ex AbstractExpression::operator/(AbstractExpression &expr)
{
    abs_ex frac = abs_ex(new Fractal(this, &expr));
    return frac->downcast();
}
abs_ex AbstractExpression::operator+(AbstractExpression & expr)
{
    abs_ex polynom = abs_ex(new Polynomial(this, &expr));

    return polynom->downcast();
}
abs_ex AbstractExpression::operator-(AbstractExpression &expr)
{
    std::unique_ptr<Fractal> subtrahend = std::unique_ptr<Fractal>(new Fractal(&expr, -1));
    abs_ex polynom = abs_ex(new Polynomial(this, subtrahend.get()));

    return polynom->downcast();
}
bool AbstractExpression::canDowncast()
{
    return this->canDowncastTo();
}
abs_ex AbstractExpression::downcast()
{
    if (!this->canDowncast())
        return makeAbstractExpression(this->getId(), this);
    abs_ex expr = copy(this);
    while (expr->canDowncast())
        expr = expr->downcastTo();
    return expr;
}
abs_ex operator*(const abs_ex & left, const abs_ex & right)
{
    return std::move(*left.get() * *right.get());
}
abs_ex operator/(const abs_ex & left, const abs_ex & right)
{
    return std::move(*left.get() / *right.get());
}

abs_ex operator+(const abs_ex & left, const abs_ex & right)
{
    return std::move(*left.get() + *right.get());
}
abs_ex operator-(const abs_ex & left, const abs_ex & right)
{
    return std::move(*left.get() - *right.get());
}
int AbstractExpression::getPositionRelativelyZero()
{
    if (this->getSetOfVariables().empty())
    {
        double res = this->getApproximateValue();
        if (res >= 0)
            return 1;
        else
            return -1;
    }
    return this->getPositionRelativelyZeroIfHasVariables();

}

bool AbstractExpression::hasVariable(int var)
{
    auto s = this->getSetOfVariables();
    return s.find(var) != s.end();
}


QString getStringArgumentOfTrigonometricalFunction(abs_ex &expr)
{
    return getStringArgumentOfTrigonometricalFunction(expr.get());
}

QString getStringArgumentOfTrigonometricalFunction(AbstractExpression *expr)
{
    AbstractExpression *arg = Degree::getArgumentOfDegree(expr);
    assert(arg->getId() == SINUS || arg->getId() == COSINUS || arg->getId() == TANGENT || arg->getId() == COTANGENT);
    switch (arg->getId()) {
    case SINUS:
        return static_cast<Sinus*>(arg)->getStringArgument();
    case COSINUS:
        return static_cast<Cosinus*>(arg)->getStringArgument();
    case TANGENT:
        return static_cast<Tangent*>(arg)->getStringArgument();
    case COTANGENT:
        return static_cast<Cotangent*>(arg)->getStringArgument();
    default:
        assert(false);

    }
}

abs_ex getArgumentOfFunction(abs_ex && expr)
{
    AbstractExpression *arg = Degree::getArgumentOfDegree(expr.get());
    assert(arg->getId() == SINUS || arg->getId() == COSINUS || arg->getId() == TANGENT || arg->getId() == COTANGENT ||
           arg->getId() == LOGARITHM || arg->getId() == ARCTANGENT || arg->getId() == ARCSINUS);
    switch (arg->getId()) {
    case SINUS:
        return static_cast<Sinus*>(arg)->getArgumentMoved();
    case COSINUS:
        return static_cast<Cosinus*>(arg)->getArgumentMoved();
    case TANGENT:
        return static_cast<Tangent*>(arg)->getArgumentMoved();
    case COTANGENT:
        return static_cast<Cotangent*>(arg)->getArgumentMoved();
    case LOGARITHM:
        return static_cast<Logarithm*>(arg)->getArgumentMoved();
    case ARCTANGENT:
        return static_cast<ArcTangent*>(arg)->getArgumentMoved();
    case ARCSINUS:
        return static_cast<ArcSinus*>(arg)->getArgumentMoved();
    default:
        assert(false);

    }
}

bool isDegreeOfTrigonometricalFunction(abs_ex &expr)
{
    auto arg = Degree::getArgumentOfDegree(expr.get())->getId();
    return arg == SINUS || arg == COSINUS || arg == TANGENT || arg == COTANGENT;
}
/*
std::vector<abs_ex > replaceEveryFunctionOnSystemVariable(abs_ex &expr)
{
    auto functions = expr->getSetOfFunctions();
    std::vector<abs_ex> result;
    int counter = 0;
    for (auto &it : functions)
    {
        abs_ex var(new Variable(systemVar(counter)));
        result.push_back(expr->changeSomePartOn(it, var));
        ++counter;
    }
    return result;
}*/
std::map<int, abs_ex> replaceEveryFunctionOnSystemVariable(abs_ex &expr, std::map<QString, int> &funcs)
{
    return replaceEveryFunctionOnSystemVariable(expr.get(), funcs);
}
void replaceSystemVariablesBackToFunctions(abs_ex &expr, std::map<int, abs_ex> & funcs)
{
    replaceSystemVariablesBackToFunctions(expr.get(), funcs);
}
std::map<int, abs_ex> replaceEveryFunctionOnSystemVariable(AbstractExpression *expr, std::map<QString, int> &funcs)
{
   // qDebug() << "BEFORE: " << expr->makeStringOfExpression();
    auto functions = expr->getSetOfFunctions();
    std::map<int, abs_ex> result;
    for (auto &it : functions)
    {
        auto it1 = funcs.find(it);
        abs_ex var;
        if (it1 == funcs.end())
        {
            var = abs_ex(new Variable(systemVar()));
            funcs.insert({it, var->getId()});
        }
        else
            var = abs_ex(new Variable(systemVar(it1->second)));
        result.insert({var->getId(), expr->changeSomePartOn(it, var)});
    }
    //qDebug() << "AFTER: " << expr->makeStringOfExpression();
    return result;
}
void replaceSystemVariablesBackToFunctions(AbstractExpression *expr, std::map<int, abs_ex> & funcs)
{
    //qDebug() << "WITH: " << expr->makeStringOfExpression();
    for (auto &it : funcs)
    {
        expr->changeSomePartOn(systemVar(it.first).makeStringOfExpression(), it.second);
    }

   // qDebug() << "WITHOUT: " << expr->makeStringOfExpression();
}
void replaceSystemVariablesToExpressions(AbstractExpression *expr, std::map<int, abs_ex> & funcs)
{
    //qDebug() << "WITH: " << expr->makeStringOfExpression();
    for (auto &it : funcs)
    {
        expr->changeSomePartOnExpression(systemVar(it.first).makeStringOfExpression(), it.second);
    }
    expr->simplify();
   // qDebug() << "WITHOUT: " << expr->makeStringOfExpression();
}
void replaceSystemVariablesToExpressions(abs_ex & expr, std::map<int, abs_ex> & funcs)
{
    replaceSystemVariablesToExpressions(expr.get(), funcs);
}
/*
void replaceSystemVariablesBackToFunctions(abs_ex & expr, std::vector<abs_ex > &functions)
{
    for (int i = 0; i < functions.size(); ++i)
    {
        expr->changeSomePartOn(systemVar(i).makeStringOfExpression(), functions[i]);
    }
}
*/
abs_ex getArgumentOfFunction(const abs_ex &expr)
{
    AbstractExpression *arg = Degree::getArgumentOfDegree(expr.get());
    assert(arg->getId() == SINUS || arg->getId() == COSINUS || arg->getId() == TANGENT || arg->getId() == COTANGENT ||
           arg->getId() == LOGARITHM || arg->getId() == ARCTANGENT || arg->getId() == ARCSINUS);
    switch (arg->getId()) {
    case SINUS:
        return static_cast<Sinus*>(arg)->getArgumentsCopy();
    case COSINUS:
        return static_cast<Cosinus*>(arg)->getArgumentsCopy();
    case TANGENT:
        return static_cast<Tangent*>(arg)->getArgumentsCopy();
    case COTANGENT:
        return static_cast<Cotangent*>(arg)->getArgumentsCopy();
    case LOGARITHM:
        return static_cast<Logarithm*>(arg)->getArgumentsCopy();
    case ARCTANGENT:
        return static_cast<ArcTangent*>(arg)->getArgumentsCopy();
    case ARCSINUS:
        return static_cast<ArcSinus*>(arg)->getArgumentsCopy();
    default:
        assert(false);

    }
}

abs_ex absEx(int num)
{
    return abs_ex(new Number(num));
}


std::pair<abs_ex, abs_ex > checkIfItsLinearFunction(const abs_ex &func, int var)
{
    return checkIfItsLinearFunction(func.get(), var);
}

std::pair<abs_ex, abs_ex > checkIfItsLinearFunction(const AbstractExpression *func, int var)
{
    if (func->getId() == var)
        return {copy(one), copy(zero)};
    if (func->getId() == FRACTAL)
        return static_cast<const Fractal*>(func)->checkIfItIsLinearFunction(var);
    if (func->getId() == POLYNOMIAL)
        return static_cast<const Polynomial*>(func)->checkIfItIsLinearFunction(var);
    return {nullptr, nullptr};
}

std::pair<abs_ex, abs_ex > checkIfItsFunctionOfLinearArgument(const abs_ex &func, int var)
{
    return checkIfItsFunctionOfLinearArgument(func.get(), var);
}

abs_ex numToAbs(int num)
{
    return abs_ex(new Number(num));
}

std::pair<abs_ex, abs_ex > checkIfItsFunctionOfLinearArgument(const AbstractExpression *func, int var)
{
    switch (func->getId()) {
    case SINUS:
        return checkIfItsLinearFunction(static_cast<const Sinus*>(func)->getArgument(), var);
    case COSINUS:
        return checkIfItsLinearFunction(static_cast<const Cosinus*>(func)->getArgument(), var);
    case TANGENT:
        return checkIfItsLinearFunction(static_cast<const Tangent*>(func)->getArgument(), var);
    case COTANGENT:
        return checkIfItsLinearFunction(static_cast<const Cotangent*>(func)->getArgument(), var);
    case LOGARITHM:
        return checkIfItsLinearFunction(static_cast<const Logarithm*>(func)->getArgument(), var);
    case ARCTANGENT:
        return checkIfItsLinearFunction(static_cast<const ArcTangent*>(func)->getArgument(), var);
    case ARCSINUS:
        return checkIfItsLinearFunction(static_cast<const ArcSinus*>(func)->getArgument(), var);
    default:
        return {nullptr, nullptr};
    }
}

std::array<abs_ex, 3> checkIfItsQuadraticFunction(const AbstractExpression *func, int var)
{
    if (func->getId() == DEGREE && Degree::getArgumentOfDegree(const_cast<AbstractExpression*>(func))->getId() == var &&
            *Degree::getDegreeOfExpression(const_cast<AbstractExpression*>(func)) == *two)
        return {copy(one), copy(zero), copy(zero)};
        if (func->getId() == DEGREE && *Degree::getDegreeOfExpression(const_cast<AbstractExpression*>(func)) == *two)
        {
            auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(const_cast<AbstractExpression*>(func)), var);
            return {pow(ln_f.first, 2), two*ln_f.first * ln_f.second, pow(ln_f.second, 2)};
        }
    if (func->getId() == FRACTAL)
    {
        const Fractal * fr = static_cast<const Fractal*>(func);
        auto frac = fr->getFractal();
        bool has_x_square = false;
        bool has_other_x = false;
        for (auto &it : *frac.first)
        {
            if (it->getId() == DEGREE && *Degree::getDegreeOfExpression(it.get()) == *two &&
                    Degree::getArgumentOfDegree(it.get())->getId() == var)
            {
                has_x_square = true;
            }
            else
            {
                auto vars = it->getSetOfVariables();
                if (vars.find(var) != vars.end())
                {
                    has_other_x = true;
                    break;
                }
            }
        }
        for (auto &it : *frac.second)
        {
            auto vars = it->getSetOfVariables();
            if (vars.find(var) != vars.end())
            {
                has_other_x = true;
                break;
            }
        }
        if (has_other_x)
            return {nullptr, nullptr ,nullptr};
        if (has_x_square)
        {
            auto mult = fr->getFractalWithoutVariable(var);
            return {toAbsEx(std::move(mult)), copy(zero), copy(zero)};
        }
        /*auto mult = fr->getFractalWithoutVariable(var);
        auto var_fr = *fr / *mult;
        if (var_fr->getId() == FRACTAL)
        {
            auto var_fr_list = var_fr->getFractal();
            if (!var_fr_list.second->empty())
                return {nullptr, nullptr, nullptr};
            if (var_fr_list.first->size() != 2)
                return {nullptr, nullptr, nullptr};

            abs_ex & f = *var_fr_list.first->begin();
            abs_ex & s = *next(var_fr_list.second->begin());
            if ((f->getId() > 0 && s->getId() == POLYNOMIAL) || (f->getId() == POLYNOMIAL && s->getId() == POLYNOMIAL) && (f->getId() == POLYNOMIAL && s->getId() == FRACTAL))
            {
                auto res = checkIfItsQuadraticFunction(toAbsEx(var_fr) + zero, var);
                if (res[0] == nullptr)
                    return {nullptr, nullptr, nullptr};
                auto a_m = toAbsEx(std::move(mult));
                return {a_m * res[0], a_m * res[1], a_m * res[2]};
            }
            else
                return {nullptr, nullptr, nullptr};
        }*/

    }
  /*  auto ln_f = checkIfItsLinearFunction(func, var);
    if (ln_f.first != nullptr)
    {
        return {copy(zero), std::move(ln_f.first), std::move(ln_f.second)};
    }*/
    auto ln_f = checkIfItsLinearFunction(func, var);
    if (ln_f.first != nullptr)
        return {nullptr, nullptr, nullptr};
    if (func->getId() != POLYNOMIAL)
        return {nullptr, nullptr, nullptr};
    return static_cast<const Polynomial*>(func)->checkQuadraticFunction(var);
}

std::array<abs_ex, 3> checkIfItsQuadraticFunction(const abs_ex &func, int var)
{
    return checkIfItsQuadraticFunction(func.get(), var);
}

bool isZero(const abs_ex &expr)
{
    return expr->getId() == NUMBER && static_cast<const Number*>(expr.get())->isZero();
}

abs_ex operator-(const abs_ex &arg)
{
    return minus_one * arg;
}

abs_ex operator-(abs_ex &&arg)
{
    return minus_one * std::move(arg);
}

abs_ex getArgumentOfFunction(AbstractExpression *expr)
{
    AbstractExpression *arg = Degree::getArgumentOfDegree(expr);
    assert(arg->getId() == SINUS || arg->getId() == COSINUS || arg->getId() == TANGENT || arg->getId() == COTANGENT
           ||arg->getId() == LOGARITHM || arg->getId() == ARCTANGENT || arg->getId() == ARCSINUS);
    switch (arg->getId()) {
    case SINUS:
        return static_cast<Sinus*>(arg)->getArgumentsCopy();
    case COSINUS:
        return static_cast<Cosinus*>(arg)->getArgumentsCopy();
    case TANGENT:
        return static_cast<Tangent*>(arg)->getArgumentsCopy();
    case COTANGENT:
        return static_cast<Cotangent*>(arg)->getArgumentsCopy();
    case LOGARITHM:
        return static_cast<Logarithm*>(arg)->getArgumentsCopy();
    case ARCTANGENT:
        return static_cast<ArcTangent*>(arg)->getArgumentsCopy();
    case ARCSINUS:
        return static_cast<ArcSinus*>(arg)->getArgumentsCopy();
    default:
        assert(false);

    }
}

std::vector<abs_ex > checkIfItsPolynom(const AbstractExpression *func, int var)
{
    std::vector<abs_ex> res(1);
    res[0]=copy(zero);
    if (func->getId() == POLYNOMIAL)
    {
        auto pol = static_cast<const Polynomial*>(func)->getMonomialsPointers();
        for (auto &it : pol)
        {
            auto monom = checkIfItsMonomOfSomeDegree(it, var);
            if (monom.second == nullptr || !monom.first.isInteger())
            {
                res.resize(0);
                return res;
            }
            int deg = monom.first.getNumerator();
            if (deg >= res.size())
            {
                res.resize(deg + 1);
                res[deg] = std::move(monom.second->downcast());
            }
            else
            {
                if (res[deg] == nullptr)
                    res[deg] = copy(zero);
                res[deg] = std::move(res[deg]) + std::move(monom.second);
            }
        }
        for (auto &it : res)
            if (it == nullptr)
                it = copy(zero);
    }
    else
    {
        auto monom = checkIfItsMonomOfSomeDegree(func, var);
        if (monom.second == nullptr || !monom.first.isInteger())
        {
            res.resize(0);
            return res;
        }
        int deg = monom.first.getNumerator();
        if (deg >= res.size())
        {
            res.resize(deg + 1);
            res[deg] = std::move(monom.second);
        }
        else
        {
            res[deg] = std::move(res[deg]) + std::move(monom.second);
        }
    }
    return res;
}
//да-да, я использую кучу const_cast, что херня, но в данной ситуации мне проще так, рили
std::pair<Number, abs_ex > checkIfItsMonomOfSomeDegree(const AbstractExpression *func, int var)
{
    if (func->getId() == var)
        return {1, copy(one)};
    if (func->getId() == DEGREE)
    {
        if (Degree::getArgumentOfDegree(const_cast<AbstractExpression*>(func))->getId() == var &&
                Degree::getDegreeOfExpression(const_cast<AbstractExpression*>(func))->getId() == NUMBER)
            return {*static_cast<Number*>(Degree::getDegreeOfExpression(const_cast<AbstractExpression*>(func)).get()), copy(one)};
    }
    if (func->getId() == FRACTAL)
    {
        auto frac = static_cast<const Fractal*>((func));
        auto fr = frac->getFractal();
        Number deg_of_x = 0;
        for (auto &it : *fr.first)
        {
            if (Degree::getArgumentOfDegree(it.get())->getId() == var &&
                    Degree::getDegreeOfExpression(it.get())->getId() == NUMBER)
                deg_of_x = *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get());
            else
            {
                auto s = it->getSetOfVariables();
                if (s.find(var) != s.end())
                {
                    return {Number::makeErrorNumber(), nullptr};
                }
            }
        }
        for (auto &it : *fr.second)
        {
            auto s = it->getSetOfVariables();
            if (s.find(var) != s.end())
            {
                return {Number::makeErrorNumber(), nullptr};
            }
        }
        return {deg_of_x, toAbsEx(frac->getFractalWithoutVariable(var))};
    }

    auto s = func->getSetOfVariables();
    if (s.find(var) != s.end())
    {
        return {Number::makeErrorNumber(), nullptr};
    }
    return {0, copy(const_cast<AbstractExpression*>(func))};
}

std::vector<abs_ex > checkIfItsIntegerPolynom(const AbstractExpression *func, int var)
{
    if (func->getSetOfVariables().size() > 1)
        return std::vector<abs_ex>();
    auto pol_res = checkIfItsPolynom(func, var);
    for (auto &it : pol_res)
        if (it->getId() != NUMBER || !static_cast<Number*>(it.get())->isInteger())
            return std::vector<abs_ex>();
    return std::move(pol_res);
}

std::vector<abs_ex > checkIfItsPolynom(const abs_ex &func, int var)
{
    return checkIfItsPolynom(func.get(), var);
}

std::vector<abs_ex > checkIfItsIntegerPolynom(const abs_ex &func, int var)
{
    return checkIfItsIntegerPolynom(func.get(), var);
}

bool subCompare(const abs_ex &a, const abs_ex &b)
{
    return *(a - b) == *zero;
}

bool isDegreeOfArcTrigonometricalFunction(abs_ex &expr)
{
    auto arg = Degree::getArgumentOfDegree(expr.get());
    if (arg->getId() == ARCSINUS || arg->getId() == ARCTANGENT)
        return true;
    return false;
}

bool isExponentialFunction(abs_ex& expr, int var)
{
    if (expr->getId() != DEGREE)
        return false;
    return has(Degree::getDegreeOfExpression(expr.get())->getSetOfVariables(), var) &&
            !has(Degree::getArgumentOfDegree(expr.get())->getSetOfVariables(), var);

}

void setUpExpressionIntoVariable(abs_ex & func, const abs_ex &expr, int var)
{
    std::map<int, abs_ex> repl_vars;
    repl_vars.insert({var, copy(expr)});
    replaceSystemVariablesToExpressions(func, repl_vars);
    func->setSimplified(false);
    func->simplify();
    func = func->downcast();
}

void setUpExpressionIntoVariable(abs_ex &func, abs_ex &&expr, int var)
{
    std::map<int, abs_ex> repl_vars;
    repl_vars.insert({var, std::move(expr)});
    replaceSystemVariablesToExpressions(func, repl_vars);
    func->setSimplified(false);
    func->simplify();
    func = func->downcast();
}
