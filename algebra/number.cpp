#include "number.h"
#include "assert.h"
#include "some_algebra_expression_conversions.h"
#include "exception.h"
#include "algorithm"
#include "variablesdistributor.h"
abs_ex one = abs_ex(new Number(1));
abs_ex two = abs_ex(new Number(2));
abs_ex half = abs_ex(new Number(1, 2));
abs_ex zero = abs_ex(new Number(0));
abs_ex minus_one = abs_ex(new Number(-1));
abs_ex three = abs_ex(new Number(3));
abs_ex four = abs_ex(new Number(4));
Number::Number(long long int ivalue)
{
    this->numerator = ivalue;
    this->denominator = 1;
}
AlgebraExpression Number::getId() const
{
    return NUMBER;
}
bool Number::operator<(const AbstractExpression &right) const
{
    if (!this->isCorrect())
        throw Exception();
    assert(right.getId() == NUMBER);
    Number * num = static_cast<Number*>(const_cast<AbstractExpression*>(&right));
    //as it's system function, i can do it unconnected with mathematic rules of comparing fractal
    if (this->numerator < num->numerator)
        return true;
    if (this->numerator > num->numerator)
        return false;
    return this->denominator < num->denominator;
}
bool Number::operator==(AbstractExpression & right)
{
    if (!this->isCorrect())
        throw Exception();
    if (right.getId() != NUMBER)
        return false;
    Number * num = static_cast<Number*>(&right);
    return this->numerator == num->numerator && this->denominator == num->denominator;
}
std::unique_ptr<AbstractExpression> Number::downcastTo(AlgebraExpression expr)
{
    assert(false);
    return std::unique_ptr<AbstractExpression>(nullptr);
}
long long int Number::getNumerator() const
{
    return this->numerator;
}
long long int Number::getDenominator() const
{
    return this->denominator;
}
Number::Number(const Number & num)
{
    this->numerator = num.numerator;
    this->denominator = num.denominator;
    this->simplified = num.simplified;
   // if (!this->isCorrect())
    //    throw Exception();
}
Number::Number(long long int num, long long int denum)
{
    if (denum == 0)
        throw Exception();
    this->numerator = num;
    this->denominator = denum;
}
bool Number::canDowncastTo(AlgebraExpression expr)
{
    return false;
}
std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr)
{
    assert(false);
    return std::unique_ptr<AbstractExpression>(nullptr);
}
Number & Number::operator=(long long int value)
{
    this->numerator = value;
    this->denominator = 1;
    return *this;
}
bool Number::isOne() const
{
    return this->numerator == 1 && this->denominator == 1;
}
void Number::simplify()
{
    SIM_IF_NEED
    long long int gcf = gcd(this->numerator, this->denominator);
    this->numerator /= gcf;
    this->denominator /= gcf;
    if (this->denominator < 0)
    {
        this->numerator *= -1;
        this->denominator *= -1;
    }
}
Number Number::operator+(const Number & num) const
{
    if (!this->isCorrect())
        throw Exception();
    if (!num.isCorrect())
        throw Exception();
    long long int gcf = gcd(this->denominator, num.denominator);
    if (abs(this->numerator) > std::numeric_limits<long long int>::max() / abs(num.denominator / gcf))
        throw Exception();
    if (abs(num.denominator) > std::numeric_limits<long long int>::max() / abs(this->denominator / gcf))
        throw Exception();
    long long int nnum1 = this->numerator * (num.denominator / gcf);
    long long int nnum2 = num.numerator * (this->denominator / gcf);
    if (abs(nnum1) > std::numeric_limits<long long int>::max() - abs(nnum2))
        throw Exception();
    long long int newnum = nnum1 + nnum2;
    if (abs(this->denominator / gcf) > std::numeric_limits<long long int>::max() / abs(num.denominator))
        throw Exception();
    long long int denum = (this->denominator / gcf) * num.denominator;
    Number result(newnum, denum);
    result.simplify();
    return result;
}
Number Number::operator-(const Number &number) const
{
    Number sum(number.numerator * -1, number.denominator);
    return *this + sum;
}
Number Number::operator*(const Number &number) const
{
    if (this->numerator == 0 || number.numerator == 0)
        return 0;
    if (!this->isCorrect())
        throw Exception();
    if (!number.isCorrect())
        throw Exception();
    //number should be simplified
    //this->simplify();
    //this is for bigger number limit
    Number num1(this->numerator, number.denominator);
    num1.simplify();
    Number num2(number.numerator, this->denominator);
    num2.simplify();
    if (abs(num1.numerator) > std::numeric_limits<long long int>::max() / abs(num2.numerator))
        throw Exception();
    if (abs(num1.denominator) > std::numeric_limits<long long int>::max() / abs(num2.denominator))
        throw Exception();
    Number result(num1.numerator * num2.numerator, num1.denominator * num2.denominator);
    //result is already simplified
    return result;
}
Number Number::operator/(const Number &number) const
{

    Number num = number;
    std::swap(num.numerator, num.denominator);
    return *this * num;
}
Number& Number::operator=(const Number & number)
{
    this->numerator = number.numerator;
    this->denominator = number.denominator;
    this->simplified = number.simplified;
    return *this;
}
short int Number::compareWith(long long int value) const
{
    if (!this->isCorrect())
        throw Exception();

    if (value > std::numeric_limits<long long int>::max() / this->denominator && value >= 0)
        return 1;
    if (value < 0 && value < std::numeric_limits<long long int>::min() / this->denominator)
        return -1;
    value *= this->denominator;
    if (this->numerator < value)
        return -1;
    if (value == this->numerator)
        return 0;
    if (this->numerator > value)
        return 1;
}
bool Number::isZero() const
{
    return this->numerator == 0;
}
bool Number::isInteger()
{
    return this->denominator == 1;
}
std::set<int> Number::getSetOfPolyVariables() const
{
    return std::set<int>();
}
std::set<int> Number::getSetOfVariables() const
{
    return std::set<int>();
}

