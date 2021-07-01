#ifndef FUNCTIONLITERAL_H
#define FUNCTIONLITERAL_H
#include "QString"
//#include "algebra/algexpr.h"
#include "functional"
#include "mathexpression.h"
#include <vector>
class FunctionArgumentType;

class FunctionLiteral
{
public:
    FunctionLiteral();
    FunctionLiteral(QString name, const  std::function<MathExpression (const std::vector<MathExpression> &)> & action, bool is_const = false);
    FunctionLiteral(QString name, int amount_of_vars, const  std::function<MathExpression (std::vector<MathExpression> &&)> & action, bool is_const = false);
    FunctionLiteral(const FunctionLiteral & func);
    void addArgument(const FunctionArgumentType & argument);
    bool operator==(const FunctionLiteral & func) const;
    bool operator!=(const FunctionLiteral & func) const;
    FunctionLiteral& operator=(const FunctionLiteral & func);

    MathExpression callAction(std::vector<MathExpression> && args) const;
    bool isConst() const;
    int amountOfArguments() const;
    void setAction(const  std::function<MathExpression (std::vector<MathExpression> &&)> & action);
private:

    std::function<MathExpression (std::vector<MathExpression> &&)> its_action;
    bool is_const;
    QString name;
    std::vector<FunctionArgumentType> arguments;
};
//аргумент - либо выражение, либо функция с какими-то своими аргументами
//вся эта хрень полезна, если я таки решу добавить замыкание, однако, пока я решил без него, а тогда весь этот класс беполезен
class FunctionArgumentType
{
public:
    FunctionArgumentType(const FunctionArgumentType & type);
    FunctionArgumentType(const FunctionLiteral & func);
    bool operator==(const FunctionArgumentType & type) const;
    bool operator!=(const FunctionArgumentType & type) const;
    FunctionArgumentType& operator=(const FunctionArgumentType & type);

    FunctionArgumentType();
private:


    bool is_function;
    FunctionLiteral its_function_arg;
    friend FunctionArgumentType expressionArgumentType();
    friend FunctionArgumentType functionArgumentType(const FunctionLiteral & func);

};
FunctionArgumentType expressionArgumentType();
FunctionArgumentType functionArgumentType(const FunctionLiteral & func);
#endif // FUNCTIONLITERAL_H
