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
std::unique_ptr<AbstractExpression> makeAbstractExpression(AlgebraExpression type, AbstractExpression * argument)
{
    assert(type == argument->getId() || (type > 0 && argument->getId() > 0));
    if (type > 0)
        return std::unique_ptr<AbstractExpression>(new Variable(*(static_cast<Variable*>(argument))));
    switch(type)
    {
    case NUMBER:
        return std::unique_ptr<AbstractExpression>(new Number(*static_cast<Number*>(argument)));
    case POLYNOMIAL:
        return std::unique_ptr<AbstractExpression>(new Polynomial(*static_cast<Polynomial*>(argument)));
    case FRACTAL:
        return std::unique_ptr<AbstractExpression>(new Fractal(*static_cast<Fractal*>(argument)));
    case DEGREE:
        return std::unique_ptr<AbstractExpression>(new Degree(*static_cast<Degree*>(argument)));
    case ABSOLUTE_VALUE:
        return std::unique_ptr<AbstractExpression>(new AbsoluteValue(*static_cast<AbsoluteValue*>(argument)));
    case CONSTANT:
        return std::unique_ptr<AbstractExpression>(new Constant(*static_cast<Constant*>(argument)));
    case SINUS:
        return std::unique_ptr<AbstractExpression>(new Sinus(*static_cast<Sinus*>(argument)));
    case COSINUS:
        return std::unique_ptr<AbstractExpression>(new Cosinus(*static_cast<Cosinus*>(argument)));
    case TANGENT:
        return std::unique_ptr<AbstractExpression>(new Tangent(*static_cast<Tangent*>(argument)));
    case COTANGENT:
        return std::unique_ptr<AbstractExpression>(new Cotangent(*static_cast<Cotangent*>(argument)));
    default:
        assert(false);

    }
}
abs_ex copy(AbstractExpression * arg)
{
    return makeAbstractExpression(arg->getId(), arg);
}
abs_ex copy(abs_ex & arg)
{
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
std::unique_ptr<Polynomial> gcd(Polynomial * a, Polynomial * b)
{
    assert(a->reduce().isOne());
    assert(b->reduce().isOne());
    assert(!a->getSetOfPolyVariables().empty() && !b->getSetOfPolyVariables().empty());
    auto div_result = a->divide(b);
    bool has_a_bigger_degree = true;
    if (div_result.first == nullptr)
    {
        div_result = b->divide(a);
        has_a_bigger_degree = false;
    }

    if (div_result.first == nullptr)
        return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
    if (div_result.second->isZero())
        return std::make_unique<Polynomial>(has_a_bigger_degree ? b : a);
    std::unique_ptr<Polynomial> last_remainder = std::move(div_result.second);
    last_remainder->reduce();
    if (has_a_bigger_degree)
        div_result = b->divide(last_remainder.get());
    else
        div_result = a->divide(last_remainder.get());
    if (div_result.second == nullptr)
        return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
    while (!div_result.second->isZero())
    {
        std::unique_ptr<Polynomial> current_remainder = std::move(div_result.second);
        current_remainder->reduce();
        div_result = last_remainder->divide(current_remainder.get());
        if (div_result.second == nullptr)
            return std::make_unique<Polynomial>(std::make_unique<Number>(1).get());
        last_remainder = std::move(current_remainder);
    }
    if (last_remainder->downcast()->getId() == NUMBER)
        return std::unique_ptr<Polynomial>(new Polynomial(std::make_unique<Number>(1).get()));
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
