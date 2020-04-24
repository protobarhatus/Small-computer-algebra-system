#include "logarithm.h"
#include "some_algebra_expression_conversions.h"
#include "exception.h"
#include "polynomial.h"
#include "fractal.h"
#include "degree.h"
#include "constant.h"
#include "number.h"
#include <cmath>
#include "absolutevalue.h"
Logarithm::Logarithm(const std::unique_ptr<AbstractExpression> &argument)
{
    this->argument = copy(argument);
    this->simplify();
}

Logarithm::Logarithm(std::unique_ptr<AbstractExpression> &&argument)
{
    this->argument = std::move(argument);
    this->simplify();
}

Logarithm::Logarithm(const Logarithm &icopy)
{
    this->argument = copy(icopy.argument.get());
    this->simplified = icopy.simplified;
}

Logarithm::Logarithm(Logarithm &&mov)
{
    this->argument = std::move(mov.argument);
    this->simplified = mov.simplified;
}

Logarithm::~Logarithm()
{

}

AlgebraExpression Logarithm::getId() const
{
    return LOGARITHM;
}

void Logarithm::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    if (this->argument->getId() == POLYNOMIAL)
    {
        int rt = 2;
        auto root = static_cast<Polynomial*>(this->argument.get())->tryToTakeRoot(rt);
        if (root == nullptr)
        {
            rt = 3;
            root = static_cast<Polynomial*>(this->argument.get())->tryToTakeRoot(rt);
        }
        if (root != nullptr)
        {
            this->argument = takeDegreeOf(root, rt);
        }
    }
    if (this->argument->getPositionRelativelyZero() < 0)
        throw Exception();
    this->simplified = true;
}

bool Logarithm::operator==(AbstractExpression &right)
{
    if (right.getId() != LOGARITHM)
        return false;
    return *this->argument == *(static_cast<Logarithm*>(&right)->argument);
}

bool Logarithm::canDowncastTo(AlgebraExpression expr)
{
    if (*this->argument == *one && expr == NUMBER)
        return true;
    if (*this->argument == *getEuler() && expr == NUMBER)
        return true;
    if (this->argument->getId() == DEGREE && *Degree::getArgumentOfDegree(this->argument.get()) == *getEuler() && expr == Degree::getDegreeOfExpression(this->argument.get())->getId())
        return true;
    if (expr == FRACTAL && this->argument->getId() == DEGREE)
        return true;
    if (expr == POLYNOMIAL && this->argument->getId() == FRACTAL)
        return true;
    return false;
}

std::unique_ptr<AbstractExpression> Logarithm::downcastTo(AlgebraExpression expr)
{
    if (*this->argument == *getEuler())
        return copy(one);
    if (*this->argument == *one)
        return copy(zero);
    if (this->argument->getId() == DEGREE && *Degree::getArgumentOfDegree(this->argument.get()) == *getEuler())
        return Degree::getDegreeOfExpression(this->argument.get());
    if (this->argument->getId() == DEGREE && Degree::getDegreeOfExpression(this->argument.get())->getId() == NUMBER &&
            static_cast<Number*>(Degree::getDegreeOfExpression(this->argument.get()).get())->getNumerator() % 2 == 0)
    {
        AbstractExpression * arg = Degree::getArgumentOfDegree(this->argument.get());
        return Degree::getDegreeOfExpression(this->argument.get()) * ln(abs(makeAbstractExpression(arg->getId(), arg)));
    }
    if (this->argument->getId() == DEGREE)
        return Degree::getDegreeOfExpression(this->argument.get()) * ln(Degree::getArgumentOfDegree(this->argument.get()));
    if (this->argument->getId() == FRACTAL)
    {
        Fractal* fr = static_cast<Fractal*>(this->argument.get());
        abs_ex res = ln(toAbsEx(fr->getCoefficient()));
        auto fr_m = fr->getFractal();
        for (auto &it : *fr_m.first)
        {
            res = res + ln(it);
        }
        for (auto &it : *fr_m.second)
        {
            res = res - ln(it);
        }
        return res;
    }
    assert(false);
}

std::set<int> Logarithm::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> Logarithm::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Logarithm::getSetOfFunctions() const
{
    std::set<QString> set;
    set.insert(this->makeStringOfExpression());
    return set;
}

Number Logarithm::getMaxDegreeOfVariable(int id)
{
    return 0;
}

void Logarithm::_qDebugOut()
{
    qDebug() << "LOGARITHM: ";
    this->argument->_qDebugOut();
    qDebug() << "#END_OF_LOGARITHM";
}

QString Logarithm::makeStringOfExpression() const
{
    return "ln(" + this->argument->makeStringOfExpression() + ")";
}

double Logarithm::getApproximateValue()
{
    return log(this->argument->getApproximateValue());
}

double Logarithm::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    return log(this->argument->getApproximateValue(choosing_value_rule));
}

int Logarithm::getPositionRelativelyZeroIfHasVariables()
{
    return (this->argument - one)->getPositionRelativelyZero();
}

QString Logarithm::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

std::unique_ptr<AbstractExpression> Logarithm::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> Logarithm::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> Logarithm::getArgumentsCopy()
{
    return copy(argument);
}

AbstractExpression *Logarithm::getArgument()
{
    return this->argument.get();
}

std::unique_ptr<AbstractExpression> Logarithm::derivative(int var) const
{
    return this->argument->derivative(var) / this->argument;
}

std::unique_ptr<AbstractExpression> Logarithm::antiderivative(int var) const
{
    auto ln_f = checkIfItsLinearFunction(this, var);
    if (ln_f.first == nullptr)
        return nullptr;
    return one / ln_f.first * this->argument * (ln(this->argument) - one);
}

const std::unique_ptr<AbstractExpression> &Logarithm::getArgument() const
{
    return this->argument;
}

bool Logarithm::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == SINUS);
    return AbstractExpression::less(this->argument.get(), (static_cast<Logarithm*>(const_cast<AbstractExpression*>(&right))->argument.get()));
}

std::unique_ptr<AbstractExpression> ln(const std::unique_ptr<AbstractExpression>& arg)
{
    return abs_ex(new Logarithm(arg));
}

std::unique_ptr<AbstractExpression> ln(std::unique_ptr<AbstractExpression> &&arg)
{
    return abs_ex(new Logarithm(std::move(arg)));
}

std::unique_ptr<AbstractExpression> ln(AbstractExpression *arg)
{
    return abs_ex(new Logarithm(copy(arg)));
}
