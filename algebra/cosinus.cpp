#include "cosinus.h"
#include "some_algebra_expression_conversions.h"
#include <assert.h>
#include "number.h"
#include <QDebug>
#include "cmath"
#include "fractal.h"
#include "constant.h"
#include "degree.h"
#include "polynomial.h"
#include "sinus.h"
#include "variablesdistributor.h"
#include "absolutevalue.h"
Cosinus::Cosinus(const abs_ex & iargument)
{
    this->argument = copy(iargument);
    this->simplify();
}
Cosinus::Cosinus(abs_ex && arg)
{
    this->argument.swap(arg);
    this->simplify();
}
Cosinus::Cosinus(const Cosinus & cop)
{
    this->argument = copy(cop.argument);
    this->simplified = cop.simplified;
    if (cop.pi_member != nullptr)
        this->pi_member = std::unique_ptr<Fractal>(new Fractal(*cop.pi_member));
    this->is_pi_member_only = cop.is_pi_member_only;
    this->simplify();
}
Cosinus::Cosinus(Cosinus && mov)
{
    this->argument = std::move(mov.argument);
    this->simplified = mov.simplified;
    if (mov.pi_member != nullptr)
        this->pi_member = std::unique_ptr<Fractal>(new Fractal(*mov.pi_member));
    this->is_pi_member_only = mov.is_pi_member_only;
    this->simplify();
}
void Cosinus::simplify()
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
        {
            fr->intoAcceptedSignForm();
            if (fr->getCoefficient().compareWith(0) < 0)
            {
                fr->setCoefficinet(fr->getCoefficient() * -1);
            }
        }
    }
    if (this->argument->getId() == POLYNOMIAL)
    {
        Polynomial * pol = static_cast<Polynomial*>(this->argument.get());
        if (pol->isNeedToChangeSignToLeadTheAcceptedForm())
        {
            pol->changeSign();
            return;
        }
    }
    auto pi_mem = getPiMember(this->argument);
    this->pi_member = std::move(pi_mem.second);
    this->is_pi_member_only = pi_mem.first;
    if (this->pi_member != nullptr)
    {
        Number coe = this->pi_member->getCoefficient();

        if (coe.compareWith(2) > 0 || coe == 2 || coe.compareWith(0) < 0)
        {
            Number to_sub = 0;
            to_sub = 2 * ((coe.getNumerator() / coe.getDenominator()) / 2);
            if (coe.compareWith(0) < 0 && !(coe.getDenominator() == 1 && coe.getNumerator() % 2 == 0))
                to_sub -= 2;
            this->pi_member = this->pi_member - std::unique_ptr<Fractal>(new Fractal(getPi(), to_sub));
            this->argument = this->argument - abs_ex(new Fractal(getPi(), to_sub));
        }

    }
    this->simplified = true;
}
bool Cosinus::operator==(AbstractExpression &right)
{
    if (right.getId() != COSINUS)
        return false;
    return subCompare(this->argument, static_cast<Cosinus*>(&right)->argument);
}
bool Cosinus::canDowncastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
        return true;
   // if (pi_member == nullptr)
     //   return false;
  /*  if (expr == NUMBER)
    {
        //k*pi или k/2 * pi
        if (pi_member->getCoefficient().getDenominator() <= 2)
            return true;
        return false;
    }*/
    if (this->is_pi_member_only)
    {
        if (pi_member->getCoefficient().getDenominator() <= 6 && pi_member->getCoefficient().getDenominator() != 5)
            return true;
        return false;
    }
    if (this->pi_member != nullptr && !this->is_pi_member_only && isPiMemberOnAxis(pi_member->getCoefficient()))
        return true;
    //if (this->argument->getId() == POLYNOMIAL && static_cast<Polynomial*>(this->argument.get())->getMonomialsPointers().size() == 2)
    //    return true;
    if (this->argument->getPositionRelativelyZero() < 0)
        return true;
    if (isIntegratingConstantAndCanChangeIt(argument->getId()))
        return true;
    if (this->argument->getId() == ARCSINUS)
        return true;
    return false;
}
abs_ex Cosinus::downcastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
    {
        return abs_ex(new Number(-1)) * abs_ex(new Cosinus(abs_ex(new Number(-1)) * this->argument));
    }
    if (this->is_pi_member_only)
    {
        Number coe = this->pi_member->getCoefficient();
        if (coe == Number(1, 2) || coe == Number(3, 2))
            return abs_ex(new Number(0));
        if (coe == 0)
            return abs_ex(new Number(1));
        if (coe == 1)
            return abs_ex(new Number(-1));
        if (coe == Number(1, 3) || coe == Number(5, 3))
            return abs_ex(new Number(1, 2));
        if (coe == Number(2, 3) || coe == Number(4, 3))
            return abs_ex(new Number(-1, 2));
        if (coe == Number(1, 4) || coe == Number(7, 4))
            return takeDegreeOf(Number(2), Number(1, 2)) * abs_ex(new Number(1, 2));
        if (coe == Number(5, 4) || coe == Number(3, 4))
            return takeDegreeOf(Number(2), Number(1, 2)) * abs_ex(new Number(-1, 2));
        if (coe == Number(1, 6) || coe == Number(11, 6))
            return takeDegreeOf(Number(3), Number(1, 2)) * abs_ex(new Number(1, 2));
        if (coe == Number(5, 6) || coe == Number(7, 6))
            return takeDegreeOf(Number(3), Number(1, 2)) * abs_ex(new Number(-1, 2));
    }
    if (this->pi_member != nullptr && isPiMemberOnAxis(pi_member->getCoefficient()))
    {
        auto left = this->argument - abs_ex(new Fractal(this->pi_member.get()));
        return cos(left)*cos(toAbsEx(this->pi_member)) - sin(toAbsEx(this->pi_member))*sin(left);
    }

    if (this->argument->getPositionRelativelyZero() < 0)
        return cos(-argument);
    if (isIntegratingConstantAndCanChangeIt(argument->getId()))
    {
        return integratingConstantExpr(this->argument->getId(), this->getRange());
    }
    if (this->argument->getId() == ARCSINUS)
        return sqrt(one - pow(getArgumentOfFunction(this->argument), 2));
    return abs_ex(nullptr);
}
std::set<int> Cosinus::getSetOfPolyVariables() const
{
    return std::set<int>();
}
std::set<int> Cosinus::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Cosinus::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}
Number Cosinus::getMaxDegreeOfVariable(int id)
{
    //return this->argument->getMaxDegreeOfVariable(id);
    return Number::makeErrorNumber();
}
void Cosinus::_qDebugOut()
{
    qDebug() << "Cosinus:";
    this->argument->_qDebugOut();
    qDebug() << "End of Cosinus;";
}
QString Cosinus::makeStringOfExpression() const
{
    return "cos(" + this->argument->makeStringOfExpression() + ")";
}

