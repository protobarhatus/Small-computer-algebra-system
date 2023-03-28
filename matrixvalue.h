#ifndef MATRIXVALUE_H
#define MATRIXVALUE_H
#include "algebra/Matrix.h"
#include "abstractvalue.h"
class MatrixValue : public AbstractValue
{
public:
    MatrixValue();
    MatrixValue(const Matrix<AlgExpr> & expr);
    MatrixValue(Matrix<AlgExpr> && expr);
    virtual QString toString() const override;
    virtual QString makeRenderString() const override;
    virtual QString makeWolframString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    const Matrix<AlgExpr>& getValue() const;
private:
    Matrix<AlgExpr> value;
};

#endif // MATRIXVALUE_H
