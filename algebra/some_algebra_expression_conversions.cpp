#include "some_algebra_expression_conversions.h"

#include "fractal.h"
#include "polynomial.h"
#include "number.h"
#include "variable.h"
#include "degree.h"
#include "polynomial.h"
#include "exception.h"
#include "limits"
#include <memory>
#include "absolutevalue.h"
#include "constant.h"
#include "sinus.h"
#include "cosinus.h"
#include "tangent.h"
#include "cotangent.h"
#include "variablesdistributor.h"
#include "logarithm.h"
#include "differential.h"
#include "arctangent.h"
#include "arcsinus.h"
#include "derivativeobject.h"
QString unicode(int sym)
{
    return QChar(sym);
}
std::unique_ptr<AbstractExpression> makeAbstractExpression(AlgebraExpression type, const AbstractExpression * argument)
{
    assert(type == argument->getId() || (type > 0 && argument->getId() > 0));
    if (type > 0)
        return std::unique_ptr<AbstractExpression>(new Variable(*(static_cast<const Variable*>(argument))));
    switch(type)
    {
    case NUMBER:
        return std::unique_ptr<AbstractExpression>(new Number(*static_cast<const Number*>(argument)));
    case POLYNOMIAL:
        return std::unique_ptr<AbstractExpression>(new Polynomial(*static_cast<const Polynomial*>(argument)));
    case FRACTAL:
        return std::unique_ptr<AbstractExpression>(new Fractal(*static_cast<const Fractal*>(argument)));
    case DEGREE:
        return std::unique_ptr<AbstractExpression>(new Degree(*static_cast<const Degree*>(argument)));
    case ABSOLUTE_VALUE:
        return std::unique_ptr<AbstractExpression>(new AbsoluteValue(*static_cast<const AbsoluteValue*>(argument)));
    case CONSTANT:
        return std::unique_ptr<AbstractExpression>(new Constant(*static_cast<const Constant*>(argument)));
    case SINUS:
        return std::unique_ptr<AbstractExpression>(new Sinus(*static_cast<const Sinus*>(argument)));
    case COSINUS:
        return std::unique_ptr<AbstractExpression>(new Cosinus(*static_cast<const Cosinus*>(argument)));
    case TANGENT:
        return std::unique_ptr<AbstractExpression>(new Tangent(*static_cast<const Tangent*>(argument)));
    case COTANGENT:
        return std::unique_ptr<AbstractExpression>(new Cotangent(*static_cast<const Cotangent*>(argument)));
    case LOGARITHM:
        return std::unique_ptr<AbstractExpression>(new Logarithm(*static_cast<const Logarithm*>(argument)));
    case DIFFERENTIAL:
        return std::unique_ptr<AbstractExpression>(new Differential(*static_cast<const Differential*>(argument)));
    case ARCTANGENT:
        return std::unique_ptr<AbstractExpression>(new ArcTangent(*static_cast<const ArcTangent*>(argument)));
    case ARCSINUS:
        return std::unique_ptr<AbstractExpression> (new ArcSinus(*static_cast<const ArcSinus*>(argument)));
    case DERIVATIVE_OBJECT:
        return std::unique_ptr<AbstractExpression> (new DerivativeObject(*static_cast<const DerivativeObject*>(argument)));
    default:
        assert(false);

    }
}
abs_ex copy(const AbstractExpression * arg)
{
    if (arg == nullptr)
        return nullptr;
    auto res = makeAbstractExpression(arg->getId(), arg);
    /*auto set = res->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it) &&
                VariablesDistributor::amountOfVariable(it) == 2)
            setUpExpressionIntoVariable(res, integratingConstantExpr(VariablesDistributor::getVariablesDefinition(it)->getRange()
                                                                     ), it);*/
    return res;
}
abs_ex copy(const abs_ex & arg)
{
    return copy(arg.get());
    if (arg == nullptr)
        return nullptr;
    return makeAbstractExpression(arg->getId(), arg.get());
}
long long int gcd(long long int a, long long int b)
{

    if (a == std::numeric_limits<long long int>::min())
        return b % 2 == 0 ? 2 : 1;
    if (b == std::numeric_limits<long long int>::min())
        return a % 2 == 0 ? 2 : 1;
    a = abs(a);
    b = abs(b);
    while (a != 0 && b != 0)
    {
        if (a > b)
            a = a % b;
        else
            b = b % a;
    }
    return a + b;
}
std::unique_ptr<Polynomial> gcd(Polynomial * a_p, Polynomial * b_p)
{
   // qDebug() << a_p->makeStringOfExpression();
    std::unique_ptr<Polynomial> a ( new Polynomial(a_p));
    std::unique_ptr<Polynomial> b ( new Polynomial(b_p));
    assert(a->reduce().isOne());
    assert(b->reduce().isOne());
    //если нет переменных, относительно которых можно делить, но есть тригонометрические (или логарифмические) функции, заменяем общую функцию на переменную с id,
    //которого точно тут быть не может. Точно такой же кусок кода есть и в Polynomial::divide, но поскольку тут деление вызывается много раз, для производительности
    //лучше оставить это здесь тоже
   /* abs_ex changed_func = nullptr;
    abs_ex changing_var = abs_ex(new Variable(systemVar(0)));
    if (a->getSetOfPolyVariables().empty() || b->getSetOfPolyVariables().empty())
    {
        auto a_s = a->getSetOfFunctions();
        auto b_s = b->getSetOfFunctions();
        if (a_s.empty() || b_s.empty())
            return nullptr;
        QString common;
        for (auto &it : a_s)
            if (b_s.find(it) != b_s.end())
            {
                common = it;
                break;
            }
        if (common == "")
            return nullptr;
        changed_func = a->changeSomePartOn(common, changing_var);
        assert(changed_func != nullptr);
        b->changeSomePartOn(common, changing_var);

    }*/
    std::map<QString, int> replaced_functions;
    auto func_vec_a = replaceEveryFunctionOnSystemVariable(a.get(), replaced_functions);
    auto func_vec_b = replaceEveryFunctionOnSystemVariable(b.get(), replaced_functions);
    if (a->getSetOfPolyVariables().empty() || b->getSetOfPolyVariables().empty())
        return nullptr;
   // assert(!a->getSetOfPolyVariables().empty() && !b->getSetOfPolyVariables().empty());
    auto div_result = a->divide(b.get());
    bool has_a_bigger_degree = true;
    if (div_result.first == nullptr)
    {
        div_result = b->divide(a.get());
        has_a_bigger_degree = false;
    }

    if (div_result.first == nullptr)
        return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
    //qDebug() << "X: " << a_p->makeStringOfExpression();
    if (div_result.second->isZero())
        return std::make_unique<Polynomial>(has_a_bigger_degree ? b_p : a_p);
    std::unique_ptr<Polynomial> last_remainder = std::move(div_result.second);
    last_remainder->reduce();
    if (has_a_bigger_degree)
        div_result = b->divide(last_remainder.get());
    else
        div_result = a->divide(last_remainder.get());
    if (div_result.second == nullptr)
        return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
    int counter = 0;
    while (!div_result.second->isZero())
    {
        if (counter > 20)
            return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
        std::unique_ptr<Polynomial> current_remainder = std::move(div_result.second);
        current_remainder->reduce();
        div_result = last_remainder->divide(current_remainder.get());
        if (div_result.second == nullptr)
            return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
        last_remainder = std::move(current_remainder);
        ++counter;
    }
    if (last_remainder->downcast()->getId() == NUMBER)
        return std::unique_ptr<Polynomial>(new Polynomial(std::make_unique<Number>(1).get()));

    for (auto &it : func_vec_b)
        func_vec_a.insert(std::move(it));
    replaceSystemVariablesBackToFunctions(last_remainder.get(), func_vec_a);
   /* if (!func_vec_a.empty())
    {
        delete a;
        delete b;
    }*/
  //  qDebug() << last_remainder->makeStringOfExpression();
    return last_remainder;
}
long long int root(long long int a, long long int b, bool & succesfully)
{
    succesfully = true;
    if (a == 0 || a == 1)
        return a;
    bool is_a_negative = a < 0;
    if (b <= 0)
        throw Exception();
    if (b == 1)
        return a;
    if ((b / 2) * 2 == b && is_a_negative)
        throw Exception();
    if (is_a_negative)
        a *= -1;
    /*
    long double x = 1;
    int counter = 0;
    while ((long long int)pow((long long int)x, b) != a && (long long int)pow((long long int)x+1, b) != a)
    {
        x = double(double(1)/b)*((b-1)*(long long int)x + a / (long long int)pow((long long int)x, b-1));
        ++counter;
        if (counter >= 1000000)
            throw Exception();
    }*/
    /*
    if (power(x,b) == a)
        return x * (is_a_negative ? -1 : 1);
    else
        return (x + 1) * (is_a_negative ? -1 : 1); */
    //границы равны +-2^32, т. к. больше этого числа все равно корней быть не может
    long long int upper_bound = 4294967296;
    long long int lower_bound = (b % 2 == 0 ? 0 : -4294967296);
    while (upper_bound != lower_bound)
    {

        long long int middle = upper_bound / 2 + lower_bound / 2;
        if (upper_bound % 2 && lower_bound % 2)
            ++middle;
        long long int pow;
         bool s;
        pow = power(middle, b, s);
        if (!s)
        {
            if (middle > 0)
                upper_bound = middle;
            else
                lower_bound = middle;
            continue;
        }
        if (pow > a)
        {

            upper_bound = middle;
        }
        else if (pow < a)
        {
            if (upper_bound - lower_bound == 1)
            {

             if (power(upper_bound, b, succesfully) == a)
                  return upper_bound;
             succesfully = false;
             return 0;
            }
            lower_bound = middle;
        }
        else
            return middle;
    }
}
long long int power(long long int a, long long int n, bool & succesfully)
{
    //ye, I know there are exists fast algorithms but I need reliability not speed. I don't wanna lose 1 on double inaccuracy or lose moment of overlimiting
    succesfully = true;
    if (n == 0)
        return 1;
    if (n == 1)
        return a;
    if (a == 1 || a == 0)
        return a;
    long long int mult = a;
    for (long long int i = 1; i < n; ++i)
    {
        if (std::numeric_limits<long long int>::max() / abs(mult) < abs(a))
        {
            succesfully = false;
            return 0;
        }
        a = a * mult;
    }
    return a;
}
long long int amountOfCombination(long long int from_what, long long int set)
{
    assert(from_what >= 0 && set >= 0);

    if (from_what < set)
        return 0;
    if (from_what == set)
        return 1;
    if (from_what == 0 || set == 0)
        return 1;
    if (from_what > 100 || set > 100)
        throw Exception();
    unsigned long long int num = 1;
    unsigned long long int denum = 1;
    for (long long int i =  (set > from_what - set ? set + 1 : from_what); i <= from_what; ++i)
        num *= i;
    for (long long int i = 1; i <= (set > from_what - set ? from_what - set : set); ++i)
        denum *= i;
    return num / denum;

}
long long int factorial(long long int N)
{
    //т. к. если оно больше, то результат не поместится в int64
    assert(N <= 20 && N >= 0);
    long long int table[21] = {
        1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600, 6227020800, 87178291200, 1307674368000, 20922789888000, 355687428096000, 6402373705728000,
        121645100408832000, 2432902008176640000
    };
    return table[N];
}
std::unique_ptr<AbstractExpression> getDegrCoefficientPtr(long long int degree, const std::vector<int> & members_degrees)
{
    assert(degree <= 20 && degree >= 0);
    long long int coefficient = factorial(degree);
    for (auto &it : members_degrees)
        coefficient /= factorial(it);
    return std::unique_ptr<AbstractExpression>(new Number(coefficient));
}
void init(VariablesCombinations & combinations, int root)
{
    assert(combinations.size() >= 2 && root >= 2);
    combinations[0] = root - 1;
    combinations[1] = 1;
}
bool inc(VariablesCombinations & combinations)
{
    int back = combinations.back();
    combinations.back() = 0;
    int i = combinations.size() - 1;
    while (i >= 0 && combinations[i] == 0)
        --i;
    if (i < 0)
        return false;
    combinations[i] -= 1;
    combinations[i + 1] += 1 + back;
    return true;
}
long long int lcm(long long int a, long long int b)
{
    return a / gcd(a, b) * b;
}

