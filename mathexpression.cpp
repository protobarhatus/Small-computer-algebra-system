#include "mathexpression.h"
#include "algebraicexpressionvalue.h"
#include "vectorvalue.h"
#include "matrixvalue.h"
#include "functioninterpretationtoken.h"
#include "scriptsnamespace.h"
#include "polygonvalue.h"
#include "stringvalue.h"
#include "polyhedronvalue.h"
#include "linevalue.h"
#include "planevalue.h"
//MathExpression::MathExpression()
//{
//}

MathExpression::MathExpression()
{

}

MathExpression::MathExpression(std::nullptr_t)
{

}

MathExpression::MathExpression(MathExpression &&expr)
{
    this->value = std::move(expr.value);
}

MathExpression::MathExpression(const AlgExpr &expr)
{

    this->value = std::make_unique<AlgebraicExpressionValue>(expr);
}

MathExpression::MathExpression(AlgExpr &&expr)
{
    this->value = std::make_unique<AlgebraicExpressionValue>(std::move(expr));
}

MathExpression::MathExpression(const Vector<AlgExpr> &expr)
{
    this->value = std::make_unique<VectorValue>(expr);
}

MathExpression::MathExpression(Vector<AlgExpr> &&expr)
{
    this->value = std::make_unique<VectorValue>(std::move(expr));
}

MathExpression::MathExpression(const Matrix<AlgExpr> &expr)
{
    this->value = std::make_unique<MatrixValue>(expr);
}

MathExpression::MathExpression(Matrix<AlgExpr> &&expr)
{
    this->value = std::make_unique<MatrixValue>(std::move(expr));
}

MathExpression::MathExpression(const std::vector<AlgVector> &vert, const std::vector<QString> &names)
{
    this->value = std::unique_ptr<AbstractValue>( new PolygonValue(vert, names));
}

MathExpression::MathExpression(std::vector<AlgVector> &&vert, std::vector<QString> &&names)
{
    this->value = std::unique_ptr<AbstractValue>( new PolygonValue(std::move(vert), std::move(names)));
}

MathExpression::MathExpression(const std::vector<AlgVector> &vert, const std::vector<QString> &names, const AlgVector &center)
{
    this->value = std::unique_ptr<AbstractValue>( new PolygonValue(vert, names, center));
}

MathExpression::MathExpression(std::vector<AlgVector> &&vert, std::vector<QString> &&names, AlgVector &&center)
{
    this->value = std::unique_ptr<AbstractValue>( new PolygonValue(std::move(vert), std::move(names), std::move(center)));
}

MathExpression::MathExpression(const Polyhedron &pol)
{
    this->value = std::unique_ptr<AbstractValue>( new PolyhedronValue(pol));
}

MathExpression::MathExpression(Polyhedron &&pol)
{
    this->value = std::unique_ptr<AbstractValue>(new PolyhedronValue(std::move(pol)));
}

MathExpression::MathExpression(const QString &str)
{
    this->value = std::make_unique<StringValue>(str);
}

MathExpression::MathExpression(QString &&str)
{
    this->value = std::make_unique<StringValue> (std::move(str));
}

MathExpression::MathExpression(const Line3d &line)
{
    this->value = std::make_unique<LineValue>(line);
}

MathExpression::MathExpression(Line3d &&line)
{
    this->value = std::make_unique<LineValue>(std::move(line));
}

MathExpression::MathExpression(const Plane &plane)
{
    this->value = std::make_unique<PlaneValue>(plane);
}

MathExpression::MathExpression(Plane &&plane)
{
    this->value = std::make_unique<PlaneValue>(std::move(plane));
}

MathExpression::MathExpression(std::unique_ptr<AbstractValue> &&expr)
{
    this->value = std::move(expr);
}

MathExpression::MathExpression(const MathExpression &expr)
{
    this->value = copy(expr.value);
}

const MathExpression &MathExpression::operator=(const MathExpression &expr)
{
    this->value = copy(expr.value);
    return *this;
}

const MathExpression &MathExpression::operator=(MathExpression &&expr)
{
    this->value = std::move(expr.value);
    return *this;
}

QString MathExpression::toString() const
{
    return this->value->toString();
}

