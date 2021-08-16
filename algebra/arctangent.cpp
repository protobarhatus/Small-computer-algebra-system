#include "arctangent.h"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
#include "abstractexpression.h"
#include "number.h"
#include "constant.h"
#include "variablesdistributor.h"
#include "logarithm.h"
#include <cmath>
#include "fractal.h"
ArcTangent::ArcTangent(const abs_ex &arg)
{
    this->argument = copy(arg);
    this->simplify();
}

ArcTangent::ArcTangent(abs_ex &&arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

ArcTangent::ArcTangent(const ArcTangent &cop)
{
    this->argument = copy(cop.argument);
    this->simplified = cop.simplified;
}

ArcTangent::ArcTangent(ArcTangent &&mov)
{
    this->argument = std::move(mov.argument);
    this->simplified = mov.simplified;
}

ArcTangent::~ArcTangent()
{

}

AlgebraExpression ArcTangent::getId() const
{
    return ARCTANGENT;
}

void ArcTangent::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    this->simplified = true;
}

bool ArcTangent::operator==(AbstractExpression &right)
{
    if (right.getId() != this->getId())
        return false;
    return subCompare(this->argument, static_cast<ArcTangent*>(&right)->argument);
}

bool ArcTangent::canDowncastTo()
{
    if (this->argument->getId() == TANGENT)
    {
        auto tan_arg = getArgumentOfFunction(this->argument);
        int pos = (tan_arg + getPi()/two)->getPositionRelativelyZero();
        if (!(pos > 0))
            return false;
        pos = (tan_arg - getPi()/two)->getPositionRelativelyZero();
        if (!(pos < 0))
            return false;
        return true;
    }
    if (this->argument->getId() == COTANGENT)
    {
        auto cot_arg = getArgumentOfFunction(this->argument);
        int pos = cot_arg->getPositionRelativelyZero();
        if (!(pos > 0))
            return false;
        pos = (cot_arg - getPi())->getPositionRelativelyZero();
        if (!(pos < 0))
            return false;
        return true;
    }

    abs_ex & arg = this->argument;
    if (*arg == *-sqrt(numToAbs(3)) || *arg == *numToAbs(-1) || *arg == *(-sqrt(numToAbs(3))/numToAbs(3)) || *arg == *zero || *arg == *(sqrt(three)/three) ||
            *arg == *one || *arg == *sqrt(three))
        return true;
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
        return true;
    if (isKindOfInf(arg))
        return true;
    return false;
}

abs_ex ArcTangent::downcastTo()
{
    if (this->argument->getId() == TANGENT)
        return getArgumentOfFunction(std::move(this->argument));
    if (this->argument->getId() == COTANGENT)
        return getPi()/two - getArgumentOfFunction(std::move(this->argument));
    abs_ex & arg = this->argument;
    if (*arg == *-sqrt(numToAbs(3)))
        return -getPi()/three;
    if (*arg == *numToAbs(-1))
        return -getPi()/four;
    if (*arg == *(-sqrt(numToAbs(3))/numToAbs(3)))
        return -getPi()/numToAbs(6);
    if (*arg == *zero)
        return copy(zero);
    if (*arg == *(sqrt(three)/three))
        return getPi()/numToAbs(6);
    if (*arg == *one)
        return getPi()/four;
    if (*arg == *sqrt(three))
        return getPi()/three;
    if (isIntegratingConstantAndCanChangeIt(argument->getId()))
    {
        return integratingConstantExpr(this->argument->getId(), this->getRange());
    }
    if (isInf(argument))
        return getPi()/two;
    if (isMinusInf(argument))
        return -getPi()/two;
    return nullptr;

}

std::set<int> ArcTangent::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> ArcTangent::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> ArcTangent::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}

Number ArcTangent::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void ArcTangent::_qDebugOut()
{
    //а и нахуй это все
}

QString ArcTangent::makeStringOfExpression() const
{
    return "atan(" + this->argument->makeStringOfExpression() + ")";
}

QString ArcTangent::makeWolframString() const
{
    return "ArcTan[" + this->argument->makeWolframString() + "]";
}

QString ArcTangent::toString() const
{
    return "atan(" + this->argument->toString() + ")";
}

double ArcTangent::getApproximateValue()
{
    return atan(this->argument->getApproximateValue());
}

double ArcTangent::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return atan(this->argument->getApproximateValue(choosing_value_rule));
}

int ArcTangent::getPositionRelativelyZeroIfHasVariables()
{
    return this->argument->getPositionRelativelyZero();
}

QString ArcTangent::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

