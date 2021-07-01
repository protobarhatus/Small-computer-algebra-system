#ifndef VECTORVALUE_H
#define VECTORVALUE_H
#include "algebra/Vector.h"
#include "algebra/algexpr.h"
#include "abstractvalue.h"
class VectorValue : public AbstractValue
{
public:
    VectorValue();
    VectorValue(const Vector<AlgExpr> & expr);
    VectorValue(Vector<AlgExpr> && expr);
    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    const Vector<AlgExpr>& getValue() const;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;
private:
   Vector<AlgExpr> value;
};

#endif // VECTORVALUE_H
