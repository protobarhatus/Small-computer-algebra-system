#include "cotangent.h"
#include "some_algebra_expression_conversions.h"
#include "sinus.h"
#include <assert.h>
#include "number.h"
#include <QDebug>
#include "cmath"
#include "fractal.h"
#include "constant.h"
#include "degree.h"
#include "polynomial.h"
#include "exception.h"
#include "absolutevalue.h"
#include "logarithm.h"
#include "variablesdistributor.h"
#include "tangent.h"
Cotangent::Cotangent(const abs_ex & iargument)
{
    this->argument = copy(iargument);
    this->simplify();
}
Cotangent::Cotangent(abs_ex && arg)
{
    this->argument.swap(arg);
    this->simplify();
}
Cotangent::Cotangent(const Cotangent & cop)
{
    this->argument = copy(cop.argument);
    this->simplified = cop.simplified;
    if (cop.pi_member != nullptr)
        this->pi_member = std::unique_ptr<Fractal>(new Fractal(*cop.pi_member));
    this->is_pi_member_only = cop.is_pi_member_only;
    this->simplify();
}
Cotangent::Cotangent(Cotangent && mov)
{
    this->argument = std::move(mov.argument);
    this->simplified = mov.simplified;
    if (mov.pi_member != nullptr)
        this->pi_member = std::unique_ptr<Fractal>(new Fractal(*mov.pi_member));
    this->is_pi_member_only = mov.is_pi_member_only;
    this->simplify();
}

Cotangent::~Cotangent()
{

}

AlgebraExpression Cotangent::getId() const
{
    return COTANGENT;
}

void Cotangent::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    if (this->argument->getId() == FRACTAL)
    {
        Fractal * fr = static_cast<Fractal*>(this->argument.get());
        if (fr->canTurnIntoPolynomWithOpeningParentheses())
            this->argument = abs_ex(fr->turnIntoPolynomWithOpeningParentheses(false).release());
        else
            fr->intoAcceptedSignForm();
        if (fr->getCoefficient().compareWith(0) < 0)
        {
            this->simplified = true;
            return;
        }
    }
    if (this->argument->getId() == POLYNOMIAL)
    {
        Polynomial * pol = static_cast<Polynomial*>(this->argument.get());
        if (pol->isNeedToChangeSignToLeadTheAcceptedForm())
        {
            pol->changeSign();
            //после скобки нельзя раскрывать, поэтому выходим. приведение периодов, выделение пи-члена будет после downcast-а
            this->argument = abs_ex(new Number(-1)) * this->argument;
            this->simplified = true;
            return;
        }
    }
    auto pi_mem = getPiMember(this->argument);
    this->pi_member = std::move(pi_mem.second);
    this->is_pi_member_only = pi_mem.first;
    if (this->pi_member != nullptr)
    {
        Number coe = this->pi_member->getCoefficient();

        if (coe.compareWith(1) > 0 || coe.compareWith(0) < 0 || coe==1)
        {
            Number to_sub = 0;
            to_sub = ((coe.getNumerator() / coe.getDenominator()));
            if (coe.compareWith(0) < 0 && !(coe.getDenominator() == 1 && coe.getNumerator() % 1 == 0))
                to_sub -= 1;
            this->pi_member = this->pi_member - std::unique_ptr<Fractal>(new Fractal(getPi(), to_sub));
            this->argument = this->argument - abs_ex(new Fractal(getPi(), to_sub));
        }
        if (this->is_pi_member_only && coe == Number(0))
            throw Exception();
    }
    this->simplified = true;
}

bool Cotangent::operator==(AbstractExpression &right)
{
    if (right.getId() != COTANGENT)
        return false;
    return subCompare(this->argument, static_cast<Cotangent*>(&right)->argument);
}

