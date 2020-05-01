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


AbstractExpression::~AbstractExpression()
{

}
bool AbstractExpression::less(const AbstractExpression * left, const AbstractExpression * right)
{
    if (left->getId() != DIFFERENTIAL && right->getId() == DIFFERENTIAL)
        return true;
    if (right->getId() == DIFFERENTIAL && left->getId() != DIFFERENTIAL)
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
bool AbstractExpression::lessToSort(const std::unique_ptr<AbstractExpression> &left, const std::unique_ptr<AbstractExpression> &right)
{
    if (left->getId() != DIFFERENTIAL && right->getId() == DIFFERENTIAL)
        return true;
    if (right->getId() == DIFFERENTIAL && left->getId() != DIFFERENTIAL)
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
std::unique_ptr<AbstractExpression> AbstractExpression::operator*(AbstractExpression &expr)
{
    //den is empty
    fractal_argument arg, den;
    arg.push_back(makeAbstractExpression(this->getId(), this));
    arg.push_back(makeAbstractExpression(expr.getId(), &expr));
    std::unique_ptr<AbstractExpression> frac = std::unique_ptr<AbstractExpression>(new Fractal(&arg, &den));
    return frac->downcast();

}

std::unique_ptr<AbstractExpression> AbstractExpression::operator/(AbstractExpression &expr)
{
    std::unique_ptr<AbstractExpression> frac = std::unique_ptr<AbstractExpression>(new Fractal(this, &expr));
    return frac->downcast();
}
std::unique_ptr<AbstractExpression> AbstractExpression::operator+(AbstractExpression & expr)
{
    std::unique_ptr<AbstractExpression> polynom = std::unique_ptr<AbstractExpression>(new Polynomial(this, &expr));

    return polynom->downcast();
}
std::unique_ptr<AbstractExpression> AbstractExpression::operator-(AbstractExpression &expr)
{
    std::unique_ptr<Fractal> subtrahend = std::unique_ptr<Fractal>(new Fractal(&expr, -1));
    std::unique_ptr<AbstractExpression> polynom = std::unique_ptr<AbstractExpression>(new Polynomial(this, subtrahend.get()));

    return polynom->downcast();
}
bool AbstractExpression::canDowncast()
{
    for (AlgebraExpression expr = AlgebraExpression(-20); expr <= 1; expr = AlgebraExpression((int(expr) + 1)))
        if (canDowncastTo(expr))
            return true;
    return false;
}
std::unique_ptr<AbstractExpression> AbstractExpression::downcast()
{
    if (!this->canDowncast())
        return makeAbstractExpression(this->getId(), this);
    std::unique_ptr<AbstractExpression> expr;
    for (AlgebraExpression exp = AlgebraExpression(-20); exp <= 1; exp = AlgebraExpression((int(exp) + 1)))
        if (canDowncastTo(exp))
        {
            expr = this->downcastTo(exp);
          //  break;
        }
    while (expr->canDowncast())
        expr = expr->downcast();
    return expr;
}
std::unique_ptr<AbstractExpression> operator*(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right)
{
    return std::move(*left.get() * *right.get());
}
std::unique_ptr<AbstractExpression> operator/(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right)
{
    return std::move(*left.get() / *right.get());
}

std::unique_ptr<AbstractExpression> operator+(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right)
{
    return std::move(*left.get() + *right.get());
}
std::unique_ptr<AbstractExpression> operator-(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right)
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


QString getStringArgumentOfTrigonometricalFunction(std::unique_ptr<AbstractExpression> &expr)
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

abs_ex getArgumentOfTrigonometricalFunction(abs_ex && expr)
{
    AbstractExpression *arg = Degree::getArgumentOfDegree(expr.get());
    assert(arg->getId() == SINUS || arg->getId() == COSINUS || arg->getId() == TANGENT || arg->getId() == COTANGENT);
    switch (arg->getId()) {
    case SINUS:
        return static_cast<Sinus*>(arg)->getArgumentMoved();
    case COSINUS:
        return static_cast<Cosinus*>(arg)->getArgumentMoved();
    case TANGENT:
        return static_cast<Tangent*>(arg)->getArgumentMoved();
    case COTANGENT:
        return static_cast<Cotangent*>(arg)->getArgumentMoved();
    default:
        assert(false);

    }
}

bool isDegreeOfTrigonometricalFunction(std::unique_ptr<AbstractExpression> &expr)
{
    auto arg = Degree::getArgumentOfDegree(expr.get())->getId();
    return arg == SINUS || arg == COSINUS || arg == TANGENT || arg == COTANGENT;
}
/*
std::vector<std::unique_ptr<AbstractExpression> > replaceEveryFunctionOnSystemVariable(std::unique_ptr<AbstractExpression> &expr)
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
std::map<int, abs_ex> replaceEveryFunctionOnSystemVariable(std::unique_ptr<AbstractExpression> &expr, std::map<QString, int> &funcs)
{
    return replaceEveryFunctionOnSystemVariable(expr.get(), funcs);
}
void replaceSystemVariablesBackToFunctions(std::unique_ptr<AbstractExpression> &expr, std::map<int, abs_ex> & funcs)
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
/*
void replaceSystemVariablesBackToFunctions(abs_ex & expr, std::vector<std::unique_ptr<AbstractExpression> > &functions)
{
    for (int i = 0; i < functions.size(); ++i)
    {
        expr->changeSomePartOn(systemVar(i).makeStringOfExpression(), functions[i]);
    }
}
*/
std::unique_ptr<AbstractExpression> getArgumentOfTrigonometricalFunction(std::unique_ptr<AbstractExpression> &expr)
{
    AbstractExpression *arg = Degree::getArgumentOfDegree(expr.get());
    assert(arg->getId() == SINUS || arg->getId() == COSINUS || arg->getId() == TANGENT || arg->getId() == COTANGENT);
    switch (arg->getId()) {
    case SINUS:
        return static_cast<Sinus*>(arg)->getArgumentsCopy();
    case COSINUS:
        return static_cast<Cosinus*>(arg)->getArgumentsCopy();
    case TANGENT:
        return static_cast<Tangent*>(arg)->getArgumentsCopy();
    case COTANGENT:
        return static_cast<Cotangent*>(arg)->getArgumentsCopy();
    default:
        assert(false);

    }
}

std::unique_ptr<AbstractExpression> absEx(int num)
{
    return abs_ex(new Number(num));
}


std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression> > checkIfItsLinearFunction(const std::unique_ptr<AbstractExpression> &func, int var)
{
    return checkIfItsLinearFunction(func.get(), var);
}

std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression> > checkIfItsLinearFunction(const AbstractExpression *func, int var)
{
    if (func->getId() == var)
        return {copy(one), copy(zero)};
    if (func->getId() == FRACTAL)
        return static_cast<const Fractal*>(func)->checkIfItIsLinearFunction(var);
    if (func->getId() == POLYNOMIAL)
        return static_cast<const Polynomial*>(func)->checkIfItIsLinearFunction(var);
    return {nullptr, nullptr};
}

std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression> > checkIfItsFunctionOfLinearArgument(const std::unique_ptr<AbstractExpression> &func, int var)
{
    return checkIfItsFunctionOfLinearArgument(func.get(), var);
}

std::unique_ptr<AbstractExpression> numToAbs(int num)
{
    return abs_ex(new Number(num));
}

std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression> > checkIfItsFunctionOfLinearArgument(const AbstractExpression *func, int var)
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

std::array<std::unique_ptr<AbstractExpression>, 3> checkIfItsQuadraticFunction(const AbstractExpression *func, int var)
{
    if (func->getId() == DEGREE && Degree::getArgumentOfDegree(const_cast<AbstractExpression*>(func))->getId() == var &&
            *Degree::getDegreeOfExpression(const_cast<AbstractExpression*>(func)) == *two)
        return {copy(one), copy(zero), copy(zero)};
        if (func->getId() == DEGREE && *Degree::getDegreeOfExpression(const_cast<AbstractExpression*>(func)) == *two)
        {
            auto ln_f = checkIfItsLinearFunction(func, var);
            return {pow(ln_f.first, 2), two*ln_f.first * ln_f.second, pow(ln_f.second, 2)};
        }
    if (func->getId() == FRACTAL)
    {
        const Fractal * fr = static_cast<const Fractal*>(func);

        auto mult = fr->getFractalWithoutVariable(var);
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
        }

    }
    auto ln_f = checkIfItsLinearFunction(func, var);
    if (ln_f.first != nullptr)
    {
        return {copy(zero), std::move(ln_f.first), std::move(ln_f.second)};
    }
    if (func->getId() != POLYNOMIAL)
        return {nullptr, nullptr, nullptr};
    return static_cast<const Polynomial*>(func)->checkQuadraticFunction(var);
}

std::array<std::unique_ptr<AbstractExpression>, 3> checkIfItsQuadraticFunction(const std::unique_ptr<AbstractExpression> &func, int var)
{
    return checkIfItsQuadraticFunction(func.get(), var);
}

bool isZero(const std::unique_ptr<AbstractExpression> &expr)
{
    return expr->getId() == NUMBER && static_cast<const Number*>(expr.get())->isZero();
}

std::unique_ptr<AbstractExpression> operator-(const std::unique_ptr<AbstractExpression> &arg)
{
    return minus_one * arg;
}

std::unique_ptr<AbstractExpression> operator-(std::unique_ptr<AbstractExpression> &&arg)
{
    return minus_one * std::move(arg);
}
