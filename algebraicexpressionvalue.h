#ifndef ALGEBRAICEXPRESSIONVALUE_H
#define ALGEBRAICEXPRESSIONVALUE_H
#include "abstractvalue.h"
#include "algebra/algexpr.h"
class AlgebraicExpressionValue : public AbstractValue
{
public:
    AlgebraicExpressionValue();
    AlgebraicExpressionValue(const AlgExpr & expr);
    AlgebraicExpressionValue(AlgExpr && expr);
    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    const AlgExpr& getValue() const;
private:
    AlgExpr value;
};


#endif // ALGEBRAICEXPRESSIONVALUE_H
