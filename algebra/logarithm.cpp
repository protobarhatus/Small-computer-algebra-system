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
#include "solving_equations.h"
#include "arctangent.h"
#include "sinus.h"
#include "cosinus.h"
#include "factorization.h"
#include "variablesdistributor.h"
Logarithm::Logarithm(const abs_ex &argument)
{
    this->argument = copy(argument);
    this->simplify();
}

Logarithm::Logarithm(abs_ex &&argument)
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
    if (Degree::getArgumentOfDegree(this->argument.get())->getId() == TANGENT)
    {
        this->argument = pow(sin(getArgumentOfFunction(argument))/
                             cos(getArgumentOfFunction(argument)),
                             Degree::getDegreeOfExpression(argument.get()));
    }
    if (Degree::getArgumentOfDegree(this->argument.get())->getId() == COTANGENT)
    {
        this->argument = pow(cos(getArgumentOfFunction(argument))/
                             sin(getArgumentOfFunction(argument)),
                             Degree::getDegreeOfExpression(argument.get()));
    }
    if (this->argument->getId() == POLYNOMIAL)
    {
        auto common_part = static_cast<Polynomial*>(this->argument.get())->reduceCommonPart();
        if (common_part != nullptr && !(*common_part == *one))
            this->argument = this->argument * common_part;
    }
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
    if (this->argument->getId() == POLYNOMIAL && this->argument->getSetOfPolyVariables().size() > 0)
    {
        auto facts = factorizePolynom(this->argument);
        if (facts.first.size() > 1)
        {
            abs_ex fractal (new Fractal(facts.second));
            for (auto &it : facts.first)
                fractal = std::move(fractal) * std::move(it);
            this->argument = std::move(fractal);
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
    return subCompare(this->argument, static_cast<Logarithm*>(&right)->argument);
}

bool Logarithm::canDowncastTo()
{
    if (*this->argument == *one)
        return true;
    if (*this->argument == *getEuler())
        return true;
    if (this->argument->getId() == DEGREE && *Degree::getArgumentOfDegree(this->argument.get()) == *getEuler())
        return true;
    if (this->argument->getId() == DEGREE)
        return true;
    if (this->argument->getId() == FRACTAL)
    {
        auto frac = static_cast<Fractal*>(this->argument.get());
        if (((frac->getFractal().first->size() == 1 && frac->getFractal().second->empty())
                ) &&
                frac->getCoefficient() == -1)
            return false;
        return true;
    }
    if (this->argument->getId() == NUMBER)
    {
        Number num = *static_cast<Number*>(this->argument.get());
        if (!num.isInteger())
            return true;
        auto facts = factorize(num.getNumerator());
        if (facts.size() > 1 || (facts.size() == 1 && facts.begin()->second > 1))
            return true;
    }
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
        return true;

    if (isInf(this->argument))
        return true;
    return false;
}

abs_ex Logarithm::downcastTo()
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
        abs_ex degree = Degree::getDegreeOfExpressionMoved(this->argument);
        return std::move(degree) * ln(abs(Degree::getArgumentOfDegreeMoved(this->argument)));
    }
    if (this->argument->getId() == DEGREE)
    {
        abs_ex degree = Degree::getDegreeOfExpressionMoved(this->argument);
        return std::move(degree) * ln(Degree::getArgumentOfDegreeMoved(this->argument));
    }
    if (this->argument->getId() == FRACTAL)
    {
        Fractal* fr = static_cast<Fractal*>(this->argument.get());
        if (fr->getCoefficient().getNumerator() > 0)
        {
            abs_ex res = ln(toAbsEx(fr->getCoefficient()));
            auto fr_m = fr->getFractal();
            for (auto &it : *fr_m.first)
            {
                res = res + ln(it);
            }
            for (auto &it : *fr_m.second)
            {
               // qDebug() << ln(it)->toString();
                res = res - ln(it);
            }
            return res;
        }
        if (!(fr->getFractal().first->size() == 1 && fr->getFractal().second->size() == 0))
        {
            abs_ex res = ln(toAbsEx(fr->getCoefficient() * -1));
            if (fr->getFractal().first->empty())
            {
                res = std::move(res) - ln(-(*fr->getFractal().second->begin()));
                for (auto it = next(fr->getFractal().second->begin()); it != fr->getFractal().second->end(); ++it)
                    res = std::move(res) - ln(*it);
            }
            else
            {
                res = std::move(res) + ln(-(*fr->getFractal().first->begin()));
                for (auto it = next(fr->getFractal().first->begin()); it != fr->getFractal().first->end(); ++it)
                    res = std::move(res) + ln(*it);
                for (auto &it : *fr->getFractal().second)
                    res = std::move(res) - ln(it);
            }
            return res;
        }
       // res = std::move(res) + ln()
    }
    if (this->argument->getId() == NUMBER)
    {
        Number num = *static_cast<Number*>(this->argument.get());
        if (!num.isInteger())
            return ln(numToAbs(num.getNumerator())) - ln(numToAbs(num.getDenominator()));
        auto facts = factorize(num.getNumerator());


        if (facts.size() > 1 || (facts.size() == 1 && facts.begin()->second > 1))
        {
            abs_ex res = copy(zero);
            for (auto &it : facts)
                res = std::move(res) + numToAbs(it.second) * ln(numToAbs(it.first));
            return res;
        }

    }
    if (isIntegratingConstantAndCanChangeIt(this->argument->getId()))
    {
        return integratingConstantExpr(this->argument->getId(), this->getRange());
    }
    if (isInf(this->argument))
        return getInf();
    return nullptr;
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

