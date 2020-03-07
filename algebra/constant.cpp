#include "constant.h"
#include "number.h"
#include <QDebug>
bool eq(double a, double b)
{
    return abs(a - b) <= 1e-10;
}
Constant::Constant(double ivalue, QString iname) : value(ivalue), name(iname)
{

}
Constant::Constant(const Constant & copy)
{
    this->value = copy.value;
    this->name = copy.name;
}
Constant::~Constant()
{

}

AlgebraExpression Constant::getId() const
{
    return CONSTANT;
}

void Constant::simplify()
{

}

bool Constant::operator==(AbstractExpression &right)
{
    if (right.getId() != CONSTANT)
        return false;
    return eq(this->value, static_cast<Constant*>(&right)->value);
}

bool Constant::canDowncastTo(AlgebraExpression expr)
{
    return false;
}

std::unique_ptr<AbstractExpression> Constant::downcastTo(AlgebraExpression expr)
{
    return abs_ex(nullptr);
}

std::set<int> Constant::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> Constant::getSetOfVariables() const
{
    return std::set<int>();
}

Number Constant::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void Constant::_qDebugOut()
{
    qDebug() << "CONST: " << this->name;
}

QString Constant::makeStringOfExpression()
{
    return this->name;
}

double Constant::getApproximateValue()
{
    return this->value;
}

double Constant::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return this->value;
}

int Constant::getPositionRelativelyZeroIfHasVariables()
{
    return (this->value > 0 ? 1 : (this->value < 0 ? -1 : 0));
}
bool Constant::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == CONSTANT);
    return this->value < static_cast<Constant*>(const_cast<AbstractExpression*>(&right))->value;
}
abs_ex getPi()
{
    return abs_ex(new Constant(3.14159265358979323846, QChar(L'π')));
}
abs_ex getEuler()
{
    return abs_ex(new Constant(2.71828182845904523536, "e"));
}
