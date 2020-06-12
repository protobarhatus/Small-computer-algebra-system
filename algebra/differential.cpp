#include "differential.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
#include <cmath>
#include "variablesdistributor.h"
Differential::Differential(const std::unique_ptr<AbstractExpression> &arg)
{
    this->argument = copy(arg);
    this->simplify();
}

Differential::Differential(std::unique_ptr<AbstractExpression> &arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

Differential::Differential(const Differential &cop)
{
    this->argument = copy(cop.argument);
}

Differential::Differential(Differential &&mov)
{
    this->argument = std::move(mov.argument);
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

std::unique_ptr<AbstractExpression> Differential::downcastTo()
{
    if (this->argument->getId() == NUMBER)
        return copy(zero);
    return fullDifferential(this->argument);
    assert(false);
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
    s.insert(this->makeStringOfExpression());
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
    if (this->argument->getId() > 0)
        return "d" + this->argument->makeStringOfExpression();
    return "d(" + this->argument->makeStringOfExpression() + ")";
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

std::unique_ptr<AbstractExpression> Differential::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> Differential::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
   // NONCONST
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> Differential::changeSomePartOnExpression(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    NONCONST
        return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> Differential::getArgumentsCopy()
{
    return copy(this->argument);
}

std::unique_ptr<AbstractExpression> Differential::derivative(int var) const
{
    //производная от дифференциала? может быть дифференциал более высокого порядка? пока хз
    assert(false);
    return nullptr;
}

AbstractExpression *Differential::getArgument()
{
    return this->argument.get();
}

std::unique_ptr<AbstractExpression> Differential::antiderivative(int var) const
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

std::set<std::unique_ptr<AbstractExpression> > Differential::getTrigonometricalFunctions() const
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

bool Differential::operator<(const AbstractExpression &expr) const
{
    assert(expr.getId() == DIFFERENTIAL);
    return AbstractExpression::less(this->argument.get(), static_cast<Differential*>(const_cast<AbstractExpression*>(&expr))->argument.get());
}

std::unique_ptr<AbstractExpression> fullDifferential(const std::unique_ptr<AbstractExpression> &expr)
{
    auto set = expr->getSetOfVariables();
    abs_ex res = copy(zero);
    for (auto &it : set)
    {
        res = res + expr->derivative(it) * D(abs_ex(new Variable(getVariable(it))));
    }
    return res;
}

std::unique_ptr<AbstractExpression> D(const std::unique_ptr<AbstractExpression> &arg)
{
    return abs_ex(new Differential(arg));
}
