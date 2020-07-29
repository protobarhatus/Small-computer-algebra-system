#include "variableliteral.h"



VariableLiteral::VariableLiteral(const QString &name, const AlgExpr &expr, bool is_constant)
{
    this->name = name;
    this->expr = expr;
    this->is_constant = is_constant;
}

bool VariableLiteral::operator==(const VariableLiteral &var) const
{
    return this->name == var.name && this->expr == var.expr && this->is_constant == var.is_constant;
}

bool VariableLiteral::operator!=(const VariableLiteral &var) const
{
    return !(*this == var);
}

QString VariableLiteral::getName() const
{
    return this->name;
}

AlgExpr VariableLiteral::getExpr() const
{
    return this->expr;
}

bool VariableLiteral::isConstant() const
{
    return this->is_constant;
}

void VariableLiteral::setExpr(const AlgExpr &expr)
{
    this->expr = expr;
}

