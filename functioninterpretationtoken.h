#ifndef FUNCTIONINTERPRETATIONTOKEN_H
#define FUNCTIONINTERPRETATIONTOKEN_H
#include "abstractvalue.h"
#include "mathexpression.h"
#include "functionliteral.h"
class FunctionInterpretationToken : public AbstractValue
{
public:
    FunctionInterpretationToken(const FunctionLiteral& action, std::vector<MathExpression> && arguments, bool is_variable_function = false);
    virtual QString toString() const override;
    virtual AbstractValueType getId() const override;
    virtual QString makeRenderString() const override;
    virtual QString makeWolframString() const override;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const override;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const override;

    MathExpression getResult(const std::vector<MathExpression> & variables_values) const;
private:
    bool is_variable_function = false;
    std::vector<MathExpression> arguments;
    FunctionLiteral action;
};

#endif // FUNCTIONINTERPRETATIONTOKEN_H
