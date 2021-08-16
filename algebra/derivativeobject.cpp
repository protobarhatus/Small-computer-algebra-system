#include "derivativeobject.h"
#include "some_algebra_expression_conversions.h"
#include "variablesdistributor.h"
#include "number.h"
DerivativeObject::DerivativeObject(int func_var, int arg_var, int order)
{
    this->argument =getVariableExpr( func_var);
    this->order = order;
    this->func_argument_var = arg_var;
}

DerivativeObject::DerivativeObject(const abs_ex &arg, int func_arg_var, int order)
{
    this->argument = copy(arg);
    this->func_argument_var = func_arg_var;
    this->order = order;
}

DerivativeObject::DerivativeObject(abs_ex &&arg, int func_arg_var, int order)
{
    this->argument = std::move(arg);
    this->func_argument_var = func_arg_var;
    this->order = order;
}

DerivativeObject::DerivativeObject(const DerivativeObject &cop)
{
    this->argument = copy(cop.argument);
    this->order = cop.order;
    this->func_argument_var = cop.func_argument_var;
}

DerivativeObject::DerivativeObject(DerivativeObject &&cop)
{
    this->argument = std::move(cop.argument);
    this->order = cop.order;
    this->func_argument_var = cop.func_argument_var;
}

void DerivativeObject::simplify()
{
    SIM_IF_NEED
    if (order < 0 || func_argument_var < 0)
        throw "Wtf cannot be";
    this->argument->simplify();
    this->simplified = true;
}

bool DerivativeObject::canDowncastTo()
{
   if (order == 0)
       return true;
   if (this->argument->getId() == func_argument_var)
       return true;
   if (this->argument->getId() < 0)
       return true;
   return false;
}

abs_ex DerivativeObject::downcastTo()
{
    if (order == 0)
        return std::move(argument);
    if (this->argument->getId() == func_argument_var)
    {
        if (order == 1)
            return copy(one);
        return copy(zero);
    }
    if (this->argument->getId() < 0)
    {
        abs_ex deriv = std::move(argument);
        for (int i = 0; i < order; ++i)
            deriv = deriv->derivative(func_argument_var);
        return deriv;
    }
    return nullptr;
}

AlgebraExpression DerivativeObject::getId() const
{
    return DERIVATIVE_OBJECT;
}

bool DerivativeObject::operator==(AbstractExpression &right)
{
    if (right.getId() != DERIVATIVE_OBJECT)
        return false;
    auto der_obj = static_cast<DerivativeObject*>(&right);
    return *der_obj->argument == *this->argument && this->order == der_obj->order &&
            der_obj->func_argument_var == this->func_argument_var;
}

std::set<int> DerivativeObject::getSetOfVariables() const
{
    std::set<int> res;
    res.insert(argument->getId());
    return res;
}

std::set<int> DerivativeObject::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<QString> DerivativeObject::getSetOfFunctions() const
{
    return std::set<QString>();
}

Number DerivativeObject::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void DerivativeObject::_qDebugOut()
{
    assert(false);
}

QString DerivativeObject::makeStringOfExpression() const
{
    QString string = (argument)->makeStringOfExpression();
    for (int i = 0; i < order; ++i)
        string += '\'';
    return string;
}

QString DerivativeObject::toString() const
{
    return makeStringOfExpression();
}

QString DerivativeObject::makeWolframString() const
{
    QString string = (argument)->makeWolframString();
    for (int i = 0; i < order; ++i)
        string += '\'';
    string += "[" + getVariableExpr(func_argument_var)->makeWolframString() + "]";
    return string;
}

double DerivativeObject::getApproximateValue()
{
    assert(false);
    return 0;
}

double DerivativeObject::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    assert(false);
    return 0;
}

abs_ex DerivativeObject::changeSomePartOn(QString function, const abs_ex &on_what)
{
    if (argument->makeStringOfExpression() == function)
    {
        abs_ex old_arg = std::move(argument);
        argument = copy(on_what);
        return old_arg;
    }
    return argument->changeSomePartOn(function, on_what);
}

abs_ex DerivativeObject::changeSomePartOnExpression(QString part,const  abs_ex &on_what)
{
    NONCONST
            if (argument->makeStringOfExpression() == part)
            {
                abs_ex old_arg = std::move(argument);
                argument = copy(on_what);
                return old_arg;
            }
    return argument->changeSomePartOnExpression(part, on_what);
}

abs_ex DerivativeObject::derivative(int var) const
{
    if (var != func_argument_var)
        return copy(zero);
    return ::derivative(argument, var, order + 1);
}

abs_ex DerivativeObject::antiderivative(int var) const
{
    if (var == func_argument_var)
        return ::derivative(argument, var, order + 1);
    return getVariableExpr(var) * copy(this);
}

void DerivativeObject::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex> DerivativeObject::getTrigonometricalFunctions() const
{
    return this->argument->getTrigonometricalFunctions();
}

long long DerivativeObject::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long DerivativeObject::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange DerivativeObject::getRange() const
{
    return FunctionRange::getErrorRange();
}

bool DerivativeObject::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool DerivativeObject::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    return false;
}

abs_ex DerivativeObject::tryToFindExponentialFunction(int var) const
{
    return nullptr;
}

void DerivativeObject::getRidOfAbsoluteValues()
{
    if (argument->getId() == ABSOLUTE_VALUE)
        argument = getArgumentOfFunction(std::move(argument));
    argument->getRidOfAbsoluteValues();
}

void DerivativeObject::doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const
{
    func(this->argument->getId(), this->func_argument_var, this->order);
}

bool DerivativeObject::canBeZero() const
{
    return true;
}

bool DerivativeObject::hasUndefinedVariable() const
{
    return this->argument->hasUndefinedVariable();
}

int DerivativeObject::getPositionRelativelyZeroIfHasVariables()
{
    return 0;
}

bool DerivativeObject::operator<(const AbstractExpression &expr) const
{
    const DerivativeObject * der = static_cast<const DerivativeObject*>(&expr);
    if (der->order != this->order)
        return this->order > der->order;
    if (this->func_argument_var != der->func_argument_var)
        return func_argument_var < der->func_argument_var;
    return AbstractExpression::less(this->argument.get(), der->argument.get());
}

abs_ex derivative(const abs_ex &argument, int var, int order)
{
    return abs_ex (new DerivativeObject(argument, var, order))->downcast();
}

abs_ex derivative(abs_ex &&argument, int var, int order)
{
    return abs_ex (new DerivativeObject(std::move(argument), var, order))->downcast();
}
