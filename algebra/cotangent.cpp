#include "Cotangent.h"
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
Cotangent::Cotangent(abs_ex & iargument)
{
    this->argument = makeAbstractExpression(iargument->getId(), iargument.get());
    this->simplify();
}
Cotangent::Cotangent(abs_ex && arg)
{
    this->argument.swap(arg);
    this->simplify();
}
Cotangent::Cotangent(const Cotangent & copy)
{
    this->argument = makeAbstractExpression(copy.argument->getId(), copy.argument.get());
    this->simplify();
}
Cotangent::Cotangent(Cotangent && mov)
{
    this->argument = std::move(mov.argument);
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
            this->argument = abs_ex(fr->turnIntoPolynomWithOpeningParentheses().release());
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
            this->argument = this->argument - std::unique_ptr<AbstractExpression>(new Fractal(getPi(), to_sub));
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
    return *this->argument == *(static_cast<Cotangent*>(&right)->argument);
}

bool Cotangent::canDowncastTo(AlgebraExpression expr)
{
    if (expr == FRACTAL && this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
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
    if (expr == FRACTAL && this->is_pi_member_only)
    {
        if (pi_member->getCoefficient().getDenominator() <= 6 && pi_member->getCoefficient().getDenominator() != 5)
            return true;
        return false;
    }
    return false;
}

std::unique_ptr<AbstractExpression> Cotangent::downcastTo(AlgebraExpression expr)
{
    if (expr == FRACTAL && this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
    {
        return abs_ex(new Number(-1)) * abs_ex(new Cotangent(abs_ex(new Number(-1)) * this->argument));
    }
    if (this->is_pi_member_only && expr == FRACTAL)
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
    return 0;
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

std::unique_ptr<AbstractExpression> Cotangent::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> Cotangent::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> Cotangent::getArgumentsCopy()
{
    return copy(this->argument);
}

bool Cotangent::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == COTANGENT);
    return AbstractExpression::less(this->argument.get(), (static_cast<Cotangent*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}

std::unique_ptr<AbstractExpression> cot(std::unique_ptr<AbstractExpression> &expr)
{
    return abs_ex(new Cotangent(expr));
}

std::unique_ptr<AbstractExpression> cot(std::unique_ptr<AbstractExpression> &&expr)
{
    return abs_ex(new Cotangent(std::move(expr)));
}
