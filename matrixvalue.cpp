#include "matrixvalue.h"
#include "algebraicexpressionvalue.h"
#include "vectorvalue.h"
MatrixValue::MatrixValue() : value(3, 3)
{
}

MatrixValue::MatrixValue(const Matrix<AlgExpr> &expr) : value(expr)
{

}

MatrixValue::MatrixValue(Matrix<AlgExpr> &&expr) : value (std::move(expr))
{

}

QString MatrixValue::toString() const
{
    QString res;
    res = "<br>";
    for (int i = 0; i < this->value.lines(); ++i)
    {
        res += "| ";
        for (int j = 0; j < this->value.columns(); ++j)
            res += this->value[i][j].toString()  + (j < this->value.columns() - 1 ? ", " : "");
        res += " |<br>";

    }
    return res;
}

QString MatrixValue::makeRenderString() const
{
    QString res = "!(Matrix(" + QString::number(this->getValue().columns()) + "," + QString::number(this->getValue().lines());
    for (int i = 0; i < this->getValue().lines(); ++i)
    {
        for (int j = 0; j < this->getValue().columns(); ++j)
        {
            res += "," + this->getValue()[i][j].makeRenderString();
        }
    }
    res += "))";
    return res;
}

QString MatrixValue::makeWolframString() const
{
    QString res=  "{";
    for (int i = 0; i < this->getValue().lines(); ++i)
    {
        res += "{";
        for (int j = 0; j < this->getValue().columns(); ++j)
            res += this->getValue()[i][j].toWolframString() + (j < getValue().columns() - 1 ? ", " : "");
        res += QString("}") + (i < getValue().lines() - 1 ? ", " : "");
    }
    res += "}";
    return res;
}

AbstractValueType MatrixValue::getId() const
{
    return VALUE_MATRIX;
}

std::unique_ptr<AbstractValue> MatrixValue::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_MATRIX)
        throw QString("Cannot add incomparable types");
    return std::unique_ptr<AbstractValue>(new MatrixValue(this->value + toMatrixPtr(b)->value));
}

std::unique_ptr<AbstractValue> MatrixValue::operator-(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_MATRIX)
        throw QString("Cannot subtract incomparable types");
    return std::unique_ptr<AbstractValue>(new MatrixValue(this->value - toMatrixPtr(b)->value));
}

std::unique_ptr<AbstractValue> MatrixValue::operator*(const std::unique_ptr<AbstractValue> &b) const
{
    switch(b->getId())
    {
        case VALUE_ALGEBRAIC_EXPRESSION:
            return std::unique_ptr<AbstractValue>(new MatrixValue(this->value * toAlgPtr(b)->getValue()));
        break;
        case VALUE_VECTOR:
            return std::unique_ptr<AbstractValue>(new VectorValue(this->value * toVectorPtr(b)->getValue()));
        break;
        case VALUE_MATRIX:
            return std::unique_ptr<AbstractValue>(new MatrixValue(this->value * toMatrixPtr(b)->getValue()));
        break;
    default:
        throw QString("Cannot multiply incomparable types");
    }
}

std::unique_ptr<AbstractValue> MatrixValue::operator/(const std::unique_ptr<AbstractValue> &b) const
{
    if (b->getId() != VALUE_ALGEBRAIC_EXPRESSION)
        throw QString("Matrix can be divided only on objects of elementary algebra");
    return std::unique_ptr<AbstractValue>(new MatrixValue(this->value / toAlgPtr(b)->getValue()));
}

const Matrix<AlgExpr> &MatrixValue::getValue() const
{
    return value;
}
