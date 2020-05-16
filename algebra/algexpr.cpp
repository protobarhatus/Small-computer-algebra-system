#include "algexpr.h"
#include "some_algebra_expression_conversions.h"
#include "variablesdistributor.h"
#include "constant.h"
#include "sinus.h"
#include "cosinus.h"
#include "tangent.h"
#include "cotangent.h"
#include "logarithm.h"
#include "differential.h"
AlgExpr::AlgExpr()
{

}
AlgExpr::AlgExpr(const AlgExpr & expr)
{
    this->expression = makeAbstractExpression(expr.expression->getId(), expr.expression.get());
}
AlgExpr::AlgExpr(AlgExpr && expr)
{
    //this->expression = std::move(expr.expression);
    this->expression = std::unique_ptr<AbstractExpression>(expr.expression.release());
}
AlgExpr::AlgExpr(Number num)
{
    this->expression = std::unique_ptr<AbstractExpression>(new Number(num));
}
AlgExpr::AlgExpr(long long int num)
{
    this->expression = std::unique_ptr<AbstractExpression>(new Number(num));
}

AlgExpr::AlgExpr(const std::unique_ptr<AbstractExpression> &expr)
{
    this->expression = copy(expr);
}

AlgExpr::AlgExpr(abs_ex && expr)
{
    this->expression = std::move(expr);
}


AlgExpr& AlgExpr::operator=(const AlgExpr &expr)
{
    this->expression = makeAbstractExpression(expr.expression->getId(), expr.expression.get());
    return *this;
}
AlgExpr& AlgExpr::operator=(AlgExpr && expr)
{
    //this->expression = std::move(expr.expression);
    this->expression = std::unique_ptr<AbstractExpression>(expr.expression.release());
    return *this;
}
AlgExpr& AlgExpr::operator=(Number num)
{
    this->expression = std::unique_ptr<AbstractExpression>(new Number(num));
    return *this;
}
AlgExpr& AlgExpr::operator=(long long int num)
{
    this->expression = std::unique_ptr<AbstractExpression>(new Number(num));
    return *this;
}

AlgExpr &AlgExpr::operator=(const std::unique_ptr<AbstractExpression> &expr)
{
    this->expression = copy(expr);
    return *this;
}

AlgExpr &AlgExpr::operator=(std::unique_ptr<AbstractExpression> &&expr)
{
    this->expression = std::move(expr);
    return *this;
}



AlgExpr& AlgExpr::operator+=(const AlgExpr & expr)
{
    this->expression = this->expression + expr.expression;
    return *this;
}
AlgExpr& AlgExpr::operator-=(const AlgExpr & expr)
{
    this->expression = this->expression - expr.expression;
    return *this;
}
AlgExpr& AlgExpr::operator*=(const AlgExpr & expr)
{
    this->expression = this->expression * expr.expression;
    return *this;
}
AlgExpr& AlgExpr::operator/=(const AlgExpr & expr)
{
    this->expression = this->expression / expr.expression;
    return *this;
}

AlgExpr operator+(const AlgExpr &left, const AlgExpr &right)
{
    return left.expression + right.expression;
}

AlgExpr operator+(const AlgExpr &left, AlgExpr &&right)
{
    return left.expression + std::move(right.expression);
}

AlgExpr operator+(AlgExpr &&left, const AlgExpr &right)
{
    return std::move(left.expression) + right.expression;
}

AlgExpr operator+(AlgExpr &&left, AlgExpr &&right)
{
    return std::move(left.expression) + std::move(right.expression);
}



AlgExpr operator-(const AlgExpr &left, const AlgExpr &right)
{
    return left.expression - right.expression;
}

AlgExpr operator-(const AlgExpr &left, AlgExpr &&right)
{
    return left.expression - std::move(right.expression);
}

AlgExpr operator-(AlgExpr &&left, const AlgExpr &right)
{
    return std::move(left.expression) - right.expression;
}

AlgExpr operator-(AlgExpr &&left, AlgExpr &&right)
{
    return std::move(left.expression) - std::move(right.expression);
}



AlgExpr operator*(const AlgExpr &left, const AlgExpr &right)
{
    return left.expression * right.expression;
}

AlgExpr operator*(const AlgExpr &left, AlgExpr &&right)
{
    return left.expression * std::move(right.expression);
}

AlgExpr operator*(AlgExpr &&left, const AlgExpr &right)
{
    return std::move(left.expression) * right.expression;
}

AlgExpr operator*(AlgExpr &&left, AlgExpr &&right)
{
    return std::move(left.expression) * std::move(right.expression);
}



