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
#include "fractal.h"
ArcSinus::ArcSinus(const abs_ex &arg)
{
    this->argument = copy(arg);
    this->simplify();
}

ArcSinus::ArcSinus(abs_ex &&arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

ArcSinus::ArcSinus(const ArcSinus &cop)
{
    this->argument = copy(cop.argument);
    this->simplified = cop.simplified;
}

ArcSinus::ArcSinus(ArcSinus &&mov)
{
    this->argument = std::move(mov.argument);
    this->simplified = mov.simplified;
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
    return subCompare(this->argument, static_cast<ArcSinus*>(&right)->argument);
}

bool ArcSinus::canDowncastTo()
{
    if (this->argument->getId() == SINUS)
    {
        auto sin_arg = getArgumentOfFunction(this->argument);
        int pos = (sin_arg + getPi()/two)->getPositionRelativelyZero();
        if (!(pos > 0))
            return false;
        pos = (sin_arg - getPi()/two)->getPositionRelativelyZero();
        if (!(pos < 0))
            return false;
        return true;
    }
    if (this->argument->getId() == COSINUS)
    {
        auto cos_arg = getArgumentOfFunction(this->argument);
        int pos = cos_arg->getPositionRelativelyZero();
        if (!(pos > 0))
            return false;
        pos = (cos_arg - getPi())->getPositionRelativelyZero();
        if (!(pos < 0))
            return false;
        return true;
    }

    abs_ex & arg = this->argument;
    if (*arg == *minus_one || *arg == *(-sqrt(three)/two) || *arg == *-half || *arg == *zero || *arg == *(-sqrt(two)/two)||
             *arg == *half || *arg == *(-sqrt(two)/two) || *arg == *(sqrt(three)/two) || *arg == *one)
        return true;
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
        return true;
    return false;
}

abs_ex ArcSinus::downcastTo()
{
    if (this->argument->getId() == SINUS)
        return getArgumentOfFunction(std::move(this->argument));
    if (this->argument->getId() == COSINUS)
        return getPi()/two - getArgumentOfFunction(std::move(this->argument));
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
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
    {
        return integratingConstantExpr(this->argument->getId(), this->getRange());
    }
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

QString ArcSinus::makeWolframString() const
{
    return "ArcSin[" + this->argument->makeWolframString() + "]";
}

QString ArcSinus::toString() const
{
    return "asin(" + this->argument->toString() + ")";
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

abs_ex ArcSinus::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex ArcSinus::changeSomePartOn(QString part, abs_ex &on_what)
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

abs_ex ArcSinus::changeSomePartOnExpression(QString part, abs_ex &on_what)
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


abs_ex ArcSinus::getArgumentsCopy()
{
    return copy(this->argument);
}

abs_ex ArcSinus::derivative(int var) const
{
    return this->argument->derivative(var) / sqrt(one - sqr(this->argument));
}

abs_ex ArcSinus::antiderivative(int var) const
{

    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    if (isSqrt(argument))
    {
        auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(argument.get()), var);
        auto arg = copy(Degree::getArgumentOfDegree(argument.get()));
        if (ln_f.first != nullptr)
        {
            auto a = std::move(ln_f.first);
            auto b = std::move(ln_f.second);
            auto x = getVariableExpr(var);
            return x *asin(argument) -
                    (b*(two*a*x - one ) + (one - two*b)*sqrt(arg - one)*sqrt(arg) *ln(sqrt(arg - one) + sqrt(arg)) +
                     a*x*(a*x - one) +b*b)/(two*a*sqrt(-(arg - one)*arg));
        }
    }

    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first == nullptr)
        return nullptr;
    abs_ex& a = ln_f.first;
    return (one / ln_f.first) * (this->argument * asin(this->argument) + sqrt(one - sqr(this->argument)));
}

const abs_ex &ArcSinus::getArgument() const
{
    return this->argument;
}

void ArcSinus::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > ArcSinus::getTrigonometricalFunctions() const
{
    return this->argument->getTrigonometricalFunctions();
}

long long ArcSinus::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long ArcSinus::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange ArcSinus::getRange() const
{
    FunctionRange arg_range = this->argument->getRange();
    if (arg_range.isError())
        return arg_range;
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (lower(it.min(), minus_one))
        {
            if (*it.max() == *minus_one && it.isMaxIncluded())
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(-getPi()/two, -getPi()/two, true, true));
            else if ((it.max() - minus_one)->getPositionRelativelyZero() > 0)
            {
                if (lowerOrEquall(it.max(), one))
                    result.addSegmentWithoutNormilizing(FunctionRangeSegment(-getPi()/two, asin(it.max()), true, it.isMaxIncluded()));
                else
                    result.addSegmentWithoutNormilizing(FunctionRangeSegment(-getPi()/two, getPi()/two, true, true));
            }

        }
        else if (lower(it.min(), one))
        {
            if (lowerOrEquall(it.max(), one))
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(asin(it.min()), asin(it.max()),
                                                    it.isMinIncluded(), it.isMaxIncluded()));
            else
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(asin(it.min()),  getPi()/two,
                                                                         it.isMinIncluded(), true));
        }
        else if (*it.min() == *one && it.isMinIncluded())
        {
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(getPi()/two, getPi()/two, true, true));
        }
    }
    return result;
}

bool ArcSinus::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool ArcSinus::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    if (second->getId() == ARCSINUS)
    {
        auto arg = getArgumentOfFunction(second);
        if (canBeConsideredAsConstantThatCanBeChanged(argument) && canBeConsideredAsConstantThatCanBeChanged(arg))
        {
            this->argument = integratingConstantExpr(unification(argument->getRange(), arg->getRange()));
            return true;
        }
        return argument->tryToMergeIdenticalBehindConstantExpressions(getArgumentOfFunction(second));
    }
    return false;
}

abs_ex ArcSinus::tryToFindExponentialFunction(int var) const
{
    return this->argument->tryToFindExponentialFunction(var);
}

void ArcSinus::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

bool ArcSinus::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == ARCSINUS);
    return AbstractExpression::less(this->argument.get(), (static_cast<ArcSinus*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}



abs_ex asin(const abs_ex &arg)
{
    return abs_ex(new ArcSinus(arg))->downcast();
}

abs_ex asin(abs_ex &&arg)
{
    return abs_ex(new ArcSinus(std::move(arg)))->downcast();
}

abs_ex acos(const abs_ex &arg)
{
    return getPi()/two - asin(arg);
}

abs_ex acos(abs_ex &&arg)
{
    return getPi()/two - asin(std::move(arg));
}
