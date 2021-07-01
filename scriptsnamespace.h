#ifndef SCRIPTSNAMESPACE_H
#define SCRIPTSNAMESPACE_H
#include "functionliteral.h"
#include "variableliteral.h"
#include "algebra/equationrootsconditions.h"
#include "mathexpression.h"
class ScriptsNameSpace
{
public:
    ScriptsNameSpace();
    bool hasFunction(const QString & name) const;
    bool canChangeFunction(const QString & function, int amount_of_vars) const;
    MathExpression callFunctionsAction(const QString & function, std::vector<MathExpression> && args) const;
    bool hasFunctionWithThatAmountOfArguments(const QString & name, int amount_of_vars) const;
    bool hasVariable(const QString & name) const;
    const MathExpression& getVariable(const QString & name) const;
    bool canChangeVariable(const QString & name) const;
    void changeVariable(const QString & name, MathExpression && new_value);
    void addVariable(const QString & name, MathExpression && value);
    void addFunction(const QString & name, int amount_of_vars, const std::function<MathExpression (std::vector<MathExpression> && )> &action);
    void changeFunction(const QString & name, int amount_of_vars, const std::function<MathExpression (std::vector<MathExpression> && )>& new_action);
    void addCondition(const RootCondition & cond);
    const std::vector<RootCondition>& getConditions() const;

    const FunctionLiteral& getFunctionLiteral(const QString & name) const;
private:
    const FunctionLiteral& getFunctionWithThatAmountOfArguments(const QString & function, int amount_of_vars) const;

    std::map<QString, VariableLiteral> variables;

    std::multimap<QString, FunctionLiteral> functions;
    std::vector<RootCondition> conditions;
};

FunctionLiteral getAddFunction();
FunctionLiteral getSubFunction();
FunctionLiteral getMultFunction();
FunctionLiteral getDivFunction();
#endif // SCRIPTSNAMESPACE_H