QString Cosinus::makeWolframString() const
{
    return "Cos[" + this->argument->makeWolframString() + "]";
}

QString Cosinus::toString() const
{
    return "cos(" + this->argument->makeStringOfExpression() + ")";
}
double Cosinus::getApproximateValue()
{
    return cos(this->argument->getApproximateValue());
}
double Cosinus::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return cos(this->argument->getApproximateValue(choosing_value_rule));
}
int Cosinus::getPositionRelativelyZeroIfHasVariables()
{
    int pi_half_pos = (this->argument - getPi() / abs_ex(new Number(2)))->getPositionRelativelyZero();
    if (pi_half_pos > 0)
    {
        int three_pi_half_pos = (this->argument - getPi() * abs_ex(new Number(3, 2)))->getPositionRelativelyZero();
        if (three_pi_half_pos < 0)
            return -1;
        if (three_pi_half_pos > 0)
            if ((this->argument - getPi() * abs_ex(new Number(5, 2)))->getPositionRelativelyZero() < 0)
                return 1;
        return 0;
    }
    if (pi_half_pos < 0)
    {
        int minus_pi_half_pos = (this->argument + getPi() / abs_ex(new Number(2)))->getPositionRelativelyZero();
        if (minus_pi_half_pos > 0)
            return 1;
        if ((this->argument + getPi() * abs_ex(new Number(3, 2)))->getPositionRelativelyZero() > 0)
            return -1;
        return 0;
    }
    return 0;
}
bool Cosinus::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == COSINUS);
    return AbstractExpression::less(this->argument.get(), (static_cast<Cosinus*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}
Cosinus::~Cosinus()
{

}

AlgebraExpression Cosinus::getId() const
{
    return COSINUS;
}
QString Cosinus::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

abs_ex Cosinus::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex Cosinus::changeSomePartOn(QString part, abs_ex &on_what)
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

abs_ex Cosinus::changeSomePartOnExpression(QString part, abs_ex &on_what)
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

abs_ex Cosinus::getArgumentsCopy()
{
    return copy(argument);
}

abs_ex Cosinus::derivative(int var) const
{
    return minus_one * sin(this->argument) * this->argument->derivative(var);
}

abs_ex Cosinus::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    if (isSqrt(argument))
    {
        auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(argument.get()), var);
        if (ln_f.first != nullptr)
            return two*(sin(argument)*argument +  cos(argument))/ln_f.first;
    }
    if (argument->getId() == LOGARITHM)
    {
        auto ln_f = checkIfItsFunctionOfLinearArgument(argument, var);
        auto arg = getArgumentOfFunction(argument);
        if (ln_f.first != nullptr)
            return arg*(cos(argument) + sin(argument)) /two/ln_f.first;
    }
    if (argument->getId() == FRACTAL)
    {
        auto c = toAbsEx(static_cast<Fractal*>(argument.get())->getFractalWithoutVariable(var));
        auto log = argument / c;
        if (log->getId() == LOGARITHM)
        {
            auto ln_f = checkIfItsFunctionOfLinearArgument(log, var);
            auto arg = getArgumentOfFunction(log);
            if (ln_f.first != nullptr)
                return arg*(cos(argument) + c*sin(argument))/ln_f.first/(pow(c, 2) + one);
        }
        if (isSqrt(log))
        {
            auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(log.get()), var);
            if (ln_f.first != nullptr)
                return (two * sin(argument)*argument + two *cos(argument))/ln_f.first / pow(c, 2);
        }
    }
   //да, оно тут раскрывается по сумме и линейного
    auto lin_f = checkIfItsLinearFunction(this->argument, var);
    if (lin_f.first == nullptr)
        return nullptr;
    return one/lin_f.first * sin(this->argument);
}

