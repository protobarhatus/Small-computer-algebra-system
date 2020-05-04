#include "absolutevalue.h"
#include "some_algebra_expression_conversions.h"
#include "polynomial.h"
#include <QDebug>
#include "fractal.h"
#include "degree.h"
AbsoluteValue::AbsoluteValue(const std::unique_ptr<AbstractExpression> & expr)
{
    this->expression = makeAbstractExpression(expr->getId(), expr.get());
    this->simplify();
}
AbsoluteValue::AbsoluteValue(std::unique_ptr<AbstractExpression> && expr)
{
    this->expression = std::move(expr);
    this->simplify();
}
AbsoluteValue::AbsoluteValue(const AbsoluteValue & value)
{
    this->expression = makeAbstractExpression(value.expression->getId(), value.expression.get());
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
bool AbsoluteValue::canDowncastTo(AlgebraExpression expr)
{
    if ((this->expression->getId() == expr && (this->expr_position > 0 || (this->expr_position < 0 && (expr == POLYNOMIAL || expr == NUMBER || expr == FRACTAL)))))
        return true;
    if (this->expr_position < 0 && expr == FRACTAL)
        return true;
    if (this->expression->getId() == FRACTAL && expr == FRACTAL)
        return true;
    return false;
}
std::unique_ptr<AbstractExpression> AbsoluteValue::downcastTo(AlgebraExpression expr)
{
    assert(canDowncastTo(expr));
    if (this->expr_position > 0)
        return makeAbstractExpression(this->expression->getId(), this->expression.get());
    if (this->expr_position == 0 && expr == FRACTAL)
    {
        Fractal * argument = static_cast<Fractal*>(this->expression.get());
        fractal_argument num, denum;
        num.push_back(abs_ex(new AbsoluteValue(abs_ex(new Number(argument->getCoefficient())))));
        auto argument_multipliers = argument->getFractal();
        for (auto &it : *argument_multipliers.first)
            num.push_back(abs_ex(new AbsoluteValue(makeAbstractExpression(it->getId(), it.get()))));
        for (auto &it : *argument_multipliers.second)
            denum.push_back(abs_ex(new AbsoluteValue(makeAbstractExpression(it->getId(), it.get()))));
        return std::unique_ptr<AbstractExpression>(new Fractal(std::move(num), std::move(denum)));
    }
    if (expr == POLYNOMIAL)
    {
        auto copy = makeAbstractExpression(this->expression->getId(), this->expression.get());
        static_cast<Polynomial*>(copy.get())->changeSign();
        return std::move(copy);
    }
    std::unique_ptr<AbstractExpression> minus(new Number(-1));
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
    return "abs(" + this->expression->makeStringOfExpression() + ")";
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
std::unique_ptr<AbstractExpression> AbsoluteValue::open()
{
    return std::move(this->expression);
}

std::unique_ptr<AbstractExpression> AbsoluteValue::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
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

std::unique_ptr<AbstractExpression> AbsoluteValue::changeSomePartOnExpression(QString part, std::unique_ptr<AbstractExpression> &on_what)
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

std::unique_ptr<AbstractExpression> AbsoluteValue::derivative(int var) const
{
    assert(false);
    return nullptr;
}

std::unique_ptr<AbstractExpression> AbsoluteValue::antiderivative(int var) const
{
    assert(false);
    return nullptr;
}

std::unique_ptr<AbstractExpression> abs(const std::unique_ptr<AbstractExpression> &expr)
{
    return abs_ex(new AbsoluteValue(expr));
}

std::unique_ptr<AbstractExpression> abs(std::unique_ptr<AbstractExpression> &&expr)
{
    return abs_ex(new AbsoluteValue(std::move(expr)));
}
