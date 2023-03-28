#include "vectorvalue.h"
#include "algebra/algexpr.h"
#include "algebraicexpressionvalue.h"
#include "matrixvalue.h"
VectorValue::VectorValue() : value(3)
{

}

VectorValue::VectorValue(const Vector<AlgExpr> &expr)
{
    this->value = expr;
}

VectorValue::VectorValue(Vector<AlgExpr> &&expr)
{
    this->value = expr;
}

QString VectorValue::toString() const
{
    QString str = "{";
    for (int i = 0; i < this->value.size(); ++i)
    {
        str += this->value[i].toString();
        if (i < this->value.size() - 1)
            str += "; ";
        else
            str += "}";
    }
    return str;
}

QString VectorValue::makeRenderString() const
{
    QString res= "!(Matrix(1," + QString::number(this->getValue().size());
    for (int i = 0; i < this->getValue().size(); ++i)
        res += "," + this->getValue()[i].makeRenderString();
    return res + "))";
}

QString VectorValue::makeWolframString() const
{
    QString res = "{";
    for (int i = 0; i < this->getValue().size(); ++i)
        res += getValue()[i].toWolframString() + (i < getValue().size() - 1 ? ", " : "");
    return res + "}";
}

AbstractValueType VectorValue::getId() const
{
    return VALUE_VECTOR;
}

const Vector<AlgExpr> &VectorValue::getValue() const
{
    return value;
}

std::unique_ptr<AbstractValue> VectorValue::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_VECTOR)
        throw QString ("Cannot add incomparable types");
    return std::unique_ptr<AbstractValue> (new VectorValue(this->value + toVectorPtr(b)->value));
}

std::unique_ptr<AbstractValue> VectorValue::operator-(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_VECTOR)
        throw QString ("Cannot subtract incomparable types");
    return std::unique_ptr<AbstractValue> (new VectorValue(this->value - toVectorPtr(b)->value));
}

std::unique_ptr<AbstractValue> VectorValue::operator*(const std::unique_ptr<AbstractValue> &b) const
{
    switch (b->getId()) {
        case VALUE_ALGEBRAIC_EXPRESSION:
            return std::unique_ptr<AbstractValue>( new VectorValue(this->value * toAlgPtr(b)->getValue()));
        break;
        case VALUE_VECTOR:
            throw QString("For vector multiplication use VECT(a, b), for scalar - scalar(a, b)");
        break;
        case VALUE_MATRIX:
            return std::unique_ptr<AbstractValue>(new VectorValue(this->value * toMatrixPtr(b)->getValue()));
        break;
    default:
        throw QString("Cannot multiply incomparable types");
    }
}

std::unique_ptr<AbstractValue> VectorValue::operator/(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_ALGEBRAIC_EXPRESSION)
        throw QString("Vector can be divided only on objects of elementary algebra");
    return std::unique_ptr<AbstractValue>(new VectorValue(this->value / toAlgPtr(b)->getValue()));
}