const abs_ex &Cosinus::getArgument() const
{
    return this->argument;
}

void Cosinus::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > Cosinus::getTrigonometricalFunctions() const
{
    std::set<abs_ex> result;
    result.insert(cos(argument));
    return result;
}

long long Cosinus::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long Cosinus::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange Cosinus::getRange() const
{
    FunctionRange arg_range = this->argument->getRange();
    if (arg_range.isError())
        return arg_range;
    if (arg_range.getMin() == nullptr || arg_range.getMax() == nullptr)
        return FunctionRange(FunctionRangeSegment(nullptr, nullptr, false, false));
    FunctionRange result;
    auto toFirstPeriod = [](const abs_ex & val)->abs_ex
    {
        auto div_res = val/two/getPi();
        if (div_res->getId() == NUMBER)
            return toAbsEx(Number((div_res->getPositionRelativelyZero() > 0 ? 1 : -1), static_cast<Number*>( div_res.get())->getDenominator())) * two*getPi();
        int rat = val->getApproximateValue()/2/getPi()->getApproximateValue();
        return val - numToAbs(rat)*two*getPi();
    };
    for (auto &it : arg_range.getSegments())
    {
        if (biggerOrEquall(it.max() - it.min(), two*getPi()))
            return FunctionRange(FunctionRangeSegment(minus_one, one, true, true));
        auto min = toFirstPeriod(it.min());
        auto max = toFirstPeriod(it.max());
        bool min_lower_zero = lowerOrEquall(min, zero);
        if (min_lower_zero && lowerOrEquall(max, zero))
        {
            std::swap(min, max);
            min = -min;
            max = -max;
            min_lower_zero = false;
        }
        if (bigger(min, max))
        {
            //не может быть чтобы min был меньше нуля тут
            if (bigger(min, zero))
            {
                min = min - two*getPi();
                min_lower_zero = true;
            }
        }
        if (min_lower_zero)
        {
            if (lower(min, -getPi()))
                return FunctionRange(FunctionRangeSegment(minus_one, one, true, true));
            if (bigger(max, getPi()))
                return FunctionRange(FunctionRangeSegment(minus_one, one, true, true));
            bool min_take = bigger(abs(min), abs(max));
            result.addSegmentWithoutNormilizing(FunctionRangeSegment((min_take ?
                                                                          cos(min) : cos(max)), one,
                                                                     (min_take ? it.isMinIncluded() : it.isMaxIncluded()), true));
        }
        else
        {
            bool is_max_bigger_than_pi = bigger(max, getPi());
            if (is_max_bigger_than_pi && bigger(min, getPi()))
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(cos(min), cos(max),
                                                                         it.isMinIncluded(), it.isMaxIncluded()));
            else if (is_max_bigger_than_pi)
            {
                bool min_take = bigger(cos(min), cos(max));
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(minus_one, (min_take ? cos(min) : cos(max)),
                                                                         true, (min_take ? it.isMinIncluded() : it.isMaxIncluded())));
            }
            else
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(cos(max), cos(min), it.isMaxIncluded(),
                                                                     it.isMinIncluded()));
        }

    }
    result.normalize();
    return result;
}

bool Cosinus::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool Cosinus::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
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

abs_ex Cosinus::tryToFindExponentialFunction(int var) const
{
    return this->argument->tryToFindExponentialFunction(var);
}

void Cosinus::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

abs_ex cos(const abs_ex &expr)
{
    return abs_ex(new Cosinus(expr))->downcast();
}

abs_ex cos(abs_ex &&expr)
{
    return abs_ex(new Cosinus(std::move(expr)))->downcast();
}
