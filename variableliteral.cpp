#include "variableliteral.h"



VariableLiteral::VariableLiteral(const QString &name, MathExpression && iexpr, bool is_constant) : expr(std::move(iexpr))
{
    this->name = name;
    this->is_constant = is_constant;
}

bool VariableLiteral::operator==(const VariableLiteral &var) const
{
    return this->name == var.name && this->is_constant == var.is_constant;
}

bool VariableLiteral::operator!=(const VariableLiteral &var) const
{
    return !(*this == var);
}

QString VariableLiteral::getName() const
{
    return this->name;
}

const MathExpression& VariableLiteral::getExpr() const
{
    return this->expr;
}

bool VariableLiteral::isConstant() const
{
    return this->is_constant;
}

void VariableLiteral::setExpr(MathExpression &&expr)
{
    this->expr = std::move(expr);
}

