#include "parser.h"
#include <QRegExp>
#include <stack>
bool isOpenBreaket( QChar  sym)
{
    return sym == '(' || sym == '[' || sym == '{';
}
bool isClosingBreaket( QChar  sym)
{
    return sym == ')' || sym == ']' || sym == '}';
}
bool isBreaket( QChar sym)
{
    return isOpenBreaket(sym) || isClosingBreaket(sym);
}
bool isDigit(QChar sym)
{
    return sym >= '0' && sym <= '9';
}
QChar breaketsPair( QChar sym)
{
    if (sym == '(')
        return ')';
    if (sym == '[')
        return ']';
    if (sym == '{')
        return '}';
    if (sym == ')')
        return '(';
    if (sym == ']')
        return '[';
    if (sym == '}')
        return '{';
    throw (QString)"Internal error";
}
QString deleteSpaces(const QString & expr)
{
    QString res;
    for (auto &it : expr)
        if (it != ' ')
            res += it;
    return res;
}
QString deleteOuterBreakets(const QString & expr)
{
    std::vector<int> open_breakets_pair(expr.length());
    std::stack<int> open_breakets;
    for (int i = 0; i < expr.length(); ++i)
    {
        if (isOpenBreaket(expr[i]))
        {
            open_breakets.push(i);
        }
        if (isClosingBreaket(expr[i]))
        {
            if (open_breakets.size() == 0)
                throw (QString)"Syntax error: extra closing breaket in \"" + expr + "\"";
            open_breakets_pair[open_breakets.top()] = i;
            open_breakets.pop();
        }
    }
    QString res;
    bool erasing = true;
    int amount_of_erased = 0;
    for (int i = 0; i < expr.size() - amount_of_erased; ++i)
    {
        if (erasing)
        {
            if (isBreaket(expr[i]) && expr[expr.size() - 1 - i] == breaketsPair(expr[i]) &&
                    open_breakets_pair[i] == expr.size() - 1 - i)
            {
                ++amount_of_erased;
                continue;
            }
            else
                erasing = false;
        }
        res += expr[i];
    }
    return res;
}
std::set<QChar> toSet(std::initializer_list<QChar> && list)
{
    std::set<QChar> res;
    for (auto &it : list)
        res.insert(it);
    return res;
}

std::list<std::pair<QChar, QString>> tryToSplitTokenAmongActions(const QString & expr, const std::set<QChar> & separators, QChar defaul_action)
{
    auto isOneOfActionSimbols = [&separators](const QChar & sym) {
        return separators.find(sym) != separators.end();
    };
    std::list<std::pair<QChar, QString>> res;
    int amount_of_breakets = 0;
    for (auto &it : expr)
    {
        if (isOpenBreaket(it))
            ++amount_of_breakets;
        if (isClosingBreaket(it))
            --amount_of_breakets;
        if (amount_of_breakets < 0)
            throw (QString)("Syntax error: extra closing breaket in \"" + expr + "\"");

        if (amount_of_breakets == 0 && isOneOfActionSimbols(it))
        {
            if (res.size() > 0 && res.back().second.length() == 0)
                throw (QString)("Syntax error: two operators in a row in \"" + expr + "\"");
            res.push_back({it, ""});
        }
        else
        {
            if (res.empty())
                res.push_back({defaul_action, it});
            else
                res.back().second += it;
        }
    }
    if (amount_of_breakets > 0)
        throw "Syntax error: need closing breaket in \"" + expr + "\"";
    return res;
}

