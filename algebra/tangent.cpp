#include "tangent.h"
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
#include "cotangent.h"
#include "cosinus.h"
#include "logarithm.h"
#include "absolutevalue.h"
#include "variablesdistributor.h"
Tangent::Tangent(const abs_ex & iargument)
{
    this->argument = makeAbstractExpression(iargument->getId(), iargument.get());
    this->simplify();
}
Tangent::Tangent(abs_ex && arg)
{
    this->argument.swap(arg);
    this->simplify();
}
Tangent::Tangent(const Tangent & copy)
{
    this->argument = makeAbstractExpression(copy.argument->getId(), copy.argument.get());
    this->simplify();
}
Tangent::Tangent(Tangent && mov)
{
    this->argument = std::move(mov.argument);
    this->simplify();
}

Tangent::~Tangent()
{

}

AlgebraExpression Tangent::getId() const
{
    return TANGENT;
}

void Tangent::simplify()
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

        if (coe.compareWith(1) > 0 || coe.compareWith(0) < 0 || coe == 1)
        {
            Number to_sub = 0;
            to_sub = ((coe.getNumerator() / coe.getDenominator()));
            if (coe.compareWith(0) < 0 && !(coe.getDenominator() == 1 && coe.getNumerator() % 1 == 0))
                to_sub -= 1;
            this->pi_member = this->pi_member - std::unique_ptr<Fractal>(new Fractal(getPi(), to_sub));
            this->argument = this->argument - abs_ex(new Fractal(getPi(), to_sub));
        }
        if (this->is_pi_member_only && coe == Number(1, 2))
            throw Exception();
    }
    this->simplified = true;
}

bool Tangent::operator==(AbstractExpression &right)
{
    if (right.getId() != TANGENT)
        return false;
    return subCompare(this->argument, static_cast<Tangent*>(&right)->argument);
}

bool Tangent::canDowncastTo()
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
    if ( this->is_pi_member_only)
    {
        if (pi_member->getCoefficient().getDenominator() <= 6 && pi_member->getCoefficient().getDenominator() != 5)
            return true;
        return false;
    }
    if ( this->pi_member != nullptr && !this->is_pi_member_only && isPiMemberOnAxis(pi_member->getCoefficient()))
        return true;
    //if ( this->argument->getId() == POLYNOMIAL && static_cast<Polynomial*>(this->argument.get())->getMonomialsPointers().size() == 2)
     //   return true;
    if (this->argument->getPositionRelativelyZero() < 0)
        return true;
    return false;
}

abs_ex Tangent::downcastTo()
{
    if (this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
    {
        return abs_ex(new Number(-1)) * abs_ex(new Tangent(abs_ex(new Number(-1)) * this->argument));
    }
    if (this->is_pi_member_only)
    {
        Number coe = this->pi_member->getCoefficient();
        if (coe == 0)
            return abs_ex(new Number(0));
        if (coe == Number(1, 6))
            return abs_ex(abs_ex(new Number(1)) / takeDegreeOf(Number(3), Number(1, 2)));
        if (coe == Number(5, 6))
            return abs_ex(abs_ex(new Number(-1)) / takeDegreeOf(Number(3), Number(1, 2)));
        if (coe == Number(1, 4))
            return abs_ex(new Number(1));
        if (coe == Number(3, 4))
            return abs_ex(new Number(-1));
        if (coe == Number(1, 3))
            return takeDegreeOf(Number(3), Number(1, 2));
        if (coe == Number(2, 3))
            return takeDegreeOf(Number(3), Number(1, 2)) * abs_ex(new Number(-1));
    }

    if (this->pi_member != nullptr && abs(this->pi_member->getCoefficient()) == Number(1, 2))
    {
        return minus_one * cot(this->argument - toAbsEx(this->pi_member));
    }
    if ( this->pi_member != nullptr && isPiMemberOnAxis(pi_member->getCoefficient()))
    {
        auto left = this->argument - abs_ex(new Fractal(this->pi_member.get()));
        return (tan(left) + tan(toAbsEx(this->pi_member)))/(one - tan(left)*tan(toAbsEx(this->pi_member)));
    }
  /*  if ( this->argument->getId() == POLYNOMIAL)
    {
        auto monoms = static_cast<Polynomial*>(this->argument.get())->getMonomialsPointers();
        auto right = this->argument - abs_ex(new Fractal(*monoms.begin()));
        return (tan(right) + tan(this->argument - right))/(one - tan(right)*tan(this->argument - right));
    }*/
    if (this->argument->getPositionRelativelyZero() < 0)
        return -tan(-argument);
    return abs_ex(nullptr);
}

std::set<int> Tangent::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> Tangent::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Tangent::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}

Number Tangent::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void Tangent::_qDebugOut()
{
    qDebug() << "Tangent:";
    this->argument->_qDebugOut();
    qDebug() << "End of sinus;";
}

QString Tangent::makeStringOfExpression() const
{
    return "tan(" + this->argument->makeStringOfExpression() + ")";
}

double Tangent::getApproximateValue()
{
    return tan(this->argument->getApproximateValue());
}

double Tangent::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return tan(this->argument->getApproximateValue(choosing_value_rule));
}

int Tangent::getPositionRelativelyZeroIfHasVariables()
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

QString Tangent::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

abs_ex Tangent::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex Tangent::changeSomePartOn(QString part, abs_ex &on_what)
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

abs_ex Tangent::changeSomePartOnExpression(QString part, abs_ex &on_what)
{
    NONCONST
            return changeSomePartOn(part, on_what);
}

abs_ex Tangent::getArgumentsCopy()
{
    return copy(this->argument);
}

abs_ex Tangent::derivative(int var) const
{
    return this->argument->derivative(var) / takeDegreeOf(cos(this->argument), 2);
}

abs_ex Tangent::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);

    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first == nullptr)
        return nullptr;
    return minus_one / ln_f.first * ln(abs(cos(this->argument)));
}

const abs_ex &Tangent::getArgument() const
{
    return this->argument;
}

void Tangent::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > Tangent::getTrigonometricalFunctions() const
{
    std::set<abs_ex> res;
    res.insert(tan(argument));
    return res;
}

long long Tangent::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long Tangent::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

bool Tangent::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == TANGENT);
    return AbstractExpression::less(this->argument.get(), (static_cast<Tangent*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}

abs_ex tan(const abs_ex &expr)
{
    return abs_ex(new Tangent(expr))->downcast();
}

abs_ex tan(abs_ex &&expr)
{
    return abs_ex(new Tangent(std::move(expr)))->downcast();
}
