#include "functioninterpretationtoken.h"
#include <assert.h>


FunctionInterpretationToken::FunctionInterpretationToken(const FunctionLiteral &func, std::vector<MathExpression> &&args):
    action(func), arguments(std::move(args))
{

}

QString FunctionInterpretationToken::toString() const
{
    assert(false);
    return "";
}

AbstractValueType FunctionInterpretationToken::getId() const
{
    return VALUE_FUNCTION_TOKEN;
}

std::unique_ptr<AbstractValue> FunctionInterpretationToken::operator+(const std::unique_ptr<AbstractValue> &b) const
{
    assert(false);
    return nullptr;
}
std::unique_ptr<AbstractValue> FunctionInterpretationToken::operator-(const std::unique_ptr<AbstractValue> &b) const
{
    assert(false);
    return nullptr;
}
std::unique_ptr<AbstractValue> FunctionInterpretationToken::operator*(const std::unique_ptr<AbstractValue> &b) const
{
    assert(false);
    return nullptr;
}
std::unique_ptr<AbstractValue> FunctionInterpretationToken::operator/(const std::unique_ptr<AbstractValue> &b) const
{
    assert(false);
    return nullptr;
}

MathExpression FunctionInterpretationToken::getResult(const std::vector<MathExpression> &variables_values) const
{
    std::vector<MathExpression> counted_arguments(this->arguments.size());
    for (int i = 0; i < this->arguments.size(); ++i)
    {
        if (arguments[i].getType() == VALUE_FUNCTION_TOKEN)
            counted_arguments[i] = arguments[i].executeFunction(variables_values);
        else
            counted_arguments[i] = arguments[i];
    }
    return this->action.callAction(std::move(counted_arguments));
}