AlgExpr operator/(const AlgExpr &left, const AlgExpr &right)
{
    return left.expression / right.expression;
}

AlgExpr operator/(const AlgExpr &left, AlgExpr &&right)
{
    return left.expression / std::move(right.expression);
}

AlgExpr operator/(AlgExpr &&left, const AlgExpr &right)
{
    return std::move(left.expression) / right.expression;
}

AlgExpr operator/(AlgExpr &&left, AlgExpr &&right)
{
    return std::move(left.expression) / std::move(right.expression);
}



AlgExpr AlgExpr::operator-() const
{
    return -1 * *this;
}

std::unique_ptr<AbstractExpression> &AlgExpr::getExpr()
{
    return this->expression;
}

const std::unique_ptr<AbstractExpression> &AlgExpr::getExpr() const
{
    return this->expression;
}
bool AlgExpr::operator==(const AlgExpr &expr) const
{
        //if ((this->expression->getId() == FRACTAL || expr.expression->getId() == FRACTAL) && !expr.isEqualTo(0))
        //    return *this - expr == AlgExpr(0);
       // return (*this + 0).isEqualTo(expr + 0);
    return *this - expr == 0;
}
bool AlgExpr::isEqualTo(const AlgExpr &expr) const
{
    return *this->expression.get() == *expr.expression.get();
}
bool AlgExpr::operator==(Number num) const
{
    return this->expression->getId() == NUMBER && (static_cast<Number*>(this->expression.get())->compareWith(num) == 0);
}
bool AlgExpr::operator==(long long int num) const
{
    return this->expression->getId() == NUMBER && (static_cast<Number*>(this->expression.get())->compareWith(num) == 0);
}
bool AlgExpr::operator==(char letter) const
{
    return this->expression->getId() > 0 && static_cast<Variable*>(this->expression.get())->getName().size() == 1 &&
            static_cast<Variable*>(this->expression.get())->getName()[0] == letter;
}
bool AlgExpr::operator==(QString var_name) const
{
    return this->expression->getId() > 0 && static_cast<Variable*>(this->expression.get())->getName() == var_name;
}

bool AlgExpr::operator==(int num) const
{
    return *this == (long long int)num;
}
bool AlgExpr::operator!=(const AlgExpr &expr) const
{
    return !(*this == expr);
}
bool AlgExpr::operator!=(Number num) const
{
    return !(*this == num);
}
bool AlgExpr::operator!=(long long int num) const
{
    return !(*this == num);
}
bool AlgExpr::operator!=(char letter) const
{
    return !(*this == letter);
}
bool AlgExpr::operator!=(QString var_name) const
{
    return !(*this == var_name);
}
AlgExpr pow(const AlgExpr & arg, const AlgExpr & degree)
{
    AlgExpr result;
    result.expression = std::unique_ptr<AbstractExpression>(new Degree(arg.expression, degree.expression));
    result.expression = result.expression->downcast();
    return result;
}
AlgExpr root(const AlgExpr & arg, const AlgExpr & degree)
{
    return pow(arg, 1/degree);
}
AlgExpr sqrt(const AlgExpr & arg)
{
    AlgExpr result;
    result.expression = std::unique_ptr<AbstractExpression>(new Degree(arg.expression, std::make_unique<Number>(1, 2)));
    result.expression = result.expression->downcast();

    return result;
}
void AlgExpr::out()
{
    this->expression->_qDebugOut();
}