bool isPiFractal(Fractal * frac)
{
    if (frac->isZero())
        return true;
    auto fr = frac->getFractal();
    if (fr.first->size() == 1 && **fr.first->begin() == *getPi() && fr.second->size() == 0)
        return true;
    return false;
}
//true if pi member is only member
std::pair<bool, std::unique_ptr<Fractal>> getPiMember(abs_ex & expr)
{

    if (expr->getId() == CONSTANT && *expr == *getPi())
        return {true, std::unique_ptr<Fractal>(new Fractal(expr.get()))};
    if (expr->getId() == POLYNOMIAL)
    {
        auto polynom = static_cast<Polynomial*>(expr.get())->getMonomialsPointers();
        for (auto &it : polynom)
            if (isPiFractal(it))
                return {false, std::unique_ptr<Fractal>(new Fractal(it))};
        return {false, nullptr};
    }
    if (expr->getId() == FRACTAL)
    {
        Fractal * frac = static_cast<Fractal*>(expr.get());
        if (isPiFractal(frac))
            return {true, std::unique_ptr<Fractal>(new Fractal(frac))};
        auto fr = frac->getFractal();
        if (fr.first->size() == 1 && fr.first->begin()->get()->getId() == POLYNOMIAL && fr.second->size() == 0)
        {
            auto polynom = static_cast<Polynomial*>(fr.first->begin()->get())->getMonomialsPointers();
            for (auto &it : polynom)
                if (isPiFractal(it))
                    return {false, std::unique_ptr<Fractal>(new Fractal(it, frac->getCoefficient()))};
        }
    }
    if (expr->getId() == NUMBER)
    {
        if (static_cast<Number*>(expr.get())->isZero())
            return {true, std::unique_ptr<Fractal>(new Fractal(Number(0)))};
    }
    return {false, nullptr};
}
bool isPiMemberInTable(Number coe)
{
    return (coe.getDenominator() <= 6) && coe.getDenominator() != 5;
}

