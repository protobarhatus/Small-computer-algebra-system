#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H
#include "abstractvalue.h"
#include <memory>
class AlgExpr;
class ScriptsNameSpace;
#include <algebra/Matrix.h>

AbstractValueType valueTypeFromStringName(const QString & name);
class MathExpression
{
public:
    MathExpression();
    MathExpression(nullptr_t);
    MathExpression(MathExpression && expr);
    MathExpression(const AlgExpr & expr);
    MathExpression(AlgExpr && expr);
    MathExpression(const Vector<AlgExpr> & expr);
    MathExpression( Vector<AlgExpr> && expr);
    MathExpression(const Matrix<AlgExpr> & expr);
    MathExpression(Matrix<AlgExpr> && expr);
    MathExpression(std::unique_ptr<AbstractValue> && expr);

    MathExpression(const MathExpression& expr);
    const MathExpression& operator=(const MathExpression & expr);
    const MathExpression& operator=(MathExpression && expr);

    QString toString() const;
    AbstractValueType getType() const;



    friend MathExpression operator+(const MathExpression & left, const MathExpression & right);
    friend MathExpression operator-(const MathExpression & left, const MathExpression & right);
    friend MathExpression operator*(const MathExpression & left, const MathExpression & right);
    friend MathExpression operator/(const MathExpression & left, const MathExpression & right);

    const AlgExpr& getAlgExprValue() const;
    const Vector<AlgExpr>& getVectorValue() const;
    const Matrix<AlgExpr>& getMatrixValue() const;

    MathExpression executeFunction(const std::vector<MathExpression> & variables) const;
private:

    std::unique_ptr<AbstractValue> value;
};


MathExpression operator+(const MathExpression & left, const MathExpression & right);
MathExpression operator-(const MathExpression & left, const MathExpression & right);
MathExpression operator*(const MathExpression & left, const MathExpression & right);
MathExpression operator/(const MathExpression & left, const MathExpression & right);
#endif // MATHEXPRESSION_H