QString MathExpression::makeRenderString() const
{
    return this->value->makeRenderString();
}

QString MathExpression::makeWolframString() const
{
    return this->value->makeWolframString();
}

AbstractValueType MathExpression::getType() const
{
    return value->getId();
}


const AlgExpr &MathExpression::getAlgExprValue() const
{
    assert(this->getType() == VALUE_ALGEBRAIC_EXPRESSION);
    return toAlgPtr(this->value)->getValue();
}

const Vector<AlgExpr>& MathExpression::getVectorValue() const
{
    assert(this->getType() == VALUE_VECTOR);
    return toVectorPtr(this->value)->getValue();
}

const Matrix<AlgExpr>& MathExpression::getMatrixValue() const
{
    assert(this->getType() == VALUE_MATRIX);
    return toMatrixPtr(this->value)->getValue();
}

const PolygonValue &MathExpression::getPolygon() const
{
    assert(this->getType() == VALUE_POLYGON);
    return *static_cast<PolygonValue*>(this->value.get());
}

const PolyhedronValue &MathExpression::getPolyhedron() const
{
    assert(this->getType() == VALUE_POLYHEDRON);
    return *static_cast<PolyhedronValue*>(this->value.get());
}

const PlaneValue &MathExpression::getPlaneValue() const
{
    assert(this->getType() == VALUE_PLANE);
    return *static_cast<PlaneValue*>(this->value.get());
}

const LineValue &MathExpression::getLineValue() const
{
    assert(this->getType() == VALUE_LINE);
    return *static_cast<LineValue*>(this->value.get());
}

QString MathExpression::getStringValue() const
{
    assert(this->getType() == VALUE_STRING);
    return static_cast<StringValue*>(this->value.get())->getValue();
}

MathExpression MathExpression::executeFunction(const std::vector<MathExpression> &variables) const
{
    assert(this->getType() == VALUE_FUNCTION_TOKEN);
    return static_cast<const FunctionInterpretationToken*>(this->value.get())->getResult(variables);
}

MathExpression operator+(const MathExpression &left, const MathExpression &right)
{
    if (left.getType() == VALUE_FUNCTION_TOKEN || right.getType() == VALUE_FUNCTION_TOKEN)
        return MathExpression(std::unique_ptr<AbstractValue>(new FunctionInterpretationToken(
                          getAddFunction(), {left, right})));
    return MathExpression(*left.value + right.value);
}
MathExpression operator-(const MathExpression &left, const MathExpression &right)
{
    if (left.getType() == VALUE_FUNCTION_TOKEN || right.getType() == VALUE_FUNCTION_TOKEN)
        return MathExpression(std::unique_ptr<AbstractValue>(new FunctionInterpretationToken(
                           getSubFunction(), {left, right})));
    return MathExpression(*left.value - right.value);
}
MathExpression operator*(const MathExpression &left, const MathExpression &right)
{
    if (left.getType() == VALUE_FUNCTION_TOKEN || right.getType() == VALUE_FUNCTION_TOKEN)
        return MathExpression(std::unique_ptr<AbstractValue>(new FunctionInterpretationToken(
                           getMultFunction(), {left, right})));
    return MathExpression(*left.value * right.value);
}
MathExpression operator/(const MathExpression &left, const MathExpression &right)
{
    if (left.getType() == VALUE_FUNCTION_TOKEN || right.getType() == VALUE_FUNCTION_TOKEN)
        return MathExpression(std::unique_ptr<AbstractValue>(new FunctionInterpretationToken(
                           getDivFunction(), {left, right})));
    return MathExpression(*left.value / right.value);
}


AbstractValueType valueTypeFromStringName(const QString &name)
{
    if (name == "ALG" || name == "ALGEXPR" || name == "Algexpr" || name == "algexpr")
        return VALUE_ALGEBRAIC_EXPRESSION;
    if (name == "VEC" || name == "VECTOR" || name == "Vector" || name == "vector")
        return VALUE_VECTOR;
    if (name == "MAT" || name == "MATRIX" || name == "Matrix" || name == "matrix")
        return VALUE_MATRIX;
    return VALUE_INCORRECT;
}


