#include "abstractvalue.h"
#include "algebraicexpressionvalue.h"
#include "matrixvalue.h"
#include "vectorvalue.h"
AbstractValue::AbstractValue()
{

}
const AlgebraicExpressionValue* toAlgPtr(const std::unique_ptr<AbstractValue> &b)
{
    return static_cast<AlgebraicExpressionValue*>(
                b.get());
}
const MatrixValue *toMatrixPtr(const std::unique_ptr<AbstractValue> &b)
{
    return static_cast<MatrixValue*>(
                b.get());
}

const VectorValue *toVectorPtr(const std::unique_ptr<AbstractValue> &b)
{
    return static_cast<VectorValue*>(
                b.get());
}

std::unique_ptr<AbstractValue> copy(const std::unique_ptr<AbstractValue> &val)
{
    if (val == nullptr)
        return nullptr;
    switch(val->getId())
    {
        case VALUE_ALGEBRAIC_EXPRESSION:
            return std::unique_ptr<AbstractValue>(new AlgebraicExpressionValue(*static_cast<AlgebraicExpressionValue*>(val.get())));
    case VALUE_VECTOR:
        return std::unique_ptr<AbstractValue>(new VectorValue(*static_cast<VectorValue*>(val.get())));
    case VALUE_MATRIX:
        return std::unique_ptr<AbstractValue>(new MatrixValue(*static_cast<MatrixValue*>(val.get())));
    default:
        assert(false);

    }
}