bool Cotangent::canDowncastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
        return true;
    if (pi_member == nullptr)
        return false;
  /*  if (expr == NUMBER)
    {
        //k*pi или k/2 * pi
        if (pi_member->getCoefficient().getDenominator() <= 2)
            return true;
        return false;
    }*/
    if ( this->pi_member != nullptr && !this->is_pi_member_only && isPiMemberOnAxis(pi_member->getCoefficient()))
        return true;
    if (this->is_pi_member_only)
    {
        if (pi_member->getCoefficient().getDenominator() <= 6 && pi_member->getCoefficient().getDenominator() != 5)
            return true;
        return false;
    }
    if (this->argument->getPositionRelativelyZero() < 0)
        return true;
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
        return true;
    if (this->argument->getId() == ARCTANGENT)
        return true;
    return false;
}

abs_ex Cotangent::downcastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
    {
        return abs_ex(new Number(-1)) * abs_ex(new Cotangent(abs_ex(new Number(-1)) * this->argument));
    }
    if (this->is_pi_member_only)
    {
        Number coe = this->pi_member->getCoefficient();
        if (coe == Number(1, 2))
            return abs_ex(new Number(0));
        if (coe == Number(1, 3))
            return abs_ex(abs_ex(new Number(1)) / takeDegreeOf(Number(3), Number(1, 2)));
        if (coe == Number(2, 3))
            return abs_ex(abs_ex(new Number(-1)) / takeDegreeOf(Number(3), Number(1, 2)));
        if (coe == Number(1, 4))
            return abs_ex(new Number(1));
        if (coe == Number(3, 4))
            return abs_ex(new Number(-1));
        if (coe == Number(1, 6))
            return takeDegreeOf(Number(3), Number(1, 2));
        if (coe == Number(5, 6))
            return takeDegreeOf(Number(3), Number(1, 2)) * abs_ex(new Number(-1));
    }
    if (this->pi_member->getCoefficient() == Number(1, 2) || this->pi_member->getCoefficient() == Number(-1, 2))
        return -tan(argument - toAbsEx(pi_member));
    if (this->pi_member != nullptr && isPiMemberOnAxis(pi_member->getCoefficient()))
    {
        auto left = this->argument - abs_ex(new Fractal(this->pi_member.get()));
        return (cot(left)*cot(toAbsEx(pi_member)) - one)/(cot(left) + cot(toAbsEx(pi_member)));
    }
    /*if (this->pi_member != nullptr && abs(this->pi_member->getCoefficient()) == Number(1, 2))
    {
        return minus_one * cot(this->argument - toAbsEx(this->pi_member));
    }
    if ( this->pi_member != nullptr && isPiMemberOnAxis(pi_member->getCoefficient()))
    {
        auto left = this->argument - abs_ex(new Fractal(this->pi_member.get()));
        return (tan(left) + tan(toAbsEx(this->pi_member)))/(one - tan(left)*tan(toAbsEx(this->pi_member)));
    }*/
    if (this->argument->getPositionRelativelyZero() < 0)
        return -cot(-argument);
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
    {
        return integratingConstantExpr(this->argument->getId(), this->getRange());
    }
    if (this->argument->getId() == ARCTANGENT)
        return one/getArgumentOfFunction(this->argument);
    return abs_ex(nullptr);
}

std::set<int> Cotangent::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> Cotangent::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Cotangent::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}

Number Cotangent::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void Cotangent::_qDebugOut()
{
    qDebug() << "Cotangent:";
    this->argument->_qDebugOut();
    qDebug() << "End of sinus;";
}

QString Cotangent::makeStringOfExpression() const
{
    return "cot(" + this->argument->makeStringOfExpression() + ")";
}

QString Cotangent::makeWolframString() const
{
    return "Cot[" + this->argument->makeWolframString() + "]";
}

QString Cotangent::toString() const
{
    return "cot(" + this->argument->toString() + ")";
}

double Cotangent::getApproximateValue()
{
    return tan(this->argument->getApproximateValue());
}

double Cotangent::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return tan(this->argument->getApproximateValue(choosing_value_rule));
}

