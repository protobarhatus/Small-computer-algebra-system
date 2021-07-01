#include "parser.h"
#include <QRegExp>
#include <stack>
#include "algebra/algexpr.h"
#include "vectorvalue.h"
#include "matrixvalue.h"
bool isOpenBreaket( QChar  sym)
{
    return sym == '(' || sym == '[';
}
bool isClosingBreaket( QChar  sym)
{
    return sym == ')' || sym == ']';
}
bool isOpenGroupSeparator(QChar sym)
{
    return sym == '(' || sym == '[' || sym == '{';
}
bool isClosingGroupSeparator(QChar sym)
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
bool isEnglishLetter(QChar sym)
{
    return (sym >= 'a' && sym <= 'z') || (sym >= 'A' && sym <= 'Z');
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
        if (isOpenGroupSeparator(it))
            ++amount_of_breakets;
        if (isClosingGroupSeparator(it))
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
MathExpression parseFunction(const QString & func, const ScriptsNameSpace & scripts_space)
{
    auto params = textParseFunction(func);
    QString name = params.first;


    std::vector<MathExpression> args;
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
std::pair<MathExpression, bool> tryToParseImplicitMultiplicationOfNumberAndStuff(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    if (expr.contains('^'))
        return {nullptr, false};
    if (expr.size() < 2 || !isDigit(expr[0]))
        return {nullptr, false};
    int index = 0;
    while (index < expr.size() && isDigit(expr[index]))
        ++index;
    if (index == expr.size())
        return {nullptr, false};
    MathExpression num = parseAndComplete(expr.left(index), scripts_space);
    MathExpression right = parseAndComplete(expr.right(expr.size() - index), scripts_space);
    return {num * right, true};
}
MathExpression parseIntegerNumber(const QString & expr)
{
    return MathExpression(AlgExpr(expr.toInt()));
}
//возвращает строку со скобками и индекс после соответствующей закрывающей скобки
std::pair<QString, int> cutOffBreaketGroup(const QString & expr, int ind_of_open)
{
    QString res;
    int balance = 1;
    for (ind_of_open++; ind_of_open < expr.size() && balance > 0; ++ind_of_open)
    {
        if (isOpenBreaket(expr[ind_of_open]))
            ++balance;
        res += expr[ind_of_open];
        if (isClosingBreaket(expr[ind_of_open]))
            --balance;
    }
    if (balance > 0)
        throw QIODevice::tr("Отсутствует закрывающая скобка в \"") + expr + "\"";
    return {res, ind_of_open};
}
std::pair<QString, int> cutOffSingleExprUnit(const QString & expr, int start)
{
    QString res;
    if (isOpenBreaket(expr[start]))
        return cutOffBreaketGroup(expr, start);
    if (isDigit(expr[start]))
    {
        while (start < expr.length() && isDigit(expr[start]))
            res += expr[start++];
        return {res, start};
    }
    while (start < expr.length() && isEnglishLetter(expr[start]))
        res += expr[start++];
    return {res, start};

}
//придется учитывать степени скобок, чтобы можно было писать по типу (x-3)(x+2)^2(x+3) (это равно (x-3)*(x+2)^2*(x+3)
//конструкции вида x(x + 3) не позволяются, т. к. x рассматривается только как функция с аргументов x + 3. Потому что иначе sin(x)^2 не работает (считается как sin* x^2)
std::list<QString> tryToSplitAmongBreaketsMultiplications(const QString & expr)
{
    QString current_mult;
    std::list<QString> res;
    int breakets_level = 0;
    bool meeted_open_breaket = false;
    for (int i = 0; i  < expr.size(); ++i)
    {
        if (isOpenBreaket(expr[i]))
        {
            meeted_open_breaket = true;
            //условие для того чтобы учитывать ситуацию a^(x)
            if (breakets_level == 0 && (i == 0 || expr[i - 1] != '^'))
            {
                if (current_mult != "")
                    res.push_back(current_mult);
                current_mult = "";
            }
            else
                current_mult += expr[i];
            ++breakets_level;
        }
        else if (isClosingBreaket(expr[i]))
        {
            if (breakets_level == 0)
                throw "Syntax error: extra closing breaket in \"" + expr + "\"";
            --breakets_level;
            if (breakets_level == 0)
            {
                if (i < expr.size() - 2 && expr[i + 1] == '^' && (isEnglishLetter(expr[i + 2]) || isOpenBreaket(expr[i + 2]) || isDigit(expr[i + 2])))
                {
                    auto deg = cutOffSingleExprUnit(expr, i + 2);
                    res.push_back("(" + current_mult + ")^" + deg.first);
                    //deg.second будет указывать на символ после окончания юнита (числа/перменной/скобки), то есть
                    //на следующую открывающую скобку, но
                    //так как i еще раз увеличится в конце цикла, мы пропустим эту открывающую скобку
                    i = deg.second - 1;
                }
                else
                {
                    if (current_mult != "")
                        res.push_back(current_mult);
                }
                current_mult = "";
            }
            else
                current_mult += expr[i];
        }
        else if (meeted_open_breaket)
            current_mult += expr[i];
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
MathExpression parseDerivativeObject(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    /*int index = 0;
    while (index < expr.size() && expr[index] != '\'')
        ++index;

    QString name = expr.left(index);
    if (name == 'x')
        throw (QString)"x is considered as argument of "*/
    int order = expr.size() - 1;

    return derivObj(scripts_space.getVariable("y").getAlgExprValue(), scripts_space.getVariable("x").getAlgExprValue(), order);
}
bool isMinusMultiplySomething(const QString & expr)
{
    return expr.size() > 1 && expr[0] == '-';
}
MathExpression parseMinusMultiplySomething(const QString & expr, const ScriptsNameSpace & scripts_space)
{
    return AlgExpr(-1) * parseAndComplete(expr.right(expr.length() - 1), scripts_space);
}
MathExpression parseContainer(QString  expr, const ScriptsNameSpace & scripts_space)
{
    expr = expr.mid(1, expr.size() - 2);
    auto args = tryToSplitTokenAmongActions(expr, {';'},';');
    if (args.size() == 0)
        throw QIODevice::tr("Необходимо указать хотя бы один элемент в контейнере");
    std::vector<MathExpression> parsed_args(args.size());
    int i = 0;
    for (auto &it : args)
    {
        parsed_args[i] = parseAndComplete(it.second, scripts_space);
        ++i;
    }
    bool is_all_argexpr = true;
    bool is_all_vectors = true;
    for (auto &it : parsed_args)
    {
        if (it.getType() != VALUE_ALGEBRAIC_EXPRESSION)
            is_all_argexpr = false;
        if (it.getType() != VALUE_VECTOR)
            is_all_vectors = false;
    }
    if (is_all_argexpr)
    {
        Vector<AlgExpr> vec = Vector<AlgExpr>::create(parsed_args.size());
        for (int i = 0; i < parsed_args.size(); ++i)
            vec[i] = parsed_args[i].getAlgExprValue();
        return MathExpression(std::unique_ptr<AbstractValue>(new VectorValue(std::move(vec))));
    }
    if (is_all_vectors)
    {
        Matrix<AlgExpr> mat(parsed_args.size(), parsed_args[0].getVectorValue().size());
        for (int i = 0; i < parsed_args.size(); ++i)
        {
            if (parsed_args[i].getVectorValue().size() != mat.columns())
                throw QIODevice::tr("Строки в матрице должны быть одинаковой длины");
            mat[i] = parsed_args[i].getVectorValue();
        }
        return MathExpression(std::unique_ptr<AbstractValue>(new MatrixValue(std::move(mat))));
    }
    throw QIODevice::tr("Элементы контейнера должны быть однородны");
}
bool isContainer(const QString& expr)
{
    if (!  (expr[0] == "{" && expr.back() == "}"))
        return false;
    int fig_breaket_balance = 1;
    for (int i = 1; i < expr.length(); ++i)
    {
        if (expr[i] == '{')
            ++fig_breaket_balance;
        if (expr[i] == '}')
            --fig_breaket_balance;
        if (fig_breaket_balance == 0 && i != expr.length() - 1)
            return false;
    }
    return true;
}
MathExpression parseAndComplete(QString expr, const ScriptsNameSpace & scripts_space)
{
    expr = deleteSpaces(expr);
    expr = deleteOuterBreakets(expr);
    if (expr.length() == 0)
        throw (QString)"Syntax error: expected expression";
    MathExpression result(nullptr);

    if (isContainer(expr))
        return parseContainer(expr, scripts_space);

    auto sum = tryToSplitTokenAmongActions(expr, toSet({'+', '-'}), '+');
    if (sum.size() > 1)
    {
        auto it = sum.begin();
        result = parseAndComplete(it->second, scripts_space);
        for (++it; it != sum.end(); ++it)
        {
            if (it->first == '+')
                result = result + parseAndComplete(it->second, scripts_space);
            else
                result = result - parseAndComplete(it->second, scripts_space);
        }
        return result;
    }

    auto mult = tryToSplitTokenAmongActions(expr, toSet({'*', '/'}), '*');
    if (mult.size() > 1)
    {
        if (mult.begin()->first == '/')
            throw (QString)"Syntax error: missed epxression before '/'\"" + expr + "\"";
        auto it = mult.begin();
        result = parseAndComplete(it->second, scripts_space);
        for (++it; it != mult.end(); ++it)
        {
            if (it->first == '*')
                result = result * parseAndComplete(it->second, scripts_space);
            else
                result = result / parseAndComplete(it->second, scripts_space);
        }
        return result;
    }
    if (isMinusMultiplySomething(expr))
        return parseMinusMultiplySomething(expr, scripts_space);


    auto impl_mult_res = tryToParseImplicitMultiplicationOfNumberAndStuff(expr, scripts_space);
    if (impl_mult_res.second)
        return std::move(impl_mult_res.first);

    if (isFunction(expr, scripts_space))
        return parseFunction(expr, scripts_space);
    if (isVariable(expr, scripts_space))
        return scripts_space.getVariable(expr);
    if (isIntegerNumber(expr))
        return parseIntegerNumber(expr);

    auto mult_split = tryToSplitAmongBreaketsMultiplications(expr);
    if (mult_split.size() > 1)
    {
        auto it = mult_split.begin();
        MathExpression res = parseAndComplete(*it, scripts_space);
        for (++it; it != mult_split.end(); ++it)
            res = res * parseAndComplete(*it, scripts_space);
        return res;
    }


    auto degs = tryToSplitTokenAmongActions(expr, toSet({'^'}), ' ');
    if (degs.size() > 1)
    {
        if (degs.begin()->first == '^')
            throw (QString)"Syntax error: missed expression before '^' \"" + expr + "\"";
        result = parseAndComplete(degs.back().second, scripts_space);
        if (result.getType() != VALUE_ALGEBRAIC_EXPRESSION)
            throw QString("Only objects of elementary algebra can be raised to power");
        for (auto it = next(degs.rbegin()); it != degs.rend(); ++it)
            result = pow(parseAndComplete(it->second, scripts_space).getAlgExprValue(), std::move(result.getAlgExprValue()));
        return result;
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
bool isAppropriateVariableName(const QString &str)
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
bool isAppropriateVarialbeSignature(const QString &str)
{
    auto sign = str.split(' ');
    if (sign.size() == 1)
        return isAppropriateVariableName(*sign.begin());
    if (sign.size() > 2)
        return false;
    return valueTypeFromStringName(*sign.begin()) != VALUE_INCORRECT && isAppropriateVariableName(*++sign.begin());
}
