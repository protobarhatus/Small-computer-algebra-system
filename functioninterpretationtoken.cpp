#include "functioninterpretationtoken.h"
#include <assert.h>


FunctionInterpretationToken::FunctionInterpretationToken(const FunctionLiteral &func, std::vector<MathExpression> &&args, bool is_variable):
    action(func), arguments(std::move(args)), is_variable_function(is_variable)
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

QString FunctionInterpretationToken::makeRenderString() const
{
    assert(false);
    return "";
}

QString FunctionInterpretationToken::makeWolframString() const
{
    assert(false);
    return "";
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
    if (this->is_variable_function)
        return this->action.callAction(std::vector<MathExpression>(variables_values));
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