std::pair<long long, std::pair<long long, long long> > xea(long long a, long long b)
{
    std::pair<long long int, long long int> a_pair = {a, b};
    std::pair<long long int, long long int> x_pair = {1, 0};
    std::pair<long long int, long long int> y_pair = {0, 1};
    while (a_pair.second != 0)
    {
        long long int q = a_pair.first / a_pair.second;
        a_pair = {a_pair.second, a_pair.first - a_pair.second * q};
        x_pair = {x_pair.second, x_pair.first - x_pair.second * q};
        y_pair = {y_pair.second, y_pair.first - y_pair.second * q};
    }
    return {a_pair.first, {x_pair.first, y_pair.first}};
}

std::multiset<int> findSubsetWithSum(const std::multiset<int> &set, int n)
{
    int size = 0;
    for (auto it = set.begin(); it != set.end(); ++it, ++size)
    {
        if (*it > n)
            break;
    }
    std::vector<std::vector<std::set<const int*>>> subsets_with_some_sum(size + 1, std::vector<std::set<const int*>>(n + 1));
    for (auto it = set.begin(); it != set.end() && *it <= n; ++it)
    {
        if (*it == n)
        {
            std::multiset<int> res;
            res.insert(*it);
            return res;
        }
        subsets_with_some_sum[1][*it].insert(&(*it));
    }
    for (int i = *set.begin() + 1; i <= n; ++i)
    {
        for (int j = 2; j <= size; ++j)
        {
            for (auto it = set.begin(); it != set.end() && *it  <= n; ++it)
            {
                if (i - *it <= 0)
                    continue;
                if (subsets_with_some_sum[j - 1][i - *it].empty())
                    continue;
                if (subsets_with_some_sum[j - 1][i - *it].find(&(*it)) != subsets_with_some_sum[j - 1][i - *it].end())
                    continue;
                subsets_with_some_sum[j][i] = subsets_with_some_sum[j - 1][i - *it];
                subsets_with_some_sum[j][i].insert(&(*it));
            }
        }
    }
    for (int i = 1; i <= size; ++i)
    {
        if (!subsets_with_some_sum[i][n].empty())
        {
            std::multiset<int> res;
            for (auto &it : subsets_with_some_sum[i][n])
                res.insert(*it);
            return res;
        }
    }
    return std::multiset<int>();
}