QString Logarithm::makeWolframString() const
{
    return "Log[" + argument->makeWolframString() + "]";
}

QString Logarithm::toString() const
{
    return "ln(" + this->argument->toString() + ")";
}

QString Logarithm::makeRenderString() const
{
    QString argstr = this->argument->makeRenderString();
    if (argstr.length() > 2)
        return "ln!(" + argstr + ")";
    return "ln(" + argstr + ")";
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

abs_ex Logarithm::getArgumentMoved()
{
    return std::move(this->argument);
}

abs_ex Logarithm::changeSomePartOn(QString part, const abs_ex &on_what)
{
   // NONCONST
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

abs_ex Logarithm::changeSomePartOnExpression(QString part, const abs_ex &on_what)
{
    NONCONST
            return changeSomePartOn(part, on_what);
}

abs_ex Logarithm::getArgumentsCopy()
{
    return copy(argument);
}

AbstractExpression *Logarithm::getArgument()
{
    return this->argument.get();
}

abs_ex Logarithm::derivative(int var) const
{
    return this->argument->derivative(var) / this->argument;
}
#include "variablesdistributor.h"
abs_ex Logarithm::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first != nullptr)
        return one / ln_f.first * this->argument * (ln(this->argument) - one);
    auto qc_f = checkIfItsQuadraticFunction(argument, var);
    if (qc_f[0] != nullptr)
    {
        abs_ex x(new Variable(getVariable(var)));
        auto a = std::move(qc_f[0]);
        auto b = std::move(qc_f[1]);
        auto c = std::move(qc_f[2]);
        auto D = sqr(b) - four*a*c;
        //D->getPositionRelativelyZero в логарифме не может быть > 0
        auto D_sqrt = sqrt(-D);
        auto der = two*a*x + b;
        return (two*D_sqrt *atan(der/D_sqrt) + der*ln(abs(argument)) - four*a*x)/two/a;
    }
    int lcm_of_denoms = this->getLcmOfDenominatorsOfDegreesOfVariable(var);
    if (lcm_of_denoms > 1)
    {
        abs_ex t;
        if (lcm_of_denoms % 2 == 0)
            t = systemVarExpr(zero, nullptr, true, false);
        else
            t = systemVarExpr();
        abs_ex cop = copy(this);
        setUpExpressionIntoVariable(cop, pow(t, lcm_of_denoms), var);
        cop = std::move(cop) * pow(t, lcm_of_denoms)->derivative(t->getId());
       // qDebug() << cop->toString();
        auto integr = cop->antiderivative(t->getId());

        if (integr != nullptr)
        {
            setUpExpressionIntoVariable(integr, pow(getVariableExpr(var), one/numToAbs(lcm_of_denoms)), t->getId());
            return integr;
        }

    }
    auto ln_deg = checkIfItsDegreeOfLinearFunction(argument, var);
    if (ln_deg.first != nullptr)
    {
        abs_ex t = systemVarExpr();
        abs_ex cop = copy(this);
        setUpExpressionIntoVariable(cop, (t - ln_deg.second) / ln_deg.first, var);
        auto integr = cop->antiderivative(t->getId());
        if (integr != nullptr)
        {
            setUpExpressionIntoVariable(integr, ln_deg.first * getVariableExpr(var) + ln_deg.second, t->getId());
            return integr / ln_deg.first;
        }
        return nullptr;
    }
    return nullptr;
    return nullptr;
}

