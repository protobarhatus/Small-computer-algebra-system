#include "sinus.h"
#include "some_algebra_expression_conversions.h"
#include <assert.h>
#include "number.h"
#include <QDebug>
#include "cmath"
#include "fractal.h"
#include "constant.h"
#include "degree.h"
#include "polynomial.h"
#include "cosinus.h"
#include "variablesdistributor.h"
Sinus::Sinus(const abs_ex & iargument)
{
    this->argument = copy(iargument);
    this->simplify();
}
Sinus::Sinus(abs_ex && arg)
{
    this->argument.swap(arg);
    this->simplify();
}
Sinus::Sinus(const Sinus & cop)
{
    this->argument = copy(cop.argument);
    this->simplified = cop.simplified;
    if (cop.pi_member != nullptr)
        this->pi_member = std::unique_ptr<Fractal>(new Fractal(*cop.pi_member));
    this->is_pi_member_only = cop.is_pi_member_only;
    this->simplify();
}
Sinus::Sinus(Sinus && mov)
{
    this->argument = std::move(mov.argument);
    this->simplified = mov.simplified;
    if (mov.pi_member != nullptr)
        this->pi_member = std::unique_ptr<Fractal>(new Fractal(*mov.pi_member));
    this->is_pi_member_only = mov.is_pi_member_only;
    this->simplify();
}
void Sinus::simplify()
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
        this->argument->simplify();
        this->argument = this->argument->downcast();
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
bool Sinus::operator==(AbstractExpression &right)
{
    if (right.getId() != SINUS)
        return false;
    return subCompare(this->argument, static_cast<Sinus*>(&right)->argument);
}
bool Sinus::canDowncastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
        return true;
    //if (pi_member == nullptr)
    //    return false;
    if (this->argument->getPositionRelativelyZero() < 0)
        return true;
    if (this->is_pi_member_only)
    {
        if (pi_member->getCoefficient().getDenominator() <= 6 && pi_member->getCoefficient().getDenominator() != 5)
            return true;
        return false;
    }
    if ( this->pi_member != nullptr && !this->is_pi_member_only && isPiMemberOnAxis(pi_member->getCoefficient()))
        return true;
   // if (this->argument->getId() == POLYNOMIAL && static_cast<Polynomial*>(this->argument.get())->getMonomialsPointers().size() == 2)
     //   return true;
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
        return true;
    if (this->argument->getId() == ARCSINUS)
        return true;
    return false;
}
abs_ex Sinus::downcastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
    {
        return abs_ex(new Number(-1)) * abs_ex(new Sinus(abs_ex(new Number(-1)) * this->argument));
    }
    if (this->is_pi_member_only)
    {
        Number coe = this->pi_member->getCoefficient();
        if (coe == 1 || coe == 0 || coe == -1)
            return abs_ex(new Number(0));
        if (coe == Number(1, 2))
            return abs_ex(new Number(1));
        if (coe == Number(3, 2))
            return abs_ex(new Number(-1));
        if (coe == Number(1, 6) || coe == Number(5, 6))
            return abs_ex(new Number(1, 2));
        if (coe == Number(7, 6) || coe == Number(11, 6))
            return abs_ex(new Number(-1, 2));
        if (coe == Number(1, 4) || coe == Number(3, 4))
            return takeDegreeOf(Number(2), Number(1, 2)) * abs_ex(new Number(1, 2));
        if (coe == Number(5, 4) || coe == Number(7, 4))
            return takeDegreeOf(Number(2), Number(1, 2)) * abs_ex(new Number(-1, 2));
        if (coe == Number(1, 3) || coe == Number(2, 3))
            return takeDegreeOf(Number(3), Number(1, 2)) * abs_ex(new Number(1, 2));
        if (coe == Number(4, 3) || coe == Number(5, 3))
            return takeDegreeOf(Number(3), Number(1, 2)) * abs_ex(new Number(-1, 2));
    }
    if (this->pi_member != nullptr && isPiMemberOnAxis(pi_member->getCoefficient()))
    {
        auto left = this->argument - abs_ex(new Fractal(this->pi_member.get()));
        return sin(left)*cos(toAbsEx(this->pi_member)) + sin(toAbsEx(this->pi_member))*cos(left);
    }

    if (this->argument->getPositionRelativelyZero() < 0)
    {
        return -sin(-argument);
    }
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
    {
        return integratingConstantExpr(this->argument->getId(), this->getRange());
    }
    if (this->argument->getId() == ARCSINUS)
        return getArgumentOfFunction(this->argument);
    return abs_ex(nullptr);
}
std::set<int> Sinus::getSetOfPolyVariables() const
{
    return std::set<int>();
}
std::set<int> Sinus::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Sinus::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}
Number Sinus::getMaxDegreeOfVariable(int id)
{
    //return this->argument->getMaxDegreeOfVariable(id);
    return Number::makeErrorNumber();
}
void Sinus::_qDebugOut()
{
    qDebug() << "SINUS:";
    this->argument->_qDebugOut();
    qDebug() << "End of sinus;";
}
QString Sinus::makeStringOfExpression() const
{
    return "sin(" + this->argument->makeStringOfExpression() + ")";
}

