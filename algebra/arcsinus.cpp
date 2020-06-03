#include "arcsinus.h"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
#include "abstractexpression.h"
#include "number.h"
#include "constant.h"
#include "variablesdistributor.h"
#include "logarithm.h"
#include "exception.h"
#include <cmath>
ArcSinus::ArcSinus(const std::unique_ptr<AbstractExpression> &arg)
{
    this->argument = copy(arg);
    this->simplify();
}

ArcSinus::ArcSinus(std::unique_ptr<AbstractExpression> &&arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

ArcSinus::ArcSinus(const ArcSinus &cop)
{
    this->argument = copy(cop.argument);
}

ArcSinus::ArcSinus(ArcSinus &&mov)
{
    this->argument = std::move(mov.argument);
}

ArcSinus::~ArcSinus()
{

}

AlgebraExpression ArcSinus::getId() const
{
    return ARCSINUS;
}

void ArcSinus::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    int pos;
    pos = (this->argument - one)->getPositionRelativelyZero();
    if (pos > 0)
        throw Exception();
    pos = (this->argument + one)->getPositionRelativelyZero();
    if (pos < 0)
        throw Exception();
    this->simplified = true;
}

bool ArcSinus::operator==(AbstractExpression &right)
{
    if (right.getId() != this->getId())
        return false;
    return *this->argument == *static_cast<ArcSinus*>(&right)->argument;
}

bool ArcSinus::canDowncastTo()
{
    if (this->argument->getId() == SINUS)
    {
        auto sin_arg = getArgumentOfTrigonometricalFunction(this->argument);
        int pos = (sin_arg + getPi()/two)->getPositionRelativelyZero();
        if (!(pos > 0))
            return false;
        pos = (sin_arg - getPi()/two)->getPositionRelativelyZero();
        if (!(pos < 0))
            return false;
        return true;
    }
    abs_ex & arg = this->argument;
    if (*arg == *-sqrt(numToAbs(3)) || *arg == *numToAbs(-1) || *arg == *(-sqrt(numToAbs(3))/numToAbs(3)) || *arg == *zero || *arg == *(sqrt(three)/three) ||
            *arg == *one || *arg == *sqrt(three))
        return true;
    return false;
}

std::unique_ptr<AbstractExpression> ArcSinus::downcastTo()
{
    if (this->argument->getId() == SINUS)
        return getArgumentOfTrigonometricalFunction(std::move(this->argument));
    abs_ex & arg = this->argument;
    if (*arg == *minus_one)
        return -getPi()/two;
    if (*arg == *(-sqrt(numToAbs(3))/two))
        return -getPi()/three;
    if (*arg == *(-sqrt(two)/two))
        return -getPi()/four;
    if (*arg == *(-half))
        return -getPi()/numToAbs(6);
    if (*arg == *zero)
        return copy(zero);
    if (*arg == *half)
        return getPi()/numToAbs(6);
    if (*arg == *(sqrt(two)/two))
        return getPi()/four;
    if (*arg == *(sqrt(three)/two))
        return getPi()/three;
    if (*arg == *one)
        return getPi()/two;
    assert(false);
    return nullptr;

}

std::set<int> ArcSinus::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> ArcSinus::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> ArcSinus::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}

Number ArcSinus::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void ArcSinus::_qDebugOut()
{
    //а и нахуй это все
}

QString ArcSinus::makeStringOfExpression() const
{
    return "asin(" + this->argument->makeStringOfExpression() + ")";
}

double ArcSinus::getApproximateValue()
{
    return asin(this->argument->getApproximateValue());
}

double ArcSinus::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return asin(this->argument->getApproximateValue(choosing_value_rule));
}

int ArcSinus::getPositionRelativelyZeroIfHasVariables()
{
    return this->argument->getPositionRelativelyZero();
}

QString ArcSinus::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

std::unique_ptr<AbstractExpression> ArcSinus::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> ArcSinus::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
  //  NONCONST
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> ArcSinus::changeSomePartOnExpression(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    NONCONST
        if (this->argument->makeStringOfExpression() == part)
        {
            abs_ex cop = copy(on_what);
            this->argument.swap(cop);
            return cop;
        }
        return this->argument->changeSomePartOn(part, on_what);
    }


std::unique_ptr<AbstractExpression> ArcSinus::getArgumentsCopy()
{
    return copy(this->argument);
}

std::unique_ptr<AbstractExpression> ArcSinus::derivative(int var) const
{
    return this->argument->derivative(var) / sqrt(one - sqr(this->argument));
}

std::unique_ptr<AbstractExpression> ArcSinus::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first == nullptr)
        return nullptr;
    abs_ex& a = ln_f.first;
    return (one / ln_f.first) * (this->argument * asin(this->argument) + sqrt(one - sqr(this->argument)));
}

const std::unique_ptr<AbstractExpression> &ArcSinus::getArgument() const
{
    return this->argument;
}

void ArcSinus::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

bool ArcSinus::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == ARCSINUS);
    return AbstractExpression::less(this->argument.get(), (static_cast<ArcSinus*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}



std::unique_ptr<AbstractExpression> asin(const std::unique_ptr<AbstractExpression> &arg)
{
    return abs_ex(new ArcSinus(arg));
}

std::unique_ptr<AbstractExpression> asin(std::unique_ptr<AbstractExpression> &&arg)
{
    return abs_ex(new ArcSinus(std::move(arg)));
}