abs_ex ArcTangent::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex ArcTangent::changeSomePartOn(QString part, const abs_ex &on_what)
{
    //NONCONST
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

abs_ex ArcTangent::changeSomePartOnExpression(QString part, const abs_ex &on_what)
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

abs_ex ArcTangent::getArgumentsCopy()
{
    return copy(this->argument);
}

abs_ex ArcTangent::derivative(int var) const
{
    return this->argument->derivative(var) / (sqr(this->argument) + one);
}

abs_ex ArcTangent::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    if (isSqrt(argument))
    {
        auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(argument.get()), var);
        auto arg = Degree::getArgumentOfDegree(argument.get());
        if (ln_f.first != nullptr)
            return ((copy(arg) + one)*atan(argument) - argument) /ln_f.first;
    }
    if (argument->getId() == FRACTAL)
    {
        auto c = toAbsEx(static_cast<Fractal*>(argument.get())->getFractalWithoutVariable(var));
        auto log = argument / c;
        if (isSqrt(log))
        {
            auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(log.get()), var);
            auto arg = Degree::getArgumentOfDegree(log.get());
            if (ln_f.first != nullptr)
                return ((pow(c, 2)*copy(arg) + one) *atan(argument) - argument)/ln_f.first/pow(c, 2);
        }
    }
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first != nullptr)
    {
        abs_ex& a = ln_f.first;
        return -(ln(sqr(this->argument) + one) - two * this->argument * atan(this->argument))/two/a;
    }
    int lcm_of_denoms = this->getLcmOfDenominatorsOfDegreesOfVariable(var);
    if (lcm_of_denoms > 1)
    {
        abs_ex t;
        if (lcm_of_denoms % 2 == 0)
            t = systemVarExpr(zero, nullptr, true, false);
        else
            t = systemVarExpr();
        abs_ex cop = copy(this);
        setUpExpressionIntoVariable(cop, pow(t, lcm_of_denoms), var);
        cop = std::move(cop) * pow(t, lcm_of_denoms)->derivative(t->getId());
       // qDebug() << cop->toString();
        auto integr = cop->antiderivative(t->getId());

        if (integr != nullptr)
        {
            setUpExpressionIntoVariable(integr, pow(getVariableExpr(var), one/numToAbs(lcm_of_denoms)), t->getId());
            return integr;
        }

    }
    auto ln_deg = checkIfItsDegreeOfLinearFunction(argument, var);
    if (ln_deg.first != nullptr)
    {
        abs_ex t = systemVarExpr();
        abs_ex cop = copy(this);
        setUpExpressionIntoVariable(cop, (t - ln_deg.second) / ln_deg.first, var);
        auto integr = cop->antiderivative(t->getId());
        if (integr != nullptr)
        {
            setUpExpressionIntoVariable(integr, ln_deg.first * getVariableExpr(var) + ln_deg.second, t->getId());
            return integr / ln_deg.first;
        }
        return nullptr;
    }
    return nullptr;
}

const abs_ex &ArcTangent::getArgument() const
{
    return this->argument;
}

void ArcTangent::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > ArcTangent::getTrigonometricalFunctions() const
{
    return this->argument->getTrigonometricalFunctions();
}

long long ArcTangent::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long ArcTangent::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange ArcTangent::getRange() const
{
    FunctionRange arg_range = this->getRange();
    if (arg_range.isError())
        return arg_range;
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        result.addSegmentWithoutNormilizing(FunctionRangeSegment(atan(it.min()), atan(it.max()),
                                                                 it.isMinIncluded(), it.isMaxIncluded()));
    }
    return result;
}

bool ArcTangent::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool ArcTangent::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    if (second->getId() == ARCTANGENT)
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

abs_ex ArcTangent::tryToFindExponentialFunction(int var) const
{
    return this->argument->tryToFindExponentialFunction(var);
}

void ArcTangent::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

void ArcTangent::doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const
{
    this->argument->doSomethingInDerivativeObject(func);
}

bool ArcTangent::canBeZero() const
{
    return this->argument->canBeZero();
}

bool ArcTangent::hasUndefinedVariable() const
{
    return this->argument->hasUndefinedVariable();
}

bool ArcTangent::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == ARCTANGENT);
    return AbstractExpression::less(this->argument.get(), (static_cast<ArcTangent*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}


abs_ex atan(const abs_ex &arg)
{
    return abs_ex(new ArcTangent(arg))->downcast();
}

abs_ex atan(abs_ex &&arg)
{
    return abs_ex(new ArcTangent(std::move(arg)))->downcast();
}

abs_ex acot(const abs_ex &arg)
{
    return getPi()/two - atan(arg);
}

abs_ex acot(abs_ex &&arg)
{
    return getPi()/two - atan(arg);
}