std::set<QString> Number::getSetOfFunctions() const
{
    return std::set<QString>();
}
Number Number::makeErrorNumber()
{
    Number num(1);
    num.denominator = 0;
    return num;
}
bool Number::isCorrect() const
{
    return this->denominator != 0;
}
Number Number::getMaxDegreeOfVariable(int id)
{
    return 0;
}
short int Number::compareWith(Number num) const
{
    if (!this->isCorrect())
        throw Exception();
    if (!num.isCorrect())
        throw Exception();

    num.simplify();
    long long int gcf = gcd(this->denominator, num.denominator);
    if (abs(this->numerator) > std::numeric_limits<long long int>::max() / abs(num.denominator / gcf) ||
            abs(num.numerator) > std::numeric_limits<long long int>::max() / abs(this->denominator / gcf))
    {
        if (this->numerator == num.numerator)
            return (this->denominator < num.numerator ? 1 : (this->denominator == num.denominator ? 0 : -1));
        if (this->numerator < num.numerator && this->denominator > num.denominator)
            return -1;
        if (this->numerator > num.numerator && this->denominator < num.denominator)
            return 1;
        //there might be errors, so it's better not to use algebra modul with big values
        if (double(this->numerator) / double(this->denominator) != double(num.numerator) / double(num.denominator))
            return (double(this->numerator) / double(this->denominator) > double(num.numerator) / double(num.denominator) ? 1 : -1);
        if (double(num.denominator) / double(this->denominator) != double(num.numerator) / double(this->numerator))
            return (double(num.numerator) / double(this->numerator) > double(num.denominator) / double(this->denominator) ? -1 : 1);
        double this_num = this->numerator / 1000000000000.0;
        double this_denom = this->denominator / 1000000000000.0;
        double n_numn = num.numerator / 1000000000000.0;
        double n_denom = num.denominator / 1000000000000.0;
        return (this_num * n_denom > n_numn * this_denom ? 1 : 0);
    }
    return (this->numerator * (num.denominator / gcf) > num.numerator * (this->denominator / gcf) ? 1 :
            (this->numerator * (num.denominator / gcf) == num.numerator * (this->denominator / gcf) ? 0 : -1));
}

void Number::_qDebugOut()
{
    if (this->isInteger())
        qDebug() << "NUMBER AS INTEGER VALUE: " << this->numerator;
    else
    {
        qDebug() << "NUMBER:";
        qDebug() << "numerator:";
        qDebug() << this->numerator;
        qDebug() << "denominator:";
        qDebug() << this->denominator;
        qDebug() << "END OF NUMBER;";
    }
}
QString Number::makeStringOfExpression() const
{
    QString result, num, denum;
    num.setNum(this->numerator);
    denum.setNum(this->denominator);
    result += num;
    if (this->denominator != 1)
        result += "/" + denum;
    return result;
}
Number Number::operator+=(const Number &number)
{
    *this = *this + number;
    return *this;
}
Number Number::operator-=(const Number &number)
{
    return *this = *this - number;
}
Number Number::operator*=(const Number &number)
{
    return *this = *this * number;
}
Number Number::operator/=(const Number &number)
{
    return *this = *this / number;
}
bool Number::operator!=(const Number & num) const
{
    return !(*const_cast<Number*>(this) == *const_cast<Number*>(&num));
}
double Number::getApproximateValue()
{
    return double(this->numerator) / double(this->denominator);
}
int Number::getPositionRelativelyZeroIfHasVariables()
{
    assert(false);
    return 0;
}
double Number::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    return this->getApproximateValue();
}

bool Number::operator==(long long num)
{
    return this->numerator == num && this->denominator == 1;
}
bool Number::operator==(const Number & num)
{
    return this->numerator == num.numerator && this->denominator == num.denominator;
}

std::unique_ptr<AbstractExpression> Number::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    return nullptr;
}

bool Number::less(const Number &b) const
{
    return (*this - b).compareWith(0) < 0 && *this != b;
}

std::unique_ptr<AbstractExpression> Number::derivative(int var) const
{
    return copy(zero);
}

std::unique_ptr<AbstractExpression> Number::antiderivative(int var) const
{
    return abs_ex(new Number(*this)) * abs_ex(new Variable(getVariable(var)));
}

Number abs(Number num)
{
    if (num.compareWith(0) < 0)
        return num * -1;
    return num;
}

Number max(Number a, Number b)
{
    return (a - b).compareWith(0) > 0 ? a : b;
}

Number min(Number a, Number b)
{
    return (a-b).compareWith(0) < 0 ? a : b;
}



std::unique_ptr<AbstractExpression> toAbsEx(Number n)
{
    return abs_ex(new Number(n));
}
