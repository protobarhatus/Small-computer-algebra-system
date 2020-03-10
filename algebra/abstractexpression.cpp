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
AbstractExpression::AbstractExpression()
{

}


AbstractExpression::~AbstractExpression()
{

}
bool AbstractExpression::less(const AbstractExpression * left, const AbstractExpression * right)
{
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
            expr = this->downcastTo(exp);
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
}

void replaceSystemVariablesBackToFunctions(abs_ex & expr, std::vector<std::unique_ptr<AbstractExpression> > &functions)
{
    for (int i = 0; i < functions.size(); ++i)
    {
        expr->changeSomePartOn(systemVar(i).makeStringOfExpression(), functions[i]);
    }
}