AlgExpr operator+(long long int left, const AlgExpr & expr)
{
    return std::move(expr + left);
}
AlgExpr operator-(long long int left, const AlgExpr & right)
{
    return std::move(AlgExpr(left) - right);
}
AlgExpr operator*(long long int left, const AlgExpr & right)
{
    return std::move(right * left);
}
AlgExpr operator/(long long int left, const AlgExpr & right)
{
    return std::move(AlgExpr(left) / right);
}
bool operator==(long long int left, const AlgExpr & right)
{
    return right == left;
}
bool operator!=(long long int left, const AlgExpr & right)
{
    return !(right == left);
}
QString AlgExpr::toString()
{
    return this->expression->makeStringOfExpression();
}
AlgExpr var()
{
    AlgExpr expr;
    expr.expression = std::unique_ptr<AbstractExpression>(new Variable(VariablesDistributor::createVariable(VariablesDefinition())));
    return expr;
}
AlgExpr positiveVar()
{
    AlgExpr expr;
    expr.expression = std::unique_ptr<AbstractExpression>(new Variable(VariablesDistributor::createVariable(getPositiveDefinition())));
    return expr;
}
AlgExpr var(VariablesDefinition def)
{
    AlgExpr expr;
    expr.expression = std::unique_ptr<AbstractExpression>(new Variable(VariablesDistributor::createVariable(def)));
    return expr;
}
AlgExpr abs(AlgExpr & arg)
{
    AlgExpr expr;
    expr.expression = std::unique_ptr<AbstractExpression>(new AbsoluteValue(arg.expression))->downcast();
    return expr;
}
AlgExpr abs(AlgExpr && arg)
{
    AlgExpr expr;
    expr.expression = std::unique_ptr<AbstractExpression>(new AbsoluteValue(std::move(arg.expression)))->downcast();
    return expr;
}
AlgExpr var(double min, double max)
{
    VariablesDefinition definition;
    definition.setMinValue(min);
    definition.setMaxValue(max);
    return var(definition);
}
AlgExpr sqrt(int arg)
{
    return sqrt(AlgExpr(arg));
}
AlgExpr sin(AlgExpr & arg)
{
    AlgExpr expr;
    expr.expression= abs_ex(new Sinus(arg.expression))->downcast();
    return expr;
}
AlgExpr sin(AlgExpr && arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Sinus(std::move(arg.expression)))->downcast();
    return expr;
}
AlgExpr pi()
{
    return AlgExpr(getPi());
}
AlgExpr euler()
{
    return AlgExpr(getEuler());
}
AlgExpr sin(int arg)
{
    return sin(AlgExpr(arg));
}
AlgExpr cos(AlgExpr & arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Cosinus(arg.expression))->downcast();
    return expr;
}
AlgExpr cos(AlgExpr && arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Cosinus(std::move(arg.expression)))->downcast();
    return expr;
}
AlgExpr cos(int arg)
{
    return cos(AlgExpr(arg));
}

AlgExpr tan(AlgExpr &arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Tangent(arg.expression))->downcast();
    return expr;
}

AlgExpr tan(AlgExpr &&arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Tangent(std::move(arg.expression)))->downcast();
    return expr;
}

AlgExpr tan(int arg)
{
    return tan(AlgExpr(arg));
}
AlgExpr cot(AlgExpr &arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Cotangent(arg.expression))->downcast();
    return expr;
}

AlgExpr cot(AlgExpr &&arg)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Cotangent(std::move(arg.expression)))->downcast();
    return expr;
}

AlgExpr cot(int arg)
{
    return cot(AlgExpr(arg));
}

AlgExpr ln(AlgExpr &arg)
{
    AlgExpr expr;
    expr.expression = ln(arg.expression)->downcast();
    return expr;
}

AlgExpr ln(AlgExpr &&arg)
{
    AlgExpr expr;
    expr.expression = ln(std::move(arg.expression))->downcast();
    return expr;
}

AlgExpr ln(int arg)
{
    return ln(AlgExpr(arg));
}

AlgExpr log(AlgExpr &arg, AlgExpr &base)
{
    return ln(arg)/ln(base);
}

AlgExpr log(AlgExpr &&arg, AlgExpr &base)
{
    return ln(std::move(arg))/ln(base);
}

AlgExpr log(AlgExpr &arg, AlgExpr &&base)
{
    return ln(arg)/ln(std::move(base));
}

AlgExpr log(AlgExpr &&arg, AlgExpr &&base)
{
    return ln(std::move(arg))/ln(std::move(base));
}

AlgExpr e()
{
    return euler();
}

AlgExpr derivative(const AlgExpr &arg, AlgExpr arg_variable)
{
    if (arg_variable.expression->getId() <= 0)
        throw (QString)"Arg_variable required to be a variable";
    return AlgExpr(arg.expression->derivative(arg_variable.expression->getId()));
}

AlgExpr D(const AlgExpr &arg)
{
    return AlgExpr(abs_ex(new Differential(arg.expression)));
}

AlgExpr D(AlgExpr &&arg)
{
    return AlgExpr(abs_ex(new Differential(std::move(arg.expression))));
}

AlgExpr operator+(long long left, AlgExpr &&expr)
{
    return AlgExpr(left) + std::move(expr);
}

AlgExpr operator-(long long left, AlgExpr &&right)
{
    return AlgExpr(left) - std::move(right);
}

AlgExpr operator*(long long left, AlgExpr &&right)
{
    return AlgExpr(left) * std::move(right);
}

AlgExpr operator/(long long left, AlgExpr &&right)
{
    return AlgExpr(left)/std::move(right);
}