QString Sinus::makeWolframString() const
{
    return "Sin[" + argument->makeWolframString() + "]";
}

QString Sinus::toString() const
{
    return "sin(" + this->argument->toString() + ")";
}

QString Sinus::makeRenderString() const
{
    QString argstr = this->argument->makeRenderString();
    if (argstr.length() > 2)
        return "sin!(" + argstr + ")";
    return "sin(" + argstr + ")";
}
double Sinus::getApproximateValue()
{
    return sin(this->argument->getApproximateValue());
}
double Sinus::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return sin(this->argument->getApproximateValue(choosing_value_rule));
}
int Sinus::getPositionRelativelyZeroIfHasVariables()
{
    int z_pos = this->argument->getPositionRelativelyZero();
    if (z_pos > 0)
    {
        int pi_pos = (this->argument - getPi())->getPositionRelativelyZero();
        if (pi_pos < 0)
            return 1;
        if (pi_pos > 0 && (this->argument - abs_ex(new Number(2)) * getPi())->getPositionRelativelyZero() < 0)
            return -1;
        return 0;
    }
    if (z_pos < 0)
    {
        int pi_pos = (this->argument + getPi())->getPositionRelativelyZero();
        if (pi_pos > 0)
            return -1;
        if (pi_pos < 0 && (this->argument + abs_ex(new Number(2)) * getPi())->getPositionRelativelyZero() > 0)
            return 1;
        return 0;
    }
    return 0;

}
bool Sinus::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == SINUS);
    return AbstractExpression::less(this->argument.get(), (static_cast<Sinus*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}
Sinus::~Sinus()
{

}

AlgebraExpression Sinus::getId() const
{
    return SINUS;
}
QString Sinus::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

abs_ex Sinus::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex Sinus::changeSomePartOn(QString part, const abs_ex &on_what)
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

abs_ex Sinus::changeSomePartOnExpression(QString part, const abs_ex &on_what)
{
    NONCONST
           return  changeSomePartOn(part, on_what);
}

abs_ex Sinus::getArgumentsCopy()
{
    return copy(this->argument);
}

abs_ex Sinus::derivative(int var) const
{
    return cos(this->argument) * this->argument->derivative(var);
}

abs_ex Sinus::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    if (isSqrt(argument))
    {
        auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(argument.get()), var);
        if (ln_f.first != nullptr)
            return (two*sin(argument) - two*argument * cos(argument))/ln_f.first;
    }
    if (argument->getId() == LOGARITHM)
    {
        auto ln_f = checkIfItsFunctionOfLinearArgument(argument, var);
        auto arg = getArgumentOfFunction(argument);
        if (ln_f.first != nullptr)
            return -arg*(cos(argument) - sin(argument)) /two/ln_f.first;
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
                return -arg*(c*cos(argument) - sin(argument))/ln_f.first/(pow(c, 2) + one);
        }
        if (isSqrt(log))
        {
            auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(log.get()), var);
            if (ln_f.first != nullptr)
                return (two * sin(argument) - two*argument *cos(argument))/ln_f.first / pow(c, 2);
        }
    }
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first != nullptr)
        return minus_one / ln_f.first * cos(this->argument);
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

const abs_ex &Sinus::getArgument() const
{
    return this->argument;
}

void Sinus::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > Sinus::getTrigonometricalFunctions() const
{
    std::set<abs_ex> result;
    result.insert(copy(this));
    return result;
}

