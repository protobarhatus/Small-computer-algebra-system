#include "absolutevalue.h"
#include "some_algebra_expression_conversions.h"
#include "polynomial.h"
#include <QDebug>
#include "fractal.h"
#include "degree.h"
#include "variablesdistributor.h"
AbsoluteValue::AbsoluteValue(const abs_ex & expr)
{
    this->expression = copy(expr);
    this->simplify();
}
AbsoluteValue::AbsoluteValue(abs_ex && expr)
{
    this->expression = std::move(expr);
    this->simplify();
}
AbsoluteValue::AbsoluteValue(const AbsoluteValue & value)
{
    this->expression = copy(value.expression);
    this->simplified = value.simplified;
    this->expr_position = value.expr_position;
}
AbsoluteValue::AbsoluteValue(AbsoluteValue && value)
{
    this->expression = std::move(value.expression);
    this->simplified = value.simplified;
    this->expr_position = value.expr_position;
}
void AbsoluteValue::simplify()
{
    SIM_IF_NEED
    this->expression->simplify();
    this->expression = this->expression->downcast();

    this->expr_position = this->expression->getPositionRelativelyZero();
    if (this->expr_position == 0 && this->expression->getId() == POLYNOMIAL)
    {
        //after reduceCommonPart this->expression automaticly is reduced
        this->expression = static_cast<Polynomial*>(this->expression.get())->reduceCommonPart() * this->expression;
        //во избежание некоторых ситуаций было решено, что многочлен с наибольшей степенью наименьшей по id переменной
        //должен быть с положительным коэффициентом
        //теоретически, нужно подобную вещь продумать на случай, если в полиноме нет таких переменных,
        //а есть только модули и тригонометрические функции. но это на потом, пока такая ситуация маловероятна
        if (this->expression->getId() == POLYNOMIAL && !this->expression->getSetOfPolyVariables().empty())
            this->checkIfNeedToCheckSignToLeadToTheAcceptedForm();
    }

    this->simplified = true;
}
void AbsoluteValue::checkIfNeedToCheckSignToLeadToTheAcceptedForm()
{
    assert(this->expression->getId() == POLYNOMIAL);
    if (static_cast<Polynomial*>(this->expression.get())->isNeedToChangeSignToLeadTheAcceptedForm())
        static_cast<Polynomial*>(this->expression.get())->changeSign();
}
AlgebraExpression AbsoluteValue::getId() const
{
    return ABSOLUTE_VALUE;
}
bool AbsoluteValue::operator==(AbstractExpression &expr)
{
    if (expr.getId() != ABSOLUTE_VALUE)
        return false;
    AbsoluteValue * expr_ptr = static_cast<AbsoluteValue*>(&expr);
    return *this->expression == *expr_ptr->expression;
}
bool AbsoluteValue::canDowncastTo()
{
    if (this->expr_position > 0 || this->expr_position < 0)
        return true;
    if (this->expression->getId() == FRACTAL)
        return true;
    if (this->expression->getId() == DEGREE)
        return true;
    if (isIntegratingConstantAndCanChangeIt(this->expression->getId()))
        return true;
    return false;
}
abs_ex AbsoluteValue::downcastTo()
{
    assert(canDowncastTo());
    if (this->expr_position > 0)
        return std::move(this->expression);
    if (this->expr_position == 0 && this->expression->getId() == FRACTAL)
    {
        Fractal * argument = static_cast<Fractal*>(this->expression.get());
        fractal_argument num, denum;
        num.push_back(abs_ex(new AbsoluteValue(abs_ex(new Number(argument->getCoefficient())))));
        auto argument_multipliers = argument->getFractal();
        for (auto &it : *argument_multipliers.first)
            num.push_back(abs_ex(new AbsoluteValue(std::move(it))));
        for (auto &it : *argument_multipliers.second)
            denum.push_back(abs_ex(new AbsoluteValue(std::move(it))));
        return abs_ex(new Fractal(std::move(num), std::move(denum)));
    }
    if (this->expr_position < 0 && this->expression->getId() == POLYNOMIAL)
    {
        auto copy = std::move(this->expression);
        static_cast<Polynomial*>(copy.get())->changeSign();
        return std::move(copy);
    }
    if (this->expression->getId() == DEGREE)
    {
        auto arg = Degree::getArgumentOfDegree(this->expression.get());
        return pow(abs(copy(arg)), Degree::getDegreeOfExpression(this->expression.get()));
    }
    if (isIntegratingConstantAndCanChangeIt(this->expression->getId()))
    {
        return integratingConstantExpr(this->expression->getId(), this->getRange());
    }
    abs_ex minus(new Number(-1));
    return this->expression * minus;
}
std::set<int> AbsoluteValue::getSetOfPolyVariables() const
{
    //я тут подумал, что переменные в модуле не являются полиномными, но посмотрим.
    //как минимум на них нельзя опираться при корне и делении
    //return this->expression->getSetOfPolyVariables();
    return std::set<int>();
}
std::set<int> AbsoluteValue::getSetOfVariables() const
{
    return this->expression->getSetOfVariables();
}

