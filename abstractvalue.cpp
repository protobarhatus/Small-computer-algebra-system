#include "abstractvalue.h"
#include "algebraicexpressionvalue.h"
#include "matrixvalue.h"
#include "vectorvalue.h"
#include "functioninterpretationtoken.h"
#include "stringvalue.h"
#include "polygonvalue.h"
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
    case VALUE_FUNCTION_TOKEN:

   //     static_cast<FunctionInterpretationToken*>(val.get())->getResult({AlgExpr(1)});
        return std::unique_ptr<AbstractValue>(new FunctionInterpretationToken(*static_cast<FunctionInterpretationToken*>(val.get())));
    case VALUE_STRING:
        return std::unique_ptr<AbstractValue>(new StringValue(*static_cast<StringValue*>(val.get())));
    case VALUE_POLYGON:
        return std::unique_ptr<AbstractValue>(new PolygonValue(*static_cast<PolygonValue*>(val.get())));
    default:
        assert(false);

    }
}