const abs_ex &Logarithm::getArgument() const
{
    return this->argument;
}

void Logarithm::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
}

std::set<abs_ex > Logarithm::getTrigonometricalFunctions() const
{
    return this->argument->getTrigonometricalFunctions();
}

long long Logarithm::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    return this->argument->getLcmOfDenominatorsOfDegreesOfVariable(var);
}

long long Logarithm::getGcdOfNumeratorsOfDegrees(int var) const
{
    return this->argument->getGcdOfNumeratorsOfDegrees(var);
}

FunctionRange Logarithm::getRange() const
{
    FunctionRange arg_range = this->argument->getRange();
    if (arg_range.isError())
        return arg_range;
    if (arg_range.getMax() != nullptr && !bigger(arg_range.getMax(), zero))
        return FunctionRange();
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (it.max() != nullptr && !bigger(it.max(), zero))
            continue;
        if (it.min() == nullptr || !bigger(it.min(), zero))
        {
            if (it.max() == nullptr)
                return FunctionRange(nullptr, nullptr, false, false);
            else
                result.addSegmentWithoutNormilizing(nullptr, ln(it.max()), false, it.isMaxIncluded());
        }
        else
        {
            if (it.max() == nullptr)
                result.addSegmentWithoutNormilizing(ln(it.min()), nullptr, it.isMinIncluded(), false);
            else
                result.addSegmentWithoutNormilizing(ln(it.min()), ln(it.max()), it.isMinIncluded(), it.isMaxIncluded());
        }
    }
    return result;

}

bool Logarithm::hasDifferential() const
{
    return this->argument->hasDifferential();
}

bool Logarithm::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    if (second->getId() == this->getId())
    {
        auto arg = getArgumentOfFunction(second);
        if (canBeConsideredAsConstantThatCanBeChanged(argument) && canBeConsideredAsConstantThatCanBeChanged(arg))
        {
            this->argument = integratingConstantExpr(unification(argument->getRange(), arg->getRange()));
            return true;
        }
        return argument->tryToMergeIdenticalBehindConstantExpressions(getArgumentOfFunction(second));
    }
    return false;
}

abs_ex Logarithm::tryToFindExponentialFunction(int var) const
{
    return this->argument->tryToFindExponentialFunction(var);
}

void Logarithm::getRidOfAbsoluteValues()
{
    NONCONST
    if (this->argument->getId() == ABSOLUTE_VALUE)
        this->argument = getArgumentOfFunction(argument);
    this->argument->getRidOfAbsoluteValues();
    this->simplify();
}

void Logarithm::doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const
{
    this->argument->doSomethingInDerivativeObject(func);
}

bool Logarithm::canBeZero() const
{
    return (this->argument - one)->canBeZero();
}

bool Logarithm::hasUndefinedVariable() const
{
    return this->argument->hasUndefinedVariable();
}

bool Logarithm::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == LOGARITHM);
    //return AbstractExpression::less(this->argument.get(), (static_cast<Logarithm*>(const_cast<AbstractExpression*>(&right))->argument.get()));
    return less(this->argument.get(), static_cast<const Logarithm*>(&right)->argument.get());
}

abs_ex ln(const abs_ex& arg)
{
    return abs_ex(new Logarithm(arg))->downcast();
}

abs_ex ln(abs_ex &&arg)
{
    return abs_ex(new Logarithm(std::move(arg)))->downcast();
}

abs_ex ln(AbstractExpression *arg)
{
    return abs_ex(new Logarithm(copy(arg)))->downcast();
}
