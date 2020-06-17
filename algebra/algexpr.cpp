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
#include "arctangent.h"
#include "arcsinus.h"
#include "solving_equations.h"
#include "solving_differential_equations.h"
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
    this->expression = abs_ex(expr.expression.release());
}
AlgExpr::AlgExpr(Number num)
{
    this->expression = abs_ex(new Number(num));
}
AlgExpr::AlgExpr(long long int num)
{
    this->expression = abs_ex(new Number(num));
}

AlgExpr::AlgExpr(const abs_ex &expr)
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
    this->expression = abs_ex(expr.expression.release());
    return *this;
}
AlgExpr& AlgExpr::operator=(Number num)
{
    this->expression = abs_ex(new Number(num));
    return *this;
}
AlgExpr& AlgExpr::operator=(long long int num)
{
    this->expression = abs_ex(new Number(num));
    return *this;
}

AlgExpr &AlgExpr::operator=(const abs_ex &expr)
{
    this->expression = copy(expr);
    return *this;
}

AlgExpr &AlgExpr::operator=(abs_ex &&expr)
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

abs_ex &AlgExpr::getExpr()
{
    return this->expression;
}

const abs_ex &AlgExpr::getExpr() const
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
/*bool AlgExpr::operator==(char letter) const
{
    return this->expression->getId() > 0 && static_cast<Variable*>(this->expression.get())->getName().size() == 1 &&
            static_cast<Variable*>(this->expression.get())->getName()[0] == letter;
}*/
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
/*bool AlgExpr::operator!=(char letter) const
{
    return !(*this == letter);
}*/
bool AlgExpr::operator!=(QString var_name) const
{
    return !(*this == var_name);
}
AlgExpr pow(const AlgExpr & arg, const AlgExpr & degree)
{
    AlgExpr result;
    result.expression = abs_ex(new Degree(arg.expression, degree.expression));
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
    result.expression = abs_ex(new Degree(arg.expression, std::make_unique<Number>(1, 2)));
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

QString AlgExpr::toWolframString()
{
    return this->expression->makeWolframString();
}
AlgExpr var()
{
    AlgExpr expr;
    expr.expression = abs_ex(new Variable(VariablesDistributor::createVariable(VariablesDefinition())));
    return expr;
}
AlgExpr positiveVar()
{
    AlgExpr expr;
    expr.expression = abs_ex(new Variable(VariablesDistributor::createVariable(getPositiveDefinition())));
    return expr;
}
AlgExpr var(VariablesDefinition def)
{
    AlgExpr expr;
    expr.expression = abs_ex(new Variable(VariablesDistributor::createVariable(def)));
    return expr;
}
AlgExpr abs(AlgExpr & arg)
{
    AlgExpr expr;
    expr.expression = abs(arg.expression);
    return expr;
}
AlgExpr abs(AlgExpr && arg)
{
    AlgExpr expr;
    expr.expression = abs(std::move(arg.expression));
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
    expr.expression= sin(arg.expression);
    return expr;
}
AlgExpr sin(AlgExpr && arg)
{
    AlgExpr expr;
    expr.expression = sin(std::move(arg.expression));
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
    expr.expression = cos(arg.expression);
    return expr;
}
AlgExpr cos(AlgExpr && arg)
{
    AlgExpr expr;
    expr.expression = cos(std::move(arg.expression));
    return expr;
}
AlgExpr cos(int arg)
{
    return cos(AlgExpr(arg));
}

AlgExpr tan(AlgExpr &arg)
{
    AlgExpr expr;
    expr.expression = tan(arg.expression);
    return expr;
}

AlgExpr tan(AlgExpr &&arg)
{
    AlgExpr expr;
    expr.expression = tan(std::move(arg.expression));
    return expr;
}

AlgExpr tan(int arg)
{
    return tan(AlgExpr(arg));
}
AlgExpr cot(AlgExpr &arg)
{
    AlgExpr expr;
    expr.expression = cot(arg.expression);
    return expr;
}

AlgExpr cot(AlgExpr &&arg)
{
    AlgExpr expr;
    expr.expression = cot(std::move(arg.expression));
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
    return AlgExpr(D(arg.expression));
}

AlgExpr D(AlgExpr &&arg)
{
    return AlgExpr(D(std::move(arg.expression)));
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

AlgExpr integral(const AlgExpr &arg)
{
    return AlgExpr(integrate(arg.getExpr()));
}

AlgExpr integral(const AlgExpr &arg, AlgExpr var)
{
    assert(var.getExpr()->getId() > 0);
    return integral(arg * D(var.getExpr()));
}

AlgExpr atan(const AlgExpr &arg)
{
    return AlgExpr(atan(arg.expression));
}
AlgExpr atan(AlgExpr &&arg)
{
    return AlgExpr(atan(std::move(arg.expression)));
}
AlgExpr asin(const AlgExpr &arg)
{
    return AlgExpr(asin(arg.expression));
}
AlgExpr asin(AlgExpr && arg)
{
    return AlgExpr(asin(std::move(arg.expression)));
}

AlgExpr definiteIntegral(const AlgExpr &arg, const AlgExpr &from, const AlgExpr &to)
{
    return definiteIntegral(arg.getExpr(), from.getExpr(), to.getExpr());
}

std::list<AlgExpr> solveEquation(const AlgExpr &equation, const AlgExpr &var)
{
    assert(var.getExpr()->getId() > 0);
    std::list<abs_ex> eq_res = solveEquation(equation.getExpr(), var.getExpr()->getId());
    std::list<AlgExpr> res;
    for (auto &it : eq_res)
        res.push_back(std::move(it));
    return res;
}

std::list<AlgExpr> solveDifur(const AlgExpr &difur, const AlgExpr &x, const AlgExpr &y)
{
    std::list<abs_ex> eq_res = solveDifur(difur.getExpr(), x.getExpr()->getId(), y.getExpr()->getId());
    std::list<AlgExpr> res;
    for (auto &it : eq_res)
        res.push_back(std::move(it));
    return res;
}