std::set<QString> AbsoluteValue::getSetOfFunctions() const
{
    return this->expression->getSetOfFunctions();
}
Number AbsoluteValue::getMaxDegreeOfVariable(int id)
{
    return this->expression->getMaxDegreeOfVariable(id);
}
void AbsoluteValue::_qDebugOut()
{
    qDebug() << "ABSOLUTE VALUE EXPRESSION:";
    this->expression->_qDebugOut();
    qDebug() << "END OF ABSOLUTE VALUE EXPRESSION;";
}
QString AbsoluteValue::makeStringOfExpression() const
{
    return "|" + this->expression->makeStringOfExpression() + "|";
}

QString AbsoluteValue::makeWolframString() const
{
    return "Abs[" + this->expression->makeWolframString() + "]";
}

QString AbsoluteValue::toString() const
{
    return "|" + this->expression->toString() + "|";
}
double AbsoluteValue::getApproximateValue()
{
    return abs(this->expression->getApproximateValue());
}
double AbsoluteValue::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    return abs(this->expression->getApproximateValue(choosing_value_rule));
}
bool AbsoluteValue::operator<(const AbstractExpression &expr) const
{
    AbsoluteValue * deg = static_cast<AbsoluteValue*>(const_cast<AbstractExpression*>(&expr));
    return AbstractExpression::less(this->expression.get(), deg->expression.get());
}
int AbsoluteValue::getPositionRelativelyZeroIfHasVariables()
{
    return 1;
}
abs_ex AbsoluteValue::open()
{
    return std::move(this->expression);
}

abs_ex AbsoluteValue::changeSomePartOn(QString part, abs_ex &on_what)
{
   // NONCONST
    if (this->expression->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->expression.swap(cop);
        return cop;
    }
    return this->expression->changeSomePartOn(part, on_what);
}

abs_ex AbsoluteValue::changeSomePartOnExpression(QString part, abs_ex &on_what)
{
    NONCONST
        if (this->expression->makeStringOfExpression() == part)
        {
            abs_ex cop = copy(on_what);
            this->expression.swap(cop);
            return cop;
        }
        return this->expression->changeSomePartOn(part, on_what);
}

abs_ex AbsoluteValue::derivative(int var) const
{
    return this->expression->derivative(var);
    assert(false);
    return nullptr;
}

abs_ex AbsoluteValue::antiderivative(int var) const
{
    assert(false);
    return nullptr;
}

AbstractExpression *AbsoluteValue::getExpression()
{
    return this->expression.get();
}

void AbsoluteValue::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->expression->setSimplified(simpl);
}

std::set<abs_ex > AbsoluteValue::getTrigonometricalFunctions() const
{
    return this->expression->getTrigonometricalFunctions();
}

long long AbsoluteValue::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->expression->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long AbsoluteValue::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->expression->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange AbsoluteValue::getRange() const
{
    FunctionRange arg_range = this->expression->getRange();
    if (arg_range.isError())
        return arg_range;
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (it.canBeBiggerThanZero())
        {
            if (it.canBeLowerThanZero())
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(copy(zero), it.max(), true, it.isMaxIncluded()));
            else
                result.addSegmentWithoutNormilizing(it);
        }
    }
    return result;
}

bool AbsoluteValue::hasDifferential() const
{
    return this->expression->hasDifferential();
}

bool AbsoluteValue::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    if (second->getId() == ABSOLUTE_VALUE)
    {
        auto arg = getArgumentOfFunction(second);
        if (canBeConsideredAsConstantThatCanBeChanged(expression) && canBeConsideredAsConstantThatCanBeChanged(arg))
        {
            this->expression = integratingConstantExpr(unification(expression->getRange(), arg->getRange()));
            return true;
        }
        return expression->tryToMergeIdenticalBehindConstantExpressions(getArgumentOfFunction(second));
    }
    return false;
}

abs_ex AbsoluteValue::tryToFindExponentialFunction(int var) const
{
    return this->expression->tryToFindExponentialFunction(var);
}

abs_ex abs(const abs_ex &expr)
{
    return abs_ex(new AbsoluteValue(expr))->downcast();
}

abs_ex abs(abs_ex &&expr)
{
    return abs_ex(new AbsoluteValue(std::move(expr)))->downcast();
}