bool has(const std::set<int> &set, int el)
{
    return set.find(el) != set.end();
}

bool isPiMemberOnAxis(Number coe)
{
    return coe == 1 || coe == -1 || coe == Number(1, 2) || coe == Number(-1, 2) || coe == Number(1, 4) || coe == Number(-1, 4);
}
//используем примитивный алгоритм, т. к. работаем на маленьких размерах
bool hasIntersections(const std::set<int> &a, const std::set<int> &b)
{
    for (auto &it :a)
        if (has(b, it))
            return true;
    return false;
}

abs_ex toAbsEx(AbstractExpression *expr)
{
    return abs_ex(expr);
}
abs_ex copyWithLiftingIntegrationConstantsThatCanBeChanged(const AbstractExpression * expr)
{
    abs_ex res = copy(expr);
    auto set = res->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it) &&
                VariablesDistributor::amountOfVariable(it) == 2)
            setUpExpressionIntoVariable(res.get(), integratingConstantExpr(VariablesDistributor::getVariablesDefinition(it)->getRange()
                                                                     ), it);
    return res;
}
abs_ex copyWithLiftingIntegrationConstantsThatCanBeChanged(const abs_ex &expr)
{
    abs_ex res = copy(expr);
    auto set = res->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it) &&
                VariablesDistributor::amountOfVariable(it) == 2)
            setUpExpressionIntoVariable(res, integratingConstantExpr(VariablesDistributor::getVariablesDefinition(it)->getRange()
                                                                     ), it);
    return res;
}

void liftAllIntegratingConstants(abs_ex &expr)
{
    auto set = expr->getSetOfVariables();
    for (auto &it : set)
        if (isIntegratingConstant(it))
            setUpExpressionIntoVariable(expr, integratingConstantExpr(VariablesDistributor::getVariablesDefinition(it)->getRange()),
                                        it);
    expr->setSimplified(false);
    expr->simplify();
    expr = expr->downcast();
}

abs_ex licCopy(const AbstractExpression *expr)
{
    return copyWithLiftingIntegrationConstantsThatCanBeChanged(expr);
}

abs_ex licCopy(const abs_ex &expr)
{
    return copyWithLiftingIntegrationConstantsThatCanBeChanged(expr);
}

long long power(long long a, long long n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return a;
    if (n == 2)
        return a*a;
    if (n % 2 == 0)
    {
        int h = power(a, n/2);
        return h*h;
    }
    int h = power(a, n/2);
    return h*h*a;
}