bool isStartNameAvailableLitera(QChar sym)
{
    return (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z') || sym == '_';
}
bool isNameAvailableLitera(QChar sym)
{
    return isStartNameAvailableLitera(sym) || isDigit(sym) || sym == '_';
}
bool isFunction(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    if (expr.size() <= 2)
        return false;
    if (!isStartNameAvailableLitera(expr[0]))
        return false;
    QString name = expr[0];
    for (int i = 1; i < expr.size(); ++i)
    {
        if (isOpenBreaket(expr[i]))
        {
            return scripts_space.hasFunction(name) && expr.back() == breaketsPair(expr[i]);
        }
        if (!isNameAvailableLitera(expr[i]))
            return false;
        name += expr[i];
    }
    return false;

}
std::pair<QString, std::vector<QString>> textParseFunction(const QString & func)
{
    QString name = "";
    int ind_of_breaket = 0;
    for (int i = 0; i < func.size(); ++i)
    {
        if (isOpenBreaket(func[i]))
        {
            ind_of_breaket = i;
            break;
        }
        name += func[i];
    }
    std::vector<QString> arguments;
    //это слегка тупо, но сойдет
    QString arguments_string = func.mid(ind_of_breaket + 1);
    arguments_string = arguments_string.left(arguments_string.size() - 1);

    auto args_str = tryToSplitTokenAmongActions(arguments_string, toSet({','}), ' ');
    if (args_str.begin()->first == ',')
        throw (QString)"Syntax error: empty argument in \"" + func + "\"";
    std::vector<QString> args;
    for (auto &it : args_str)
    {
        if (it.second.length() == 0)
            throw (QString)"Syntax error: empty argument in \"" + func + "\"";
        args.push_back(it.second);
    }
    return {std::move(name), std::move(args)};
}
//ну естесна если надо делать замыкания, то это нужно переписывать, но вряд ли я их буду делать
AlgExpr parseFunction(const QString & func, const ScriptsNameSpace & scripts_space)
{
    auto params = textParseFunction(func);
    QString name = params.first;


    std::vector<AlgExpr> args;
    for (auto &it : params.second)
    {
        if (it.length() == 0)
            throw (QString)"Syntax error: empty argument in \"" + func + "\"";
        args.push_back(parseAndComplete(it, scripts_space));
    }
    if (!scripts_space.hasFunctionWithThatAmountOfArguments(name, args.size()))
        throw (QString)"Syntax error: wrong amount of arguments in function \'" + name + "\' in \"" + func + "\"";

    return scripts_space.callFunctionsAction(name, std::move(args));

}
bool isVariable(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    return scripts_space.hasVariable(expr);
}

bool isIntegerNumber(const QString & expr)
{
    for (auto &it : expr)
        if (!isDigit(it))
            return false;
    return true;
}
std::pair<AlgExpr, bool> tryToParseImplicitMultiplicationOfNumberAndStuff(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    if (expr.size() < 2 || !isDigit(expr[0]))
        return {0, false};
    int index = 0;
    while (index < expr.size() && isDigit(expr[index]))
        ++index;
    if (index == expr.size())
        return {0, false};
    AlgExpr num = parseAndComplete(expr.left(index), scripts_space);
    AlgExpr right = parseAndComplete(expr.right(expr.size() - index), scripts_space);
    return {num * right, true};
}
AlgExpr parseIntegerNumber(const QString & expr)
{
    return expr.toInt();
}
std::list<QString> tryToSplitAmongBreaketsMultiplications(const QString & expr)
{
    QString current_mult;
    std::list<QString> res;
    int breakets_level = 0;
    for (auto &it : expr)
    {
        if (isOpenBreaket(it))
        {
            if (breakets_level == 0)
            {
                if (current_mult != "")
                    res.push_back(current_mult);
                current_mult = "";
            }
            else
                current_mult += it;
            ++breakets_level;
        }
        else if (isClosingBreaket(it))
        {
            if (breakets_level == 0)
                throw "Syntax error: extra closing breaket in \"" + expr + "\"";
            --breakets_level;
            if (breakets_level == 0)
            {
                if (current_mult != "")
                    res.push_back(current_mult);
                current_mult = "";
            }
            else
                current_mult += it;
        }
        else
            current_mult += it;
    }
    if (current_mult != "")
        res.push_back(current_mult);
    return res;
}
//возможно это потом придется исправить, но пока объект производной может быть только от y и по x
bool isDerivativeObject(const QString & expr)
{
   // return expr.contains(QRegExp("^\\w[\\w\\d]*\'+$"));
    return expr.contains(QRegExp("^y\'+$"));
}
AlgExpr parseDerivativeObject(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    /*int index = 0;
    while (index < expr.size() && expr[index] != '\'')
        ++index;

    QString name = expr.left(index);
    if (name == 'x')
        throw (QString)"x is considered as argument of "*/
    int order = expr.size() - 1;

    return derivObj(scripts_space.getVariable("y"), scripts_space.getVariable("x"), order);
}
bool isMinusMultiplySomething(const QString & expr)
{
    return expr.size() > 1 && expr[0] == '-';
}
AlgExpr parseMinusMultiplySomething(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    return -1 * parseAndComplete(expr.right(expr.length() - 1), scripts_space);
}
AlgExpr parseAndComplete(QString expr, const ScriptsNameSpace & scripts_space)
{
    expr = deleteSpaces(expr);
    expr = deleteOuterBreakets(expr);
    if (expr.length() == 0)
        throw (QString)"Syntax error: expected expression";
    AlgExpr result;

    auto sum = tryToSplitTokenAmongActions(expr, toSet({'+', '-'}), '+');
    if (sum.size() > 1)
    {
        result = 0;
        for (auto &it : sum)
            if (it.first == '+')
                result += parseAndComplete(it.second, scripts_space);
            else
                result -= parseAndComplete(it.second, scripts_space);
        return result;
    }

    auto mult = tryToSplitTokenAmongActions(expr, toSet({'*', '/'}), '*');
    if (mult.size() > 1)
    {
        if (mult.begin()->first == '/')
            throw (QString)"Syntax error: missed epxression before '/'\"" + expr + "\"";
        result = 1;
        for (auto &it : mult)
            if (it.first == '*')
                result *= parseAndComplete(it.second, scripts_space);
            else
                result /= parseAndComplete(it.second, scripts_space);
        return result;
    }
    auto degs = tryToSplitTokenAmongActions(expr, toSet({'^'}), ' ');
    if (degs.size() > 1)
    {
        if (degs.begin()->first == '^')
            throw (QString)"Syntax error: missed expression before '^' \"" + expr + "\"";
        result = parseAndComplete(degs.back().second, scripts_space);
        for (auto it = next(degs.rbegin()); it != degs.rend(); ++it)
            result = pow(parseAndComplete(it->second, scripts_space), std::move(result));
        return result;
    }
    if (isMinusMultiplySomething(expr))
        return parseMinusMultiplySomething(expr, scripts_space);
    if (isFunction(expr, scripts_space))
        return parseFunction(expr, scripts_space);
    if (isVariable(expr, scripts_space))
        return scripts_space.getVariable(expr);
    if (isIntegerNumber(expr))
        return parseIntegerNumber(expr);
    auto impl_mult_res = tryToParseImplicitMultiplicationOfNumberAndStuff(expr, scripts_space);
    if (impl_mult_res.second)
        return impl_mult_res.first;

    auto mult_split = tryToSplitAmongBreaketsMultiplications(expr);
    if (mult_split.size() > 1)
    {
        AlgExpr res = 1;
        for (auto &it : mult_split)
            res *= parseAndComplete(it, scripts_space);
        return res;
    }
    if (isDerivativeObject(expr))
        return parseDerivativeObject(expr, scripts_space);

    throw (QString)"Syntax error: uknown token \"" + expr + "\"";

}











RequestType defineTypeOfRequest(const QString &request)
{
    int double_eq = request.count("==");
    if (double_eq > 1)
        return REQUEST_TYPE_WRONG_REQUEST;
    if (double_eq == 1)
    {
        if (request.count("=") == 2)
            return REQUEST_TYPE_EQUATION;
        return REQUEST_TYPE_WRONG_REQUEST;
    }
    int eq = request.count("=");
    if (eq > 1)
        return REQUEST_TYPE_WRONG_REQUEST;
    if (eq == 1)
        return REQUEST_TYPE_ASSIGNATION;
    return REQUEST_TYPE_EXPRESSION;
}

bool isAppropriateVarialbeName(const QString &str)
{
    if (!isStartNameAvailableLitera(str[0]))
        return false;
    for (int i = 1; i < str.length(); ++i)
    {
        if (!isNameAvailableLitera(str[i]))
            return false;
    }
    return true;
}
