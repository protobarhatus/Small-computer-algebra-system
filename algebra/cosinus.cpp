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
Cosinus::Cosinus(abs_ex & iargument)
{
    this->argument = makeAbstractExpression(iargument->getId(), iargument.get());
    this->simplify();
}
Cosinus::Cosinus(abs_ex && arg)
{
    this->argument.swap(arg);
    this->simplify();
}
Cosinus::Cosinus(const Cosinus & copy)
{
    this->argument = makeAbstractExpression(copy.argument->getId(), copy.argument.get());
    this->simplify();
}
Cosinus::Cosinus(Cosinus && mov)
{
    this->argument = std::move(mov.argument);
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
            this->argument = abs_ex(fr->turnIntoPolynomWithOpeningParentheses().release());
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
            this->argument = this->argument - std::unique_ptr<AbstractExpression>(new Fractal(getPi(), to_sub));
        }

    }
    this->simplified = true;
}
bool Cosinus::operator==(AbstractExpression &right)
{
    if (right.getId() != COSINUS)
        return false;
    return *this->argument == *(static_cast<Cosinus*>(&right)->argument);
}
bool Cosinus::canDowncastTo(AlgebraExpression expr)
{
    if (expr == FRACTAL && this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
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
    if (expr == FRACTAL && this->is_pi_member_only)
    {
        if (pi_member->getCoefficient().getDenominator() <= 6 && pi_member->getCoefficient().getDenominator() != 5)
            return true;
        return false;
    }
    if (expr == POLYNOMIAL && this->pi_member != nullptr && !this->is_pi_member_only && isPiMemberInTable(pi_member->getCoefficient()))
        return true;
    if (expr == POLYNOMIAL && this->argument->getId() == POLYNOMIAL && static_cast<Polynomial*>(this->argument.get())->getMonomialsPointers().size() == 2)
        return true;
    return false;
}
abs_ex Cosinus::downcastTo(AlgebraExpression expr)
{
    if (expr == FRACTAL && this->argument->getId() == FRACTAL && static_cast<Fractal*>(this->argument.get())->getCoefficient().compareWith(0) < 0)
    {
        return abs_ex(new Number(-1)) * abs_ex(new Cosinus(abs_ex(new Number(-1)) * this->argument));
    }
    if (this->is_pi_member_only && expr == FRACTAL)
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
    if (expr == POLYNOMIAL && this->pi_member != nullptr && isPiMemberInTable(pi_member->getCoefficient()))
    {
        auto left = this->argument - abs_ex(new Fractal(this->pi_member.get()));
        return cos(left)*cos(toAbsEx(this->pi_member)) - sin(toAbsEx(this->pi_member))*sin(left);
    }
    if (expr == POLYNOMIAL && this->argument->getId() == POLYNOMIAL)
    {
        auto monoms = static_cast<Polynomial*>(this->argument.get())->getMonomialsPointers();
        auto right = this->argument - abs_ex(new Fractal(*monoms.begin()));
        return cos(this->argument - right) * cos(right) - sin(right) * sin(this->argument - right);
    }
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
    return 0;
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

std::unique_ptr<AbstractExpression> Cosinus::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> Cosinus::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> Cosinus::getArgumentsCopy()
{
    return copy(argument);
}

std::unique_ptr<AbstractExpression> cos(std::unique_ptr<AbstractExpression> &expr)
{
    return abs_ex(new Cosinus(expr))->downcast();
}

std::unique_ptr<AbstractExpression> cos(std::unique_ptr<AbstractExpression> &&expr)
{
    return abs_ex(new Cosinus(std::move(expr)))->downcast();
}
