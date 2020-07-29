#ifndef SCRIPTSNAMESPACE_H
#define SCRIPTSNAMESPACE_H
#include "functionliteral.h"
#include "variableliteral.h"
#include "algebra/equationrootsconditions.h"
class ScriptsNameSpace
{
public:
    ScriptsNameSpace();
    bool hasFunction(const QString & name) const;
    bool canChangeFunction(const QString & function, int amount_of_vars) const;
    AlgExpr callFunctionsAction(const QString & function, std::vector<AlgExpr> && args) const;
    bool hasFunctionWithThatAmountOfArguments(const QString & name, int amount_of_vars) const;
    bool hasVariable(const QString & name) const;
    AlgExpr getVariable(const QString & name) const;
    bool canChangeVariable(const QString & name) const;
    void changeVariable(const QString & name, const AlgExpr & new_value);
    void addVariable(const QString & name, const AlgExpr & value);
    void addFunction(const QString & name, int amount_of_vars, const std::function<AlgExpr (std::vector<AlgExpr> && )> &action);
    void changeFunction(const QString & name, int amount_of_vars, const std::function<AlgExpr (std::vector<AlgExpr> && )>& new_action);
    void addCondition(const RootCondition & cond);
    const std::vector<RootCondition>& getConditions() const;
private:
    const FunctionLiteral& getFunctionWithThatAmountOfArguments(const QString & function, int amount_of_vars) const;

    std::map<QString, VariableLiteral> variables;

    std::multimap<QString, FunctionLiteral> functions;
    std::vector<RootCondition> conditions;
};

#endif // SCRIPTSNAMESPACE_H
