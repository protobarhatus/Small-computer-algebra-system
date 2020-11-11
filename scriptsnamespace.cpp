#include "scriptsnamespace.h"
#
ScriptsNameSpace::ScriptsNameSpace()
{
    for (int i = 0; i < 26; ++i)
    {
        QString name = makeVariablesName(i + 1);
        auto var_expr = AlgExpr(getVariableExpr(i + 1));
        if (var_expr.toString() == "e")
            this->variables.insert({"e", VariableLiteral("e", e(), true)});
        this->variables.insert({var_expr.toString(),  VariableLiteral(var_expr.toString(), var_expr, true)});
    }
    this->variables.insert({"E", VariableLiteral("E", e(), true)});
    this->variables.insert({"pi", VariableLiteral("pi", pi(), true)});
    this->variables.insert({"Pi", VariableLiteral("Pi", pi(), true)});

    this->variables.insert({"dx", VariableLiteral("dx", D(getVariable("x")), true)});
    this->variables.insert({"dy", VariableLiteral("dy", D(getVariable("y")), true)});

    this->functions.insert({"sqrt", FunctionLiteral("sqrt", 1, [this](std::vector<AlgExpr>&& args) {
                                this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, args[0].getExpr()));
                                return sqrt(std::move(args[0]));}, true)});
    this->functions.insert({"cbrt", FunctionLiteral("cbrt", 1, [](std::vector<AlgExpr>&& args) { return cbrt(std::move(args[0]));}, true)});
    this->functions.insert({"pow", FunctionLiteral("pow", 2, [this](std::vector<AlgExpr>&& args) {
                                if (args[1].getExpr()->getId() == FRACTAL)
                                {
                                    if ( static_cast<Fractal*>(args[1].getExpr().get())->getFractal().second->empty())
                                    {
                                        if (static_cast<Fractal*>(args[1].getExpr().get())->getCoefficient().getDenominator() % 2 == 0)
                                            this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, args[0].getExpr()));
                                    }
                                    else
                                        this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, args[0].getExpr()));
                                }
                                return pow(std::move(args[0]), std::move(args[1]));}, true)
                           });

    this->functions.insert({"sin", FunctionLiteral("sin", 1, [](std::vector<AlgExpr>&& args) { return sin(std::move(args[0]));}, true)});
    this->functions.insert({"cos", FunctionLiteral("cos", 1, [](std::vector<AlgExpr>&& args) { return cos(std::move(args[0]));}, true)});
    this->functions.insert({"tan", FunctionLiteral("tan", 1, [](std::vector<AlgExpr>&& args) { return tan(std::move(args[0]));}, true)});
    this->functions.insert({"cot", FunctionLiteral("cot", 1, [](std::vector<AlgExpr>&& args) { return cot(std::move(args[0]));}, true)});

    this->functions.insert({"asin", FunctionLiteral("asin", 1, [this](std::vector<AlgExpr>&& args) {
                                this->addCondition(RootCondition(RootCondition::LESS_OR_EQUAL_ZERO, (args[0] - 1).getExpr()));
                                this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, (args[0] + 1).getExpr()));
                                return asin(std::move(args[0]));
                            }, true)});
    this->functions.insert({"acos", FunctionLiteral("acos", 1, [this](std::vector<AlgExpr>&& args) {
                                this->addCondition(RootCondition(RootCondition::LESS_OR_EQUAL_ZERO, (args[0] - 1).getExpr()));
                                this->addCondition(RootCondition(RootCondition::BIGGER_OR_EQUAL_ZERO, (args[0] + 1).getExpr()));
                                return acos(std::move(args[0]));
                            }, true)});
    this->functions.insert({"atan", FunctionLiteral("atan", 1, [](std::vector<AlgExpr>&& args) { return atan(std::move(args[0]));}, true)});
    this->functions.insert({"acot", FunctionLiteral("acot", 1, [](std::vector<AlgExpr>&& args) { return acot(std::move(args[0]));}, true)});

    this->functions.insert({"ln", FunctionLiteral("ln", 1, [this](std::vector<AlgExpr>&& args) {
                                this->addCondition(RootCondition(RootCondition::BIGGER_THAN_ZERO, args[0].getExpr()));
                                return ln(std::move(args[0]));
                            }, true)});
    this->functions.insert({"abs", FunctionLiteral("abs", 1, [](std::vector<AlgExpr>&& args) { return abs(std::move(args[0]));}, true)});

    this->functions.insert({"log", FunctionLiteral("log", 2, [this](std::vector<AlgExpr>&& args) {
                                this->addCondition(RootCondition(RootCondition::BIGGER_THAN_ZERO, args[1].getExpr()));
                                this->addCondition(RootCondition(RootCondition::BIGGER_THAN_ZERO, args[0].getExpr()));
                                this->addCondition(RootCondition(RootCondition::DONT_EQUAL_ZERO, (args[0] - 1).getExpr()));
                                //в AlgExpr идет сначала аргумент, затем основание, здесь будет наоборот
                                return log(std::move(args[1]), std::move(args[0]));}, true)});

    this->functions.insert({"D", FunctionLiteral("D", 1, [](std::vector<AlgExpr>&& args) { return D(std::move(args[0]));}, true)});
    this->functions.insert({"D", FunctionLiteral("D", 2, [](std::vector<AlgExpr>&& args) { return derivative(std::move(args[0]), args[1]);}, true)});
    this->functions.insert({"Integrate", FunctionLiteral("Integrate", 2, [](std::vector<AlgExpr>&& args) {

                                auto integr = integral(std::move(args[0]), args[1]);
                                if (integr.getExpr() == nullptr)
                                    throw QIODevice::tr("Не удается взять интеграл от ") + args[0].toString();
                                return integr + integratingConstantExpr();
                            }, true)});
    this->functions.insert({"Integrate", FunctionLiteral("Integrate", 1, [](std::vector<AlgExpr>&& args) {
                                if (!hasDifferentialAsMultiplier(args[0].getExpr()))
                                    throw QIODevice::tr("Вызов функции Integrate с одним аргументом требует дифференциал какой-либо переменной в качестве множителя");
                                auto integr = integral(std::move(args[0]));
                                if (integr.getExpr() == nullptr)
                                    throw QIODevice::tr("Не удается взять интеграл от ") + args[0].toString();
                                return integr + integratingConstantExpr();
                            })});

    this->functions.insert({"Factorize", FunctionLiteral("Factorize", 1, [](std::vector<AlgExpr>&& args) { return factorize(std::move(args[0]));}, true)});