long long Sinus::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long Sinus::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange Sinus::getRange() const
{
    FunctionRange arg_range = this->argument->getRange();
    if (arg_range.isError())
        return arg_range;
    if (arg_range.getMin() == nullptr || arg_range.getMax() == nullptr)
        return FunctionRange(minus_one, one, true, true);
    auto toFirstPeriod = [](const abs_ex & val)->abs_ex
    {
        auto div_res = val/two/getPi();
        if (div_res->getId() == NUMBER)
            return toAbsEx(Number((div_res->getPositionRelativelyZero() > 0 ? 1 : -1), static_cast<Number*>( div_res.get())->getDenominator())) * two*getPi();
        int rat = val->getApproximateValue()/2/getPi()->getApproximateValue();
        return val - numToAbs(rat)*two*getPi();
    };
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (biggerOrEquall(it.max() - it.min(), two*getPi()))
            return FunctionRange(minus_one, one, true, true);
        if (it.isPoint())
        {
            result.addSegmentWithoutNormilizing(it.min(), it.max(), true, true);
            continue;
        }
        auto min = toFirstPeriod(it.min());
        auto max = toFirstPeriod(it.max());
        bool changed_sign = false;
        if (lower(min, zero) && lower(max, zero))
        {
            min = -min;
            max = -max;
            std::swap(min, max);
            changed_sign = true;
        }
        FunctionRangeSegment segm;
        if (bigger(min, max))
            min = min - getPi() * two;
        if (lowerOrEquall(min, -three/two * getPi()) && biggerOrEquall(max, -getPi()/two))
            return FunctionRange(minus_one, one, (*min != *(-three/two * getPi())) || it.isMinIncluded(),
                                 (*max != *(-getPi()/two)) || it.isMaxIncluded());

        if (lowerOrEquall(min, -one/two * getPi()) && biggerOrEquall(max, getPi()/two))
            return FunctionRange(minus_one, one, (*min != *(-one/two * getPi())) || it.isMinIncluded(),
                                 (*max != *(getPi()/two)) || it.isMaxIncluded());
        if (lowerOrEquall(min, getPi()/two) && biggerOrEquall(max, three*getPi()/two))
            return FunctionRange(minus_one, one, (*min != *(one/two * getPi())) || it.isMinIncluded(),
                                 (*max != *(three*getPi()/two)) || it.isMaxIncluded());
        bool is_sin_min_lower = lower(sin(min), sin(max));
        if ((lower(min, -three*getPi()/two) && bigger(max, -three*getPi()/two)) ||
                (lower(min, getPi()/two) && bigger(max, getPi()/two)))
            segm = FunctionRangeSegment((is_sin_min_lower ? sin(min) : sin(max)), one,
                                                (is_sin_min_lower ? it.isMinIncluded() : it.isMaxIncluded()), true);
        else if ((lower(min, -getPi()/two) && bigger(max, -getPi()/two)) ||
                 (lower(min, three*getPi()/two) && bigger(max, three*getPi()/two)))
            segm = FunctionRangeSegment(minus_one, (is_sin_min_lower ? sin(max) : sin(min)),
                                                true, (is_sin_min_lower ? it.isMaxIncluded() : it.isMinIncluded()));
        else
        {
            if (is_sin_min_lower)
                segm = FunctionRangeSegment(sin(min), sin(max), it.isMinIncluded(), it.isMaxIncluded());
            else
                segm = FunctionRangeSegment(sin(max), sin(min), it.isMaxIncluded(), it.isMinIncluded());
        }
        if (changed_sign)
            segm *= minus_one;
        result.addSegmentWithoutNormilizing(std::move(segm));
    }
    result.normalize();
    return result;
}

bool Sinus::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool Sinus::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
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

abs_ex Sinus::tryToFindExponentialFunction(int var) const
{
    return this->argument->tryToFindExponentialFunction(var);
}

void Sinus::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

void Sinus::doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const
{
    this->argument->doSomethingInDerivativeObject(func);
}

bool Sinus::canBeZero() const
{
    return true;
}

bool Sinus::hasUndefinedVariable() const
{
    return this->argument->hasUndefinedVariable();
}

abs_ex sin(const abs_ex &expr)
{
    return abs_ex(new Sinus(expr))->downcast();
}

abs_ex sin(abs_ex &&expr)
{
    return abs_ex(new Sinus(std::move(expr)))->downcast();
}

abs_ex csc(const abs_ex &expr)
{
    return one/sin(expr);
}

abs_ex csc(abs_ex &&expr)
{
    return one/std::move(sin(expr));
}
