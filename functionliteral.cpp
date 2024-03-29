#include "functionliteral.h"
#include "functioninterpretationtoken.h"
FunctionLiteral::FunctionLiteral()
{

}

FunctionArgumentType::FunctionArgumentType(const FunctionArgumentType &type)
{
    this->is_function = type.is_function;
    if (this->is_function)
        this->its_function_arg = type.its_function_arg;
}

bool FunctionArgumentType::operator==(const FunctionArgumentType &type) const
{
    if (this->is_function == type.is_function && this->is_function == false)
        return true;
    if (this->is_function == type.is_function)
        return this->its_function_arg == type.its_function_arg;
    return false;
}

bool FunctionArgumentType::operator!=(const FunctionArgumentType &type) const
{
    return !(*this == type);
}

FunctionArgumentType &FunctionArgumentType::operator=(const FunctionArgumentType &type)
{
    this->is_function = type.is_function;
    if (this->is_function)
        this->its_function_arg = type.its_function_arg;
    return *this;
}



FunctionArgumentType::FunctionArgumentType()
{
    this->is_function = false;
}

FunctionArgumentType::FunctionArgumentType(const FunctionLiteral &func)
{
    this->its_function_arg = func;
}

FunctionArgumentType expressionArgumentType()
{

    return FunctionArgumentType();
}

FunctionArgumentType functionArgumentType(const FunctionLiteral &func)
{
    return FunctionArgumentType(func);
}

FunctionLiteral::FunctionLiteral(QString name, const  std::function<MathExpression (const std::vector<MathExpression> &)>& action, bool is_const)
{
    this->name = name;
    this->its_action = action;
    this->is_const = is_const;
}

FunctionLiteral::FunctionLiteral(QString name, int amount_of_vars, const std::function<MathExpression (std::vector<MathExpression> &&)> &action, bool is_const)
{
    this->name = name;
    this->its_action = action;
    for (int i = 0; i < amount_of_vars; ++i)
        this->arguments.push_back(expressionArgumentType());
    this->is_const = is_const;
}

FunctionLiteral::FunctionLiteral(const FunctionLiteral &func)
{
    this->name = func.name;
    this->arguments = func.arguments;
    this->is_const = func.is_const;
    this->its_action = func.its_action;
}

void FunctionLiteral::addArgument(const FunctionArgumentType &argument)
{
    this->arguments.push_back(argument);
}

bool FunctionLiteral::operator==(const FunctionLiteral &func) const
{
    return this->name == func.name && this->arguments == func.arguments && this->is_const == func.is_const;
}

bool FunctionLiteral::operator!=(const FunctionLiteral &func) const
{
    return !(*this == func);
}

FunctionLiteral &FunctionLiteral::operator=(const FunctionLiteral &func)
{
    this->name = func.name;
    this->arguments = func.arguments;
    this->is_const = func.is_const;
    this->its_action = func.its_action;
    return *this;
}

MathExpression FunctionLiteral::callAction(std::vector<MathExpression> &&args) const
{
    for (auto &it : args)
        if (it.getType() == VALUE_FUNCTION_TOKEN)
            return MathExpression(std::unique_ptr<AbstractValue>(new FunctionInterpretationToken(
               FunctionLiteral(this->name, this->amountOfArguments(),its_action
                              /* [this](std::vector<MathExpression> &&args){return this->callAction(std::move(args));}*/, true),
                                  std::move(args))));
    return this->its_action(std::move(args));
}

bool FunctionLiteral::isConst() const
{
    return this->is_const;
}

int FunctionLiteral::amountOfArguments() const
{
    return this->arguments.size();
}

void FunctionLiteral::setAction(const std::function<MathExpression (std::vector<MathExpression> &&)> &action)
{
    this->its_action = action;
}
