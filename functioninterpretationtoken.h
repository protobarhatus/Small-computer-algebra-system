#ifndef FUNCTIONINTERPRETATIONTOKEN_H
#define FUNCTIONINTERPRETATIONTOKEN_H
#include "abstractvalue.h"
#include "mathexpression.h"
#include "functionliteral.h"
class FunctionInterpretationToken : public AbstractValue
{
public:
    FunctionInterpretationToken(const FunctionLiteral& action, std::vector<MathExpression> && arguments);
    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    MathExpression getResult(const std::vector<MathExpression> & variables_values) const;
private:
    std::vector<MathExpression> arguments;
    const FunctionLiteral& action;
};

#endif // FUNCTIONINTERPRETATIONTOKEN_H
