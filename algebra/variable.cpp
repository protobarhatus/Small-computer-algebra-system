#include "variable.h"
#include "number.h"
#include "variablesnamedistributor.h"
#include "variablesdistributor.h"
#include "cmath"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
int Variable::id_counter = 0;
int Variable::system_id_counter;
int Variable::integrating_constant_id_counter = 0;
Variable::Variable()
{
    assert(false);
}
Variable::Variable(int id, VariablesDefinition * def)
{
    assert(id <= Variable::id_counter);
    this->id = id;
    this->name = VariablesNameDistributor::getName(id);
    this->definition = def;
}
//вообще, костыльный конструктор, мне следует получше разобраться с выдачей переменных
Variable::Variable(int id, QString iname)
{
    this->id = id;
    if (isIntegratingConstant(id))
        VariablesDistributor::increaseIntegratingConstant(id);
    this->name = iname;
    if (id >= VariablesDistributor::firstSystemNum())
    {
        this->definition = VariablesDistributor::getVariablesDefinition(id);
    }
    //VariablesNameDistributor::addVariable(id, name);
    //if (this->id >= Variable::id_counter)
    //    Variable::id_counter = id;
}

Variable::Variable(int id, QString name, VariablesDefinition *def)
{
    this->id = id;
    if (isIntegratingConstant(id))
        VariablesDistributor::increaseIntegratingConstant(id);
    this->name = name;
    this->definition = def;
}

Variable::~Variable()
{
    if (isIntegratingConstant(id))
        VariablesDistributor::decreaseIntegratingConstant(id);
}
Variable::Variable(const Variable & var)
{
    this->id = var.id;
    if (isIntegratingConstant(id))
        VariablesDistributor::increaseIntegratingConstant(id);
    this->name = var.name;
    this->definition = var.definition;
}
AlgebraExpression Variable::getId() const
{
    return (AlgebraExpression)this->id;
}
void Variable::simplify()
{

}
QString Variable::getName() const
{
    return this->name;
}
bool Variable::operator<(const AbstractExpression &right) const
{
    assert(right.getId() > 0);
    Variable * var = static_cast<Variable*>(const_cast<AbstractExpression*>(&right));
    return this->id < var->id;

}
bool Variable::operator==(AbstractExpression &right)
{
    if (right.getId() <= 0)
        return false;
    Variable * var = static_cast<Variable*>(&right);
    return this->id == var->id;
}
std::set<int> Variable::getSetOfPolyVariables() const
{
    std::set<int> set;
    set.insert((int)this->id);
    return set;
}
std::set<int> Variable::getSetOfVariables() const
{
    std::set<int> set;
    set.insert((int)this->id);
    return set;
}

std::set<QString> Variable::getSetOfFunctions() const
{
    return std::set<QString>();
}
Number Variable::getMaxDegreeOfVariable(int _id)
{
    if (_id == this->id)
        return 1;
    return 0;
}
bool Variable::canDowncastTo()
{
    if (this->definition->getRange().isPoint())
        return true;
    return false;
}
abs_ex Variable::downcastTo()
{
    if (this->definition->getRange().isPoint())
        return this->definition->getRange().getPoint();
   // assert(false);
    return abs_ex(nullptr);
}
void Variable::_qDebugOut()
{
    qDebug() << "VARIABLE: " << this->name;
}
QString Variable::makeStringOfExpression() const
{
    return this->getName();
}

QString Variable::makeWolframString() const
{
    return this->makeStringOfExpression();
}

QString Variable::toString() const
{
    return this->makeStringOfExpression();
}

QString Variable::makeRenderString() const
{
    return this->toString();
}
double Variable::getApproximateValue()
{
    assert(false);
    return std::nan("0");
}
Variable::Variable(int id)
{
   // assert(id <= Variable::id_counter);
    this->id = id;
    if (isIntegratingConstant(id))
        VariablesDistributor::increaseIntegratingConstant(id);
    this->definition = VariablesDistributor::getVariablesDefinition(id);
    if (id <= Variable::id_counter)
    {

        this->name = VariablesNameDistributor::getName(id);
    }
    else
    {
       // this->definition = VariablesDistributor::get().system_var_def;
        this->name = makeVariablesName(id);
    }
}
int Variable::getPositionRelativelyZeroIfHasVariables()
{
    if (this->definition->getMaxValue() <= 0)
        return -1;
    if (this->definition->getMinValue() >= 0)
        return 1;
    return 0;
}
double Variable::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    return choosing_value_rule(this->definition);
}

abs_ex Variable::changeSomePartOn(QString part, const abs_ex &on_what)
{
    return nullptr;
}

abs_ex Variable::changeSomePartOnExpression(QString part, const abs_ex &on_what)
{
    return nullptr;
}

abs_ex Variable::derivative(int var) const
{
    int spec_der = this->definition->getSpecificDerivative(var);
    if (spec_der >= 0)
        return getVariableExpr(spec_der);

    if (var == this->id)
        return copy(one);
    return copy(zero);
}

abs_ex Variable::antiderivative(int var) const
{
    int spec_int = this->definition->getSpecificIntegral(var);
    if (spec_int >= 0)
        return getVariableExpr(spec_int);

    if (this->getId() != var)
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    return takeDegreeOf(abs_ex(new Variable(getVariable(var))), 2) / two;
}

void Variable::setSimplified(bool simpl)
{
    this->simplified = simpl;
}

std::set<abs_ex > Variable::getTrigonometricalFunctions() const
{
    return std::set<abs_ex>();
}

long long Variable::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    if (var != id)
        return 0;
    return 1;
}

long long Variable::getGcdOfNumeratorsOfDegrees(int var) const
{
    if (var != id)
        return 0;
    return 1;
}

FunctionRange Variable::getRange() const
{
    return this->definition->getRange();
}

bool Variable::hasDifferential() const
{
    return false;
}

bool Variable::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    return *this == *second;
}

abs_ex Variable::tryToFindExponentialFunction(int var) const
{
    return nullptr;
}

void Variable::getRidOfAbsoluteValues()
{

}

void Variable::doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const
{
    func(id, 0, 0);
}

void Variable::setName(const QString &name)
{
    this->name = name;
    VariablesDistributor::addSpecialName(id, name);
}

void Variable::setRange(const FunctionRange &range)
{
    this->definition->setRange(range);
}

bool Variable::canBeZero() const
{
    return this->definition->getRange().containsZero();
}

bool Variable::hasUndefinedVariable() const
{
    return this->definition->isUndefined();
}



abs_ex varToAbsEx(const Variable &var)
{
    return abs_ex(new Variable(var));
}
