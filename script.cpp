#include "script.h"
#include "parser.h"
Script::Script()
{

}

Script::Script(const Script &script)
{
    this->commands = script.commands;
}

Script::Script(Script &&script)
{
    this->commands = std::move(script.commands);
}

Script &Script::operator=(const Script &script)
{
    this->commands = script.commands;
    return *this;
}

Script &Script::operator=(Script &&script)
{
    this->commands = std::move(script.commands);
    return *this;
}

void Script::setCommands(const std::vector<QString> &commands)
{
    this->commands = commands;
}
QString executeEquationSolving(QString equation, const ScriptsNameSpace & space)
{
    equation = deleteOuterBreakets(deleteSpaces(equation));
    std::pair<QString, std::vector<QString>> func_solve;
    try {
        func_solve = textParseFunction(equation);
    } catch (const QString & mes) {

    }
    auto buildAnswerFromNonDifferentialEquation = [&space](const AlgExpr & equation, const QString & var)->QString
    {
        std::list<AlgExpr> roots;
        try {
            roots = solveEquation(equation, parseAndComplete(var, space));
        } catch (...) {
            return QIODevice::tr("Нет корней или программа не может найти корни");
        }

        if (roots.size() == 0)
            return QIODevice::tr("Нет корней или программа не может найти корни");
        std::vector<AlgExpr> roots_vectors;
        for (auto &it : roots)
            roots_vectors.push_back(it);
        auto selectes = selectRootsAndConditions(roots_vectors, space.getConditions(), parseAndComplete(var, space).getExpr()->getId());
        QString res;
        for (auto &it : selectes.first)
            res += var + " = " + it.toString() + "<br>";
        if (selectes.second.size() > 0)
        {
            res += "AND {<br>";
            for (auto &it : selectes.second)
                res += it.toString() + "<br>,";
            res += "}<br>";
        }
        return res;

    };
    if (func_solve.first == "SolveFor")
    {
        if (func_solve.second.size() == 2)
        {
            if (func_solve.second[0].contains("=="))
            {
                if (isVariable(func_solve.second[1], space))
                {
                    AlgExpr left = parseAndComplete(func_solve.second[0].left(func_solve.second[0].indexOf("==")), space);
                    AlgExpr right = parseAndComplete(func_solve.second[0].right(func_solve.second[0].length() - func_solve.second[0].indexOf("==") - 2), space);
                    AlgExpr eq = left - right;
                    if (isDifferentialEquation(eq))
                        throw QIODevice::tr("Функция 'SolveFor' может быть применена только для недифференциальных уравнений");
                    return buildAnswerFromNonDifferentialEquation(eq, func_solve.second[1]);
                }
                else
                    throw QIODevice::tr("Второй аргумент функции 'SolveFor' должен быть переменной");
            }
            else
                throw QIODevice::tr("Первый аргумент функции 'SolveFor' должен быть корректным уравнением ");
        }
        else
            throw QIODevice::tr("Функция 'SolveFor' должна иметь два аргумента");
        }

    AlgExpr left = parseAndComplete(equation.left(equation.indexOf("==")), space);
    AlgExpr right = parseAndComplete(equation.right(equation.length() - equation.indexOf("==") - 2), space);
    auto eq = left - right;
    if (isDifferentialEquation(eq))
    {
        //да, тут привязываемся к конкретным символам
        /*AlgExpr x = var();
        AlgExpr y = var();
        AlgExpr dx = D(x);
        AlgExpr dy = D(y);
        qDebug() << (x*derivObj(y, x, 1) - y).toString();
        auto res1 = solveDifur(x*derivObj(y, x, 1) - y , x, y);
        qDebug() << "RES: ";
        for (auto &it : res1)
            qDebug() << it.toString();
        qDebug() << eq.toString();*/
        auto roots = solveDifur(eq, parseAndComplete("x", space), parseAndComplete("y", space));
        if (roots.first.size() == 0)
            return QIODevice::tr("Нет решений или программа не может их найти");
        for (auto &it : roots.first)
            downgradeIntegratingConstantsIndexes(it.expr());
        std::vector<DifurResult> rtsvec;
        for (auto &it : roots.first)
            rtsvec.push_back(it);
        auto selected = selectRootsAndConditions(rtsvec, space.getConditions(), parseAndComplete("y", space).getExpr()->getId());
        QString res;
        for (auto &it : selected.first)
            res += it.toString() + "<br>";
        if (selected.second.size() > 0)
        {
            res += "AND {<br>";
            for (auto &it : selected.second)
                res += it.toString() + ",<br>";
            res += "}<br>";
        }
        res += "#^<" + equation + "#>";
        for (auto &it : roots.second)
            res += std::move(it) + "<br>";
        res += "#$";
        return res;
    }
    auto vars = eq.getExpr()->getSetOfVariables();
    if (vars.size() > 1)
        throw QIODevice::tr("Для уравнения с двумя и более переменными используйте функцию \'SolveFor\'");
    if (vars.size() == 0)
        throw QIODevice::tr("Для уравнения необходима хоть одна переменная");
    return buildAnswerFromNonDifferentialEquation(eq, getVariableExpr(*vars.begin())->toString());
}
void executeAssignation(QString command, ScriptsNameSpace & space)
{
    command = deleteOuterBreakets(deleteSpaces(command));
    QString left = command.left(command.indexOf("="));
    QString right = command.right(command.length() - command.indexOf("=") - 1);
    if (isAppropriateVarialbeName(left))
    {
        if (isVariable(left, space))
        {
            if (space.canChangeVariable(left))
            {
                space.changeVariable(left, parseAndComplete(right, space));
            }
            else
                throw QIODevice::tr("Нельзя присваивать значение константам");
        }
        else
        {
            space.addVariable(left, parseAndComplete(right, space));
        }
        return;
    }
    std::pair<QString, std::vector<QString>> func;
    try {
        func = textParseFunction(left);
    } catch (const QString & expr) {
        throw QIODevice::tr("Левая часть должна быть либо переменной, либо функцией");
    }
    if (func.second.empty())
        throw QIODevice::tr("Задаваемая пользователем функция должна иметь хотя бы один аргумент. Иначе, задайте переменную");

    ScriptsNameSpace function_name_space = space;
    for (auto &it : func.second)
    {
        if (!isAppropriateVarialbeName(it))
            throw QIODevice::tr("Имена аргументов функций должны быть переменными");
        if (isVariable(it, space))
            throw QIODevice::tr("Запрещено использовать существующие переменные как имена аргументов функций");

        function_name_space.addVariable(it, systemVarExpr());
    }

    AlgExpr func_def = parseAndComplete(right, function_name_space);
    std::vector<int> arg_vars_indexes(func.second.size());
    for (int i = 0; i < func.second.size(); ++i)
    {
        arg_vars_indexes[i] = parseAndComplete(func.second[i], function_name_space).getExpr()->getId();
    }
    auto action = [func_def, arg_vars_indexes](std::vector<AlgExpr> && args)->AlgExpr {
        AlgExpr res = func_def;
        for (int i = 0; i < args.size(); ++i)
        {
            setUpExpressionIntoVariable(res.getExpr(), args[i].getExpr(), arg_vars_indexes[i]);
        }
        return res;

    };
    if (space.hasFunctionWithThatAmountOfArguments(func.first, func.second.size()))
        space.changeFunction(func.first, func.second.size(), action);
    else
        space.addFunction(func.first, func.second.size(), action);

}
std::vector<CommandResponse> Script::execute(const std::vector<QString> &commands)
{
    this->setCommands(commands);
    std::vector<CommandResponse> result(commands.size());
    ScriptsNameSpace space;
    int input_counter = 0;
    for (int i = 0; i < commands.size(); ++i)
    {
        if (commands[i].length() == 0 || commands[i][0] == systemUnreadableSymbol())
        {
            result[i] = skippedCommandRespond();
            continue;
        }
        auto request_type = defineTypeOfRequest(commands[i]);
        if (request_type == REQUEST_TYPE_WRONG_REQUEST)
        {
            result[i] = errorCommandRespond("Wrong request");
            continue;
        }
        else if (request_type == REQUEST_TYPE_EXPRESSION)
        {
            try {
                AlgExpr res = parseAndComplete(commands[i], space);
                result[i] = acceptedInputCommandRespond(input_counter++, res.toString());
            } catch (const QString &error) {
                result[i] = errorCommandRespond(error);
            } catch (...) {
                result[i] = errorCommandRespond("");
            }
        }
        else if (request_type == REQUEST_TYPE_EQUATION)
        {
            try {
                QString res = executeEquationSolving(commands[i], space);
                result[i] = acceptedInputCommandRespond(input_counter++, res);
            } catch (const QString &error) {
                result[i] = errorCommandRespond(error);
            } catch (...) {
                result[i] = errorCommandRespond("");
            }
        }
        else if (request_type == REQUEST_TYPE_ASSIGNATION)
        {
            try {
                executeAssignation(commands[i], space);
                result[i] = skippedCommandRespond();
            } catch (const QString &error) {
                result[i] = errorCommandRespond(error);
            } catch (...) {
                result[i] = errorCommandRespond("");
            }
        }

    }
    return result;

}


QChar systemUnreadableSymbol()
{
    return QChar(1517);
}
