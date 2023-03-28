#include "script.h"
#include "parser.h"
#include "algebra/algexpr.h"
#include "functioninterpretationtoken.h"
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
//это конечно ужас потом надо отрефакторить
std::vector<QString> executeEquationSolving(QString equation, const ScriptsNameSpace & space)
{
    equation = deleteOuterBreakets(deleteSpaces(equation));
    std::pair<QString, std::vector<QString>> func_solve;
    try {
        func_solve = textParseFunction(equation);
    } catch (const QString & mes) {

    }
    auto buildAnswerFromNonDifferentialEquation = [&space](const AlgExpr & equation, const QString & var)->std::vector<QString>
    {
        std::list<AlgExpr> roots;
        try {
            auto varexpr = parseAndComplete(var, space);
            if (varexpr.getType() != VALUE_ALGEBRAIC_EXPRESSION)
                throw QIODevice::tr("Решать (пока что) можно только для переменных, представляющих объекты элементарной алгебры");
            roots = solveEquation(equation, varexpr.getAlgExprValue());
        } catch (...) {
            return {QIODevice::tr("String(\"Нет корней или программа не может найти корни\")"),
                        QIODevice::tr("Нет корней или программа не может найти корни")};
        }

        if (roots.size() == 0)
            return {QIODevice::tr("String(\"Нет корней или программа не может найти корни\")"),
                        QIODevice::tr("Нет корней или программа не может найти корни")};
        std::vector<AlgExpr> roots_vectors;
        for (auto &it : roots)
            roots_vectors.push_back(it);
        auto selectes = selectRootsAndConditions(roots_vectors, space.getConditions(), parseAndComplete(var, space).getAlgExprValue().getExpr()->getId());
        /*QString res;
        for (auto &it : selectes.first)
            res += var + " = " + it.toString() + "<br>";
        if (selectes.second.size() > 0)
        {
            res += "AND {<br>";
            for (auto &it : selectes.second)
                res += it.toString() + "<br>,";
            res += "}<br>";
        }
        return res;*/
        QString render_string_res;
        if (selectes.second.size() > 0)
            render_string_res = "System(";
        if (selectes.first.size() == 1)
            render_string_res += selectes.first.begin()->makeRenderString();
        else
        {
            render_string_res += "[Matrix(1," + QString::number(selectes.first.size());
            for (auto &it : selectes.first)
                render_string_res += "," + var + "&Symbol(32)=Symbol(32)&" +it.makeRenderString();
            render_string_res += "]";
        }

        if (selectes.second.size() > 0)
        {
            render_string_res += ",";
            for (auto &it : selectes.second)
                render_string_res += it.makeRenderString()+",";
            render_string_res = render_string_res.left(render_string_res.length() - 1);
            render_string_res += ")";
        }

        QString wolfram_res;
        for (auto &it : selectes.first)
            wolfram_res += var + " = " + it.toString() + "<br>";
        if (selectes.second.size() > 0)
        {
            wolfram_res += "AND {<br>";
            for (auto &it : selectes.second)
                wolfram_res += it.toString() + "<br>,";
            wolfram_res += "}<br>";
        }
        return {render_string_res, wolfram_res};

    };
    if (func_solve.first == "SolveFor")
    {
        if (func_solve.second.size() == 2)
        {
            if (func_solve.second[0].contains("=="))
            {
                if (isVariable(func_solve.second[1], space))
                {
                    MathExpression left = parseAndComplete(func_solve.second[0].left(func_solve.second[0].indexOf("==")), space);
                    MathExpression right = parseAndComplete(func_solve.second[0].right(func_solve.second[0].length() - func_solve.second[0].indexOf("==") - 2), space);
                    MathExpression eq = left - right;
                    if (eq.getType() != VALUE_ALGEBRAIC_EXPRESSION)
                        throw QIODevice::tr("Программа решает уравнения только с объектами элементарной алгебры");
                    if (isDifferentialEquation(eq.getAlgExprValue()))
                        throw QIODevice::tr("Функция 'SolveFor' может быть применена только для недифференциальных уравнений");
                    return buildAnswerFromNonDifferentialEquation(eq.getAlgExprValue(), func_solve.second[1]);
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

    MathExpression left = parseAndComplete(equation.left(equation.indexOf("==")), space);
    MathExpression right = parseAndComplete(equation.right(equation.length() - equation.indexOf("==") - 2), space);
    auto eq = left - right;
    if (eq.getType() != VALUE_ALGEBRAIC_EXPRESSION)
        throw QIODevice::tr("Программа решает уравнения только с объектами элементарной алгебры");
    if (isDifferentialEquation(eq.getAlgExprValue()))
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
        auto roots = solveDifur(eq.getAlgExprValue(), parseAndComplete("x", space).getAlgExprValue()
                                , parseAndComplete("y", space).getAlgExprValue());
        if (roots.first.size() == 0)
            return {QIODevice::tr("String(\"Нет решений или программа не может их найти\")"),
                        QIODevice::tr("Нет решений или программа не может их найти")};
        for (auto &it : roots.first)
            downgradeIntegratingConstantsIndexes(it.expr());
        std::vector<DifurResult> rtsvec;
        for (auto &it : roots.first)
            rtsvec.push_back(it);
        auto selected = selectRootsAndConditions(rtsvec, space.getConditions(),
                                                 parseAndComplete("y", space).getAlgExprValue().getExpr()->getId());
        /*QString res;
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
        return res;*/
        QString render_string_res;
        if (selected.second.size() > 0)
            render_string_res = "System(";
        if (selected.first.size() == 1)
            render_string_res += selected.first.begin()->makeRenderString();
        else
        {
            render_string_res += "[Matrix(1," + QString::number(selected.first.size());
            for (auto &it : selected.first)
                render_string_res += "," + it.makeRenderString();
            render_string_res += "]";
        }

        if (selected.second.size() > 0)
        {
            render_string_res += ",";
            for (auto &it : selected.second)
                render_string_res += it.makeRenderString()+",";
            render_string_res = render_string_res.left(render_string_res.length() - 1);
            render_string_res += ")";
        }

        //да это вообще не вольфрам но и фиг с ним целью не стоит покрыть вообще весь синтаксис вольфрамом
        QString wolfram_string_res;
        for (auto &it : selected.first)
            wolfram_string_res += it.toString() + "<br>";
        if (selected.second.size() > 0)
        {
            wolfram_string_res += "AND {<br>";
            for (auto &it : selected.second)
                wolfram_string_res += it.toString() + ",<br>";
            wolfram_string_res += "}<br>";
        }
        wolfram_string_res += "#^<" + equation + "#>";
        for (auto &it : roots.second)
            wolfram_string_res += std::move(it) + "<br>";
        wolfram_string_res += "#$";
        return {render_string_res, wolfram_string_res};
    }
    auto vars = eq.getAlgExprValue().getExpr()->getSetOfVariables();
    if (vars.size() > 1)
        throw QIODevice::tr("Для уравнения с двумя и более переменными используйте функцию \'SolveFor\'");
    if (vars.size() == 0)
        throw QIODevice::tr("Для уравнения необходима хоть одна переменная");
    return buildAnswerFromNonDifferentialEquation(eq.getAlgExprValue(), getVariableExpr(*vars.begin())->toString());
}
void executeAssignation(QString command, ScriptsNameSpace & space)
{
    command = deleteOuterBreakets(deleteSpaces(command));
    QString left = command.left(command.indexOf("="));
    QString right = command.right(command.length() - command.indexOf("=") - 1);
    if (isAppropriateVarialbeSignature(left))
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
    for (int i = 0; i < func.second.size(); ++i)
    {
        if (!isAppropriateVarialbeSignature(func.second[i]))
            throw QIODevice::tr("Неверное объявление аргумента");
        if (isVariable(func.second[i], space))
            throw QIODevice::tr("Запрещено использовать существующие переменные как имена аргументов функций");

        function_name_space.addVariable(func.second[i], MathExpression(std::unique_ptr<AbstractValue>(
            new FunctionInterpretationToken(FunctionLiteral("__VAR__", 0,
                       [i](std::vector<MathExpression>&&arg) {return std::move(arg[i]);}, true),
        {}, true))));
    }

    MathExpression func_def = parseAndComplete(right, function_name_space);
    auto action = [func_def](std::vector<MathExpression> && args)->MathExpression {
        if (func_def.getType() == VALUE_FUNCTION_TOKEN)
            return func_def.executeFunction(args);
        return func_def;
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
            result[i] = errorCommandRespond({"Wrong request"});
            continue;
        }
        else if (request_type == REQUEST_TYPE_EXPRESSION)
        {
            try {
                MathExpression res = parseAndComplete(commands[i], space);
                result[i] = acceptedInputCommandRespond(input_counter++, {res.makeRenderString(), res.makeWolframString()});
            } catch (const QString &error) {
                result[i] = errorCommandRespond({error});
            } catch (...) {
                result[i] = errorCommandRespond({"Error"});
            }
        }
        else if (request_type == REQUEST_TYPE_EQUATION)
        {
            try {
                std::vector<QString> res = executeEquationSolving(commands[i], space);
                result[i] = acceptedInputCommandRespond(input_counter++, res);
            } catch (const QString &error) {
                result[i] = errorCommandRespond({error});
            } catch (...) {
                result[i] = errorCommandRespond({"Error"});
            }
        }
        else if (request_type == REQUEST_TYPE_ASSIGNATION)
        {
            try {
                executeAssignation(commands[i], space);
                result[i] = skippedCommandRespond();
            } catch (const QString &error) {
                result[i] = errorCommandRespond({error});
            } catch (...) {
                result[i] = errorCommandRespond({"Error"});
            }
        }

    }
    return result;

}


QChar systemUnreadableSymbol()
{
    return QChar(8291);
}
