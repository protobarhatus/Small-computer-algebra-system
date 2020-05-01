#include "arctangent.h"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
#include "abstractexpression.h"
#include "number.h"
#include "constant.h"
#include "variablesdistributor.h"
#include "logarithm.h"
ArcTangent::ArcTangent(const std::unique_ptr<AbstractExpression> &arg)
{
    this->argument = copy(arg);
    this->simplify();
}

ArcTangent::ArcTangent(std::unique_ptr<AbstractExpression> &&arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

ArcTangent::ArcTangent(const ArcTangent &cop)
{
    this->argument = copy(cop.argument);
}

ArcTangent::ArcTangent(ArcTangent &&mov)
{
    this->argument = std::move(mov.argument);
}

ArcTangent::~ArcTangent()
{

}

AlgebraExpression ArcTangent::getId() const
{
    return ARCTANGENT;
}

void ArcTangent::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    this->simplified = true;
}

bool ArcTangent::operator==(AbstractExpression &right)
{
    assert(this->getId() == right.getId());
    return *this->argument == *static_cast<ArcTangent*>(&right)->argument;
}

bool ArcTangent::canDowncastTo(AlgebraExpression expr)
{
    if (this->argument->getId() == TANGENT)
    {
        auto tan_arg = getArgumentOfTrigonometricalFunction(this->argument);
        int pos = (tan_arg + getPi()/two)->getPositionRelativelyZero();
        if (!(pos > 0))
            return false;
        pos = (tan_arg - getPi()/two)->getPositionRelativelyZero();
        if (!(pos < 0))
            return false;
        return true;
    }
    abs_ex & arg = this->argument;
    if (*arg == *-sqrt(numToAbs(3)) || *arg == *numToAbs(-1) || *arg == *(-sqrt(numToAbs(3))/numToAbs(3)) || *arg == *zero || *arg == *(sqrt(three)/three) ||
            *arg == *one || *arg == *sqrt(three))
        return true;
    return false;
}

std::unique_ptr<AbstractExpression> ArcTangent::downcastTo(AlgebraExpression expr)
{
    if (this->argument->getId() == TANGENT)
        return getArgumentOfTrigonometricalFunction(std::move(this->argument));
    abs_ex & arg = this->argument;
    if (*arg == *-sqrt(numToAbs(3)))
        return -getPi()/three;
    if (*arg == *numToAbs(-1))
        return -getPi()/four;
    if (*arg == *(-sqrt(numToAbs(3))/numToAbs(3)))
        return -getPi()/numToAbs(6);
    if (*arg == *zero)
        return copy(zero);
    if (*arg == *(sqrt(three)/three))
        return getPi()/numToAbs(6);
    if (*arg == *one)
        return getPi()/four;
    if (*arg == *sqrt(three))
        return getPi()/three;
    assert(false);
    return nullptr;

}

std::set<int> ArcTangent::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> ArcTangent::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> ArcTangent::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}

Number ArcTangent::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void ArcTangent::_qDebugOut()
{
    //а и нахуй это все
}

QString ArcTangent::makeStringOfExpression() const
{
    return "atan(" + this->argument->makeStringOfExpression() + ")";
}

double ArcTangent::getApproximateValue()
{
    return atan(this->argument->getApproximateValue());
}

double ArcTangent::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return atan(this->argument->getApproximateValue(choosing_value_rule));
}

int ArcTangent::getPositionRelativelyZeroIfHasVariables()
{
    return this->argument->getPositionRelativelyZero();
}

QString ArcTangent::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

std::unique_ptr<AbstractExpression> ArcTangent::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> ArcTangent::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> ArcTangent::getArgumentsCopy()
{
    return copy(this->argument);
}

std::unique_ptr<AbstractExpression> ArcTangent::derivative(int var) const
{
    return this->argument->derivative(var) / (sqr(this->argument) + one);
}

std::unique_ptr<AbstractExpression> ArcTangent::antiderivative(int var) const
{
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first == nullptr)
        return nullptr;
    abs_ex& a = ln_f.first;
    return -(ln(sqr(this->argument) + one) - two * this->argument * atan(this->argument))/two/a;
}

const std::unique_ptr<AbstractExpression> &ArcTangent::getArgument() const
{
    return this->argument;
}

bool ArcTangent::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == ARCTANGENT);
    return AbstractExpression::less(this->argument.get(), (static_cast<ArcTangent*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}


std::unique_ptr<AbstractExpression> atan(const std::unique_ptr<AbstractExpression> &arg)
{
    return abs_ex(new ArcTangent(arg));
}

std::unique_ptr<AbstractExpression> atan(std::unique_ptr<AbstractExpression> &&arg)
{
    return abs_ex(new ArcTangent(std::move(arg)));
}