this->functions.insert({"Expand", FunctionLiteral("Expand", 1, [](std::vector<AlgExpr>&& args) { return expand(std::move(args[0]));}, true)});
}

bool ScriptsNameSpace::hasFunction(const QString &name) const
{
    return this->functions.find(name) != this->functions.end();
}

bool ScriptsNameSpace::canChangeFunction(const QString &function, int amount_of_vars) const
{
    return this->getFunctionWithThatAmountOfArguments(function, amount_of_vars).isConst();
}

AlgExpr ScriptsNameSpace::callFunctionsAction(const QString &function, std::vector<AlgExpr> &&args) const
{
   return this->getFunctionWithThatAmountOfArguments(function, args.size()).callAction(std::move(args));
    assert(false);
}

bool ScriptsNameSpace::hasFunctionWithThatAmountOfArguments(const QString &name, int amount_of_vars) const
{
    auto it = this->functions.find(name);
    while (it != this->functions.end() && it->first == name)
    {
        if (it->second.amountOfArguments() == amount_of_vars)
            return true;
        ++it;
    }
    return false;
}

bool ScriptsNameSpace::hasVariable(const QString &name) const
{
   // qDebug() << (this->variables.find(name) != this->variables.end());
  //  qDebug() << name;
   // qDebug() << this->variables.find(name)->first;
    return this->variables.find(name) != this->variables.end();
}

AlgExpr ScriptsNameSpace::getVariable(const QString &name) const
{
    return this->variables.find(name)->second.getExpr();
}

bool ScriptsNameSpace::canChangeVariable(const QString &name) const
{
    return !this->variables.find(name)->second.isConstant();
}

void ScriptsNameSpace::changeVariable(const QString &name, const AlgExpr &new_value)
{
    this->variables.find(name)->second.setExpr(new_value);
}

void ScriptsNameSpace::addVariable(const QString &name, const AlgExpr &value)
{
    this->variables.insert({name, VariableLiteral(name, value)});
}

void ScriptsNameSpace::addFunction(const QString &name, int amount_of_vars, const std::function<AlgExpr (std::vector<AlgExpr> &&)> & action)
{
    this->functions.insert({name, FunctionLiteral(name, amount_of_vars, action)});
}

void ScriptsNameSpace::changeFunction(const QString &function, int amount_of_vars, const std::function<AlgExpr (std::vector<AlgExpr> &&)> &new_action)
{
    auto it = this->functions.find(function);
    while (it != this->functions.end() && it->first == function)
    {
        if (it->second.amountOfArguments() ==amount_of_vars)
            it->second.setAction(new_action);
        ++it;
    }
}

void ScriptsNameSpace::addCondition(const RootCondition &cond)
{
    this->conditions.push_back(cond);
}

const std::vector<RootCondition> &ScriptsNameSpace::getConditions() const
{
    return conditions;
}

const FunctionLiteral &ScriptsNameSpace::getFunctionWithThatAmountOfArguments(const QString &function, int amount_of_vars) const
{
    auto it = this->functions.find(function);
    while (it != this->functions.end() && it->first == function)
    {
        if (it->second.amountOfArguments() ==amount_of_vars)
            return it->second;
        ++it;
    }
}


