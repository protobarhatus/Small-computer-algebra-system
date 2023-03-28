#include "differential.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
#include <cmath>
#include "variablesdistributor.h"
#include "fractal.h"
#include "number.h"
Differential::Differential(const abs_ex &arg)
{
    this->argument = copy(arg);
    this->simplify();
}
Differential::Differential(const abs_ex & arg, int order)
{
    this->argument = copy(arg);
    this->order = order;
    this->simplify();
}
Differential::Differential(abs_ex &arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

Differential::Differential(const Differential &cop)
{
    this->argument = copy(cop.argument);
    this->order = cop.order;
}

Differential::Differential(Differential &&mov)
{
    this->argument = std::move(mov.argument);
    this->order = mov.order;
}

Differential::~Differential()
{

}

AlgebraExpression Differential::getId() const
{
    return DIFFERENTIAL;
}

void Differential::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    if (this->argument->getId() == DIFFERENTIAL)
    {
        this->order++;
        abs_ex arg = std::move(static_cast<Differential*>(argument.get())->argument);
        this->argument = std::move(arg);
    }
    this->simplified = true;
}

bool Differential::operator==(AbstractExpression &right)
{
    if (right.getId() != DIFFERENTIAL)
        return false;
    return *this->argument == *static_cast<Differential*>(&right)->argument;
}

bool Differential::canDowncastTo()
{
    if (this->argument->getId() == NUMBER)
        return true;
    if (this->argument->getId() <= 0)
        return true;

    return false;
}

abs_ex Differential::downcastTo()
{
    if (this->argument->getId() == NUMBER)
        return copy(zero);
    if (this->argument->getId() <= 0)
        return fullDifferential(this->argument);
   // assert(false);
    return nullptr;
}

std::set<int> Differential::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> Differential::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Differential::getSetOfFunctions() const
{
    //считать ли его функцией? Вряд ли это что-то сломает, но если не считать, то может и что-то полететь
    std::set<QString> s;
    //s.insert(this->makeStringOfExpression());
    return s;
}

Number Differential::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void Differential::_qDebugOut()
{
    //да мне уже и нахрен не нужна эта функция, зачем я ее заполняю?
}

QString Differential::makeStringOfExpression() const
{
    QString sym = "d";
    if (order > 1)
        sym += "^" + QString::number(order);
    if (this->argument->getId() > 0 && order == 1)
        return sym + this->argument->makeStringOfExpression();
    return sym + "(" + this->argument->makeStringOfExpression() + ")";
}

QString Differential::makeWolframString() const
{
    QString sym = "D";
    if (order > 1)
        sym += "^" + QString::number(order);
    return sym + "[" + argument->makeWolframString() + "]";
}

QString Differential::toString() const
{
    QString sym = "d";
    if (order > 1)
        sym += "^" + QString::number(order);
    if (this->argument->getId() > 0 && order == 1)
        return sym + this->argument->makeStringOfExpression();
    return sym + "(" + this->argument->makeStringOfExpression() + ")";
}

QString Differential::makeRenderString() const
{
    return "PDiffN(" + this->argument->makeRenderString() + "," + QString::number(order) + ")";
}

double Differential::getApproximateValue()
{
    assert(false);
    return std::nan("0");
}

double Differential::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    //нельзя брать значение у бесконечно маленькой величины
    assert(false);
    return 0;
}

int Differential::getPositionRelativelyZeroIfHasVariables()
{
    return 0;
}

QString Differential::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

abs_ex Differential::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex Differential::changeSomePartOn(QString part, const abs_ex &on_what)
{
   // NONCONST
    if (this->argument->makeStringOfExpression() == part)
        this->argument = copy(on_what);
    return this->argument->changeSomePartOn(part, on_what);
}

abs_ex Differential::changeSomePartOnExpression(QString part, const abs_ex &on_what)
{
    NONCONST
            if (this->argument->makeStringOfExpression() == part)
    {
        this->argument = copy(on_what);
        return copy(on_what);
    }
        return this->argument->changeSomePartOn(part, on_what);
}

abs_ex Differential::getArgumentsCopy() const
{
    return copy(this->argument);
}

abs_ex Differential::derivative(int var) const
{
    //производная от дифференциала? может быть дифференциал более высокого порядка? пока хз
    assert(false);
    return nullptr;
}

AbstractExpression *Differential::getArgument()
{
    return this->argument.get();
}

abs_ex Differential::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    if (this->argument->getId() == var)
        return copy(this->argument);
    assert(false);
    return nullptr;
}

void Differential::setSimplified(bool simpl)
{
    this->simplified = simpl;
}

std::set<abs_ex > Differential::getTrigonometricalFunctions() const
{
    return this->argument->getTrigonometricalFunctions();
}

long long Differential::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long Differential::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange Differential::getRange() const
{
    return FunctionRange::getErrorRange();
}

bool Differential::hasDifferential() const
{
    return true;
}

bool Differential::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{

    return false;
}

abs_ex Differential::tryToFindExponentialFunction(int var) const
{
    return nullptr;
}

void Differential::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

void Differential::doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const
{
    this->argument->doSomethingInDerivativeObject(func);
}

bool Differential::canBeZero() const
{
    return true;
}

bool Differential::hasUndefinedVariable() const
{
    return this->argument->hasUndefinedVariable();
}

bool Differential::operator<(const AbstractExpression &expr) const
{
    assert(expr.getId() == DIFFERENTIAL);
    return AbstractExpression::less(this->argument.get(), static_cast<Differential*>(const_cast<AbstractExpression*>(&expr))->argument.get());
}

std::list<abs_ex> copy(const std::list<abs_ex> & list)
{
    std::list<abs_ex> res;
    for (auto &it : list)
        res.push_back(copy(it));
    return res;
}

abs_ex fullDifferential(const abs_ex &expr)
{
    //qDebug() << expr->makeStringOfExpression();

    auto set = expr->getSetOfVariables();
    abs_ex res = copy(zero);
    for (auto &it : set)
    {
        res = res + expr->derivative(it) * D(abs_ex(new Variable(getVariable(it))));
    }
    return res;
}

abs_ex D(const abs_ex &arg)
{
    return abs_ex(new Differential(arg))->downcast();
}

abs_ex D(abs_ex &&arg)
{
    return abs_ex(new Differential(std::move(arg)))->downcast();
}

#include "mathsets.h"
abs_ex D(const abs_ex & arg, int order)
{
    if (order == 1)
        return D(arg);
    if (arg->getId() > 0)
    {
        return abs_ex(new Differential(arg, order));
    }
    auto vars = arg->getSetOfVariables();
    std::vector<long long int> degrees(vars.size(), 0);
    std::vector<int> vars_vec;
    for (auto &it : vars)
        vars_vec.push_back(it);

    degrees[0] = order;

    abs_ex result = copy(zero);

    long long int factorial = fact(order);
    do
    {
        abs_ex deriv = copy(arg);
        abs_ex Diff = copy(one);
        for (int i = 0; i < vars.size(); ++i)
        {
            for (int j = 0; j < degrees[i]; ++j)
                deriv = deriv->derivative(vars_vec[i]);
            if(degrees[i] > 0)
                Diff = Diff * D(getVariableExpr(vars_vec[i]), degrees[i]);
        }
        long long int multiplier = factorial;
        for (auto &it : degrees)
            multiplier /= fact(it);
        result = result + numToAbs(multiplier)* deriv * Diff;
    }while(incrementSumm(degrees));
    return result;
}