int Cotangent::getPositionRelativelyZeroIfHasVariables()
{
    int z_pos = this->argument->getPositionRelativelyZero();
    if (z_pos > 0)
    {
        int pi_half_pos = (this->argument - getPi() * abs_ex(new Number(1,2)))->getPositionRelativelyZero();
        if (pi_half_pos < 0)
            return 1;
        if (pi_half_pos > 0 && (this->argument - getPi())->getPositionRelativelyZero() < 0)
            return -1;
        return 0;
    }
    if (z_pos < 0)
    {
        int minus_pi_half_pos = (this->argument - getPi() * abs_ex(new Number(-1, 2)))->getPositionRelativelyZero();
        if (minus_pi_half_pos > 0)
            return -1;
        if (minus_pi_half_pos < 0 && (this->argument +  getPi())->getPositionRelativelyZero() > 0)
            return 1;
        return 0;
    }
    return 0;
}

QString Cotangent::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

abs_ex Cotangent::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex Cotangent::changeSomePartOn(QString part, abs_ex &on_what)
{
   // NONCONST
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

abs_ex Cotangent::changeSomePartOnExpression(QString part, abs_ex &on_what)
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

abs_ex Cotangent::getArgumentsCopy()
{
    return copy(this->argument);
}

abs_ex Cotangent::derivative(int var) const
{
    return minus_one / takeDegreeOf(sin(this->argument), 2) * this->argument->derivative(var);
}

abs_ex Cotangent::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first == nullptr)
        return nullptr;
    return one/ln_f.first * ln(abs(sin(this->argument)));
}

const abs_ex &Cotangent::getArgument() const
{
    return this->argument;
}

void Cotangent::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > Cotangent::getTrigonometricalFunctions() const
{
    std::set<abs_ex> set;
    set.insert(cot(argument));
    return set;
}

long long Cotangent::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long Cotangent::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange Cotangent::getRange() const
{
    FunctionRange arg_range = argument->getRange();


    if (arg_range.isError())
        return arg_range;
    if (arg_range.getMin() == nullptr || arg_range.getMax() == nullptr)
        return FunctionRange(FunctionRangeSegment(nullptr, nullptr, false, false));
    FunctionRange result;
    auto toFirstPeriod = [](const abs_ex & arg)->abs_ex
    {
        abs_ex div_res = arg / getPi();
        if (div_res->getId() == NUMBER)
            return getPi()*abs_ex(new Number(((static_cast<Number*>(div_res.get())->getNumerator() > 0 ? 1 : -1)), static_cast<Number*>(div_res.get())->getDenominator()));
        int rat = div_res->getApproximateValue();
        return arg - numToAbs(rat)*getPi();
    };
    for (auto &it : arg_range.getSegments())
    {
        if (biggerOrEquall(it.max() - it.min(), getPi()))
            return FunctionRange(FunctionRangeSegment(nullptr, nullptr, false, false));

        abs_ex max = toFirstPeriod(it.max());
        abs_ex min = toFirstPeriod(it.min());
        if (lower(max, zero))
            max = max + getPi();
        if (lower(min, zero))
            min = min + getPi();
        if (bigger(min, max))
        {
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(nullptr, cot(min), false, it.isMinIncluded()));
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(cot(max), nullptr, it.isMaxIncluded(), false));
        }
        else
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(cot(max), cot(min), it.isMaxIncluded(), it.isMinIncluded()));

    }
    result.normalize();
    return result;
}

bool Cotangent::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool Cotangent::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    if (second->getId() == this->getId())
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

abs_ex Cotangent::tryToFindExponentialFunction(int var) const
{
    return this->argument->tryToFindExponentialFunction(var);
}

void Cotangent::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

bool Cotangent::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == COTANGENT);
    return AbstractExpression::less(this->argument.get(), (static_cast<Cotangent*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}

abs_ex cot(const abs_ex &expr)
{
    return abs_ex(new Cotangent(expr))->downcast();
}

abs_ex cot(abs_ex &&expr)
{
    return abs_ex(new Cotangent(std::move(expr)))->downcast();
}
