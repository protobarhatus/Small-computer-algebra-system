#include "algebraicexpressionvalue.h"
#include "vectorvalue.h"
#include "matrixvalue.h"
AlgebraicExpressionValue::AlgebraicExpressionValue()
{

}

AlgebraicExpressionValue::AlgebraicExpressionValue(const AlgExpr &expr)
{
    value = expr;
}

AlgebraicExpressionValue::AlgebraicExpressionValue(AlgExpr &&expr)
{
    value = std::move(expr);
}

QString AlgebraicExpressionValue::toString() const
{
    return this->value.toString();
}

QString AlgebraicExpressionValue::makeRenderString() const
{
    return this->value.makeRenderString();
}

QString AlgebraicExpressionValue::makeWolframString() const
{
    return this->value.toWolframString();
}

AbstractValueType AlgebraicExpressionValue::getId() const
{
    return VALUE_ALGEBRAIC_EXPRESSION;
}

std::unique_ptr<AbstractValue> AlgebraicExpressionValue::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_ALGEBRAIC_EXPRESSION)
        throw QString("Cannot add incomparable types");
    return std::unique_ptr<AbstractValue>(new AlgebraicExpressionValue(this->value + toAlgPtr(b)->value));
}

std::unique_ptr<AbstractValue> AlgebraicExpressionValue::operator-(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_ALGEBRAIC_EXPRESSION)
        throw QString("Cannot subtract incomparable types");
    return std::unique_ptr<AbstractValue>(new AlgebraicExpressionValue(this->value - toAlgPtr(b)->value));
}
std::unique_ptr<AbstractValue> AlgebraicExpressionValue::operator*(const std::unique_ptr<AbstractValue> &b) const
{
    switch (b->getId()) {
    case VALUE_ALGEBRAIC_EXPRESSION:
        return std::unique_ptr<AbstractValue>(new AlgebraicExpressionValue(this->value * toAlgPtr(b)->value));
    case VALUE_VECTOR:
        return std::unique_ptr<AbstractValue>(new VectorValue(this->value * toVectorPtr(b)->getValue()));
    case VALUE_MATRIX:
        return std::unique_ptr<AbstractValue>(new MatrixValue(toMatrixPtr(b)->getValue() * this->value));
    default:
        throw QString("Cannot multiply incomparable types");
    }

}
std::unique_ptr<AbstractValue> AlgebraicExpressionValue::operator/(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_ALGEBRAIC_EXPRESSION)
        throw QString("Cannot divide incomparable types");
    return std::unique_ptr<AbstractValue>(new AlgebraicExpressionValue(this->value / toAlgPtr(b)->value));
}

const AlgExpr &AlgebraicExpressionValue::getValue() const
{
    return value;
}
