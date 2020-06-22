#include "degree.h"
#include "number.h"
#include "some_algebra_expression_conversions.h"
#include "assert.h"
#include "fractal.h"
#include "cmath"
#include "limits"
#include "exception.h"
#include "polynomial.h"
#include "factorization.h"
#include "primetest.h"
#include "absolutevalue.h"
#include "logarithm.h"
#include "constant.h"
#include "sinus.h"
#include "cosinus.h"
#include "logarithm.h"
#include "tangent.h"
#include "cotangent.h"
#include "variablesdistributor.h"
#include "arcsinus.h"
Degree::Degree(abs_ex && iargument, abs_ex && idegree)
{
    this->argument = std::move(iargument);
    this->degree = std::move(idegree);
    this->simplify();
}
Degree::Degree(const abs_ex & arg, const abs_ex & deg)
{
    this->argument = makeAbstractExpression(arg->getId(), arg.get());
    this->degree = makeAbstractExpression(deg->getId(), deg.get());
    this->simplify();
}
Degree::Degree(const abs_ex & arg, Number deg)
{
    this->argument = makeAbstractExpression(arg->getId(), arg.get());
    this->degree = abs_ex(new Number(deg));
    this->simplify();
}
Degree::Degree(abs_ex && arg, Number deg)
{
    this->argument = std::move(arg);
    this->degree = abs_ex(new Number(deg));
    this->simplify();
}
bool Degree::isArgument()
{
    return this->degree->getId() == NUMBER && (static_cast<Number*>(this->degree.get()))->isOne() == 1;
}

AlgebraExpression Degree::getId() const
{
    return DEGREE;
}
bool Degree::operator==(AbstractExpression &right)
{
    if (right.getId() != DEGREE)
        return false;
    Degree * deg = static_cast<Degree*>(&right);
    return *this->argument == *deg->argument && *this->degree == *deg->degree;
}
Degree::Degree(const Degree & deg)
{
    this->degree = makeAbstractExpression(deg.degree->getId(), deg.degree.get());
    this->argument = makeAbstractExpression(deg.argument->getId(), deg.argument.get());
    this->simplified = deg.simplified;
    this->simplify();
}
bool Degree::operator<(const AbstractExpression &right) const
{
    Degree * deg = static_cast<Degree*>(const_cast<AbstractExpression*>(&right));
    if (AbstractExpression::less(this->degree.get(), deg->degree.get()))
        return true;
    if (AbstractExpression::less(deg->degree.get(), this->degree.get()))
        return false;
    return AbstractExpression::less(this->argument.get(), deg->argument.get());
}
bool Degree::canDowncastTo()
{
    if (this->argument->getId() == FRACTAL)
    {
        Fractal * frac = static_cast<Fractal*>(this->argument.get());
        auto fr = frac->getFractal();
        //это на случай подобно sqrt(-a), чтобы не пытался разложить на sqrt(-1)*sqrt(a)
        if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->getDenominator() % 2 == 0 &&
                ((fr.first->size() == 1 && fr.second->empty())) &&
                frac->getCoefficient() == -1)
            return false;
        return true;
    }
    if (this->degree->getPositionRelativelyZero() < 0)
        return true;
    if (this->argument->getId() == NUMBER && !static_cast<Number*>(this->argument.get())->isInteger())
        return true;
    if (this->argument->getId() == NUMBER && factorize(abs(static_cast<Number*>(this->argument.get())->getNumerator())).size() != 1)
        return true;
    //there must be check on limit but i'll do it later
    //giving a set of expressions in parenthesses (like (a+b)^2 = (a+b)(a+b)) is in another function. operations with Numbers as argument must be realized in simplify()
    if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isZero())
        return true;
    if (this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isOne())
        return true;
    if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isOne())
        return true;
    if (*this->argument == *getEuler() && this->degree->getId() == LOGARITHM)
        return true;
    if (*this->argument == *getEuler() && Degree::getArgumentOfDegree(this->degree.get())->getId() == LOGARITHM &&
            (Degree::getDegreeOfExpression(degree.get()) - one)->getPositionRelativelyZero() > 0)
        return true;
    if (*this->argument == *getEuler() && this->degree->getId() == FRACTAL && static_cast<Fractal*>(this->degree.get())->tryToFindLogarithmInNumerator() !=
            nullptr)
        return true;

    if (*this->argument == *getEuler() && this->degree->getId() == POLYNOMIAL &&
            static_cast<Polynomial*>(this->degree.get())->hasLogarithmicMonoms())
        return true;
    if (this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isZero())
        return true;
    if (*this->argument == *getEuler() && this->degree->getId() == FRACTAL && static_cast<Fractal*>(this->degree.get())->canTurnIntoPolynomWithOpeningParentheses(true) &&
            static_cast<Fractal*>(this->degree.get())->turnIntoPolynomWithOpeningParentheses(true)->hasLogarithmicMonoms())
        return true;
   /* if (this->degree->getId() == FRACTAL && !static_cast<Fractal*>(this->degree.get())->getCoefficient().isOne() &&
            this->argument->getId() == NUMBER)
        return true;*/
    if (this->isOnlyVarsIntegratingConstants())
        return true;
    if (this->argument->getSetOfVariables().empty())
    {
        if (this->degree->getId() == FRACTAL && static_cast<Fractal*>(degree.get())->hasIntegratingConstant())
            return true;
        if (this->degree->getId() == POLYNOMIAL && static_cast<Polynomial*>(degree.get())->hasIntegratingConstant())
            return true;
    }
    return false;
}
abs_ex Degree::downcastTo()
{

    assert(canDowncastTo());

    if (*this->argument == *getEuler() && this->degree->getId() == LOGARITHM)
        return static_cast<Logarithm*>(this->degree.get())->getArgumentMoved();
    if (*this->argument == *getEuler() && Degree::getArgumentOfDegree(this->degree.get())->getId() == LOGARITHM)
    {
        auto arg = static_cast<Logarithm*>(Degree::getArgumentOfDegree(degree.get()))->getArgumentsCopy();
        return pow(arg, pow(copy(Degree::getArgumentOfDegree(degree.get())), Degree::getDegreeOfExpression(degree.get()) - one));
    }

    if ((this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isOne()) || static_cast<Number*>(this->degree.get())->isZero())
        return abs_ex(new Number(1));
    if (this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isZero())
        return numToAbs(0);
    if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isOne())
        return std::move(argument);
    if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isZero())
        return numToAbs(1);
    if (this->argument->getId() == NUMBER && !static_cast<Number*>(this->argument.get())->isInteger())
    {
        fractal_argument num, denum;
        if (static_cast<Number*>(this->argument.get())->getNumerator() != 1)
        {
            Number num_coe = static_cast<Number*>(this->argument.get())->getNumerator();
            num.push_back(abs_ex(new Degree(makeAbstractExpression(NUMBER, &num_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
            num.begin()->get()->simplify();
        }
        Number denom_coe = static_cast<Number*>(this->argument.get())->getDenominator();
        denum.push_back(abs_ex(new Degree(makeAbstractExpression(NUMBER, &denom_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        denum.begin()->get()->simplify();
        return abs_ex(new Fractal(&num, &denum));
    }    
    else if (this->argument->getId() == NUMBER)
    {
        bool is_negative = static_cast<Number*>(this->argument.get())->getNumerator() < 0;
        auto multiples = factorize(abs(static_cast<Number*>(this->argument.get())->getNumerator()));
        if (multiples.size() != 1)
        {
            fractal_argument num, den;

            //раскладываем степень на произведение степеней чисел только с одним множителем. к примеру, root(3, 3*3*5*5) = root(3, 3*3)*root(5*5)
            for (auto &it : multiples)
            {
                bool s;
                num.push_back(abs_ex(new Degree(abs_ex(new Number(power(it.first, it.second, s))),
                                                                             makeAbstractExpression(this->degree->getId(), this->degree.get()))));
            }
            return abs_ex(new Fractal(std::move(num), std::move(den), is_negative ? -1 : 1));
        }
    }
    else if (this->argument->getId() == FRACTAL)
    {
        auto fract = static_cast<Fractal*>(this->argument.get())->getFractal();

        //though this degree suppose to be destroyed after this function, I would use safe copy instead of moving in denumenator of fractal

        fractal_argument num, denum;
        Number num_coe = static_cast<Fractal*>(this->argument.get())->getCoefficient().getNumerator();
        Number denom_coe = Number( static_cast<Fractal*>(this->argument.get())->getCoefficient().getDenominator());
        if (num_coe.getNumerator() < 0 && this->degree->getId() == NUMBER &&
                static_cast<Number*>(this->degree.get())->getDenominator() % 2 == 0)
        {

            if (!(num_coe * -1).isOne())
                num.push_back(pow(numToAbs(-num_coe.getNumerator()), degree));
            if (!denom_coe.isOne())
                denum.push_back(abs_ex(new Degree(makeAbstractExpression(NUMBER, &denom_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
            if(fract.first->empty())
            {

                auto it = fract.second->begin();
                denum.push_back(pow(-(*it), copy(degree)));
                ++it;
                while (it != fract.second->end())
                {
                    denum.push_back(pow(copy(*it), copy(degree)));
                    ++it;
                }

            }
            else
            {
                auto it1 = fract.first->begin();
                num.push_back(pow(-(*it1), copy(degree)));
                ++it1;
                while (it1 != fract.first->end())
                {
                    num.push_back(pow(*it1, degree));
                    ++it1;
                }
                for (auto &it2 : *fract.second)
                    denum.push_back(pow(it2, degree));
            }
            auto fr =  abs_ex(new Fractal(&num, &denum));
            fr->simplify();
            return fr;
        }
        if (!num_coe.isOne())
            num.push_back(abs_ex(new Degree(makeAbstractExpression(NUMBER, &num_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        for (auto &it : *fract.first)
        {
            num.push_back(abs_ex(new Degree(makeAbstractExpression(it->getId(), it.get()), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        }
        if (!denom_coe.isOne())
            denum.push_back(abs_ex(new Degree(makeAbstractExpression(NUMBER, &denom_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        for (auto &it : *fract.second)
        {
            denum.push_back(abs_ex(new Degree(makeAbstractExpression(it->getId(), it.get()), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        }
        auto fr =  abs_ex(new Fractal(&num, &denum));
        fr->simplify();
        return fr;
    }
    if (this->degree->getPositionRelativelyZero() < 0)
    {
        return one/takeDegreeOf(std::move(argument), -degree);
    }

    if (*this->argument == *getEuler() && this->degree->getId() == FRACTAL && static_cast<Fractal*>(this->degree.get())->tryToFindLogarithmInNumerator() != nullptr)
    {
        auto log = static_cast<Fractal*>(this->degree.get())->tryToFindLogarithmInNumerator();
        assert(log != nullptr);
        return pow(static_cast<Logarithm*>(log.get())->getArgumentMoved(), degree/log);
    }
    if (this->argument->getSetOfVariables().empty())
    {
        if (this->degree->getId() == FRACTAL && static_cast<Fractal*>(degree.get())->hasIntegratingConstant())
        {
            abs_ex constant = static_cast<Fractal*>(degree.get())->takeAwayConstantMultiplier();
            return pow(pow(argument, constant), degree);
        }
        if (this->degree->getId() == POLYNOMIAL && static_cast<Polynomial*>(degree.get())->hasIntegratingConstant())
        {
            abs_ex constant = static_cast<Polynomial*>(degree.get())->takeAwayIntegragingConstant();
            return pow(argument, constant) * pow(argument, degree);
        }
    }
    if (*this->argument == *getEuler() && this->degree->getId() == FRACTAL)
    {
        abs_ex cop = pow(argument, static_cast<Fractal*>(this->degree.get())->turnIntoPolynomWithOpeningParentheses(true));
      //  qDebug() << cop->makeStringOfExpression();
        return cop;
    }
    if (*this->argument == *getEuler() && this->degree->getId() == POLYNOMIAL)
    {
        auto monoms = static_cast<Polynomial*>(this->degree.get())->getMonomialsPointers();
        abs_ex res = copy(one);
        abs_ex degree_without_logs = copy(zero);
        for (auto &it : monoms)
        {
            if (it->tryToFindLogarithmInNumerator() == nullptr)
                degree_without_logs = std::move(degree_without_logs) + copy(it);
            else
                res = std::move(res) * pow(this->argument, copy(it));
        }
        return res * pow(this->argument, degree_without_logs);
    }
   /* if (this->degree->getId() == FRACTAL && !static_cast<Fractal*>(this->degree.get())->getCoefficient().isOne() &&
            this->argument->getId() == NUMBER)
    {
        return pow(pow(argument, static_cast<Fractal*>(this->degree.get())->getCoefficient()),
                   degree/toAbsEx(static_cast<Fractal*>(this->degree.get())->getCoefficient()));
    }*/

    if (this->isOnlyVarsIntegratingConstants())
    {
        return integratingConstantExpr(this->getRange());
    }

    return copy( this->argument.get());

}
AbstractExpression * Degree::getArgumentOfDegree(AbstractExpression *expr)
{
    if (expr->getId() == DEGREE)
        return static_cast<const Degree*>(expr)->argument.get();
    else
        return expr;

}
abs_ex Degree::getDegreeOfExpression(AbstractExpression *expr)
{
    if (expr->getId() == DEGREE)
        return makeAbstractExpression(static_cast<Degree*>(expr)->degree->getId(), static_cast<Degree*>(expr)->degree.get());
    else
        return abs_ex(new Number(1));
}
bool canReduceWithDowngradingDegree(AbstractExpression * left, AbstractExpression * right)
{
    return *Degree::getArgumentOfDegree(left) == *Degree::getArgumentOfDegree(right);
}
bool Degree::isZeroDegree()
{
    return this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isZero();
}
bool Degree::canGetListOfArguments()
{
    return this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isInteger();
}
std::pair<std::unique_ptr<fractal_argument>, bool> Degree::getListOfArguments()
{
    assert(this->canGetListOfArguments());
    fractal_argument * list = new fractal_argument;
    for (int i = 0; i < abs(static_cast<Number*>(this->degree.get())->getNumerator()); ++i)
    {
        list->push_back(makeAbstractExpression(argument->getId(), argument.get()));
    }
    return std::pair<std::unique_ptr<fractal_argument>, bool>(std::unique_ptr<fractal_argument>(list), static_cast<Number*>(this->degree.get())->compareWith(0) < 0);
}
void Degree::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->argument = this->argument->downcast();
    this->degree->simplify();
    this->degree = this->degree->downcast();

    if (this->argument->getId() == DEGREE)
    {

        //ADD MODUL
        bool is_argument_square = false;
        if (static_cast<Degree*>(this->argument.get())->degree->getId() == NUMBER)
            is_argument_square = (static_cast<Number*>(static_cast<Degree*>(this->argument.get())->degree.get()))->getNumerator() % 2 == 0;
        this->degree = *static_cast<Degree*>(static_cast<Degree*>(this->argument.get())->degree.get()) * *this->degree;
        //what I want to do is sqrt((-2)^2) = 2, like this
        if (static_cast<Degree*>(this->argument.get())->degree->getId() == NUMBER &&
                static_cast<Degree*>(this->argument.get())->argument->getId() == NUMBER &&
                this->degree->getId() == NUMBER)
        {
            Number num = *static_cast<Number*>(static_cast<Degree*>(this->argument.get())->degree.get());
            if ((num.getDenominator() / 2) * 2 != num.getDenominator() && (num.getNumerator() / 2) * 2 == num.getNumerator() &&
                    static_cast<Number*>(static_cast<Degree*>(this->argument.get())->argument.get())->compareWith(0) < 0)
                *static_cast<Number*>(static_cast<Degree*>(this->argument.get())->argument.get()) = *static_cast<Number*>(static_cast<Degree*>(this->argument.get())->argument.get()) * -1;
        }
        this->argument = std::move(static_cast<Degree*>(this->argument.get())->argument);
        if (is_argument_square && (this->degree->getId() != NUMBER || static_cast<Number*>(this->degree.get())->getNumerator() % 2))
            this->argument = abs_ex(new AbsoluteValue(std::move(argument)));
    }
    if ((this->degree->getId() == NUMBER || this->degree->getId() == FRACTAL) && this->argument->getId() == NUMBER)
    {
        bool is_fractal = this->degree->getId() == FRACTAL;
        Number argument = *static_cast<Number*>(this->argument.get());
        Number degree = (is_fractal ? static_cast<Fractal*>(this->degree.get())->getCoefficient() : *static_cast<Number*>(this->degree.get()));
        if (degree.compareWith(0) < 0)
        {
            argument = Number(1) / argument;
            degree = degree * -1;
        }
        long long int arg_num = argument.getNumerator();
        long long int arg_denum = argument.getDenominator();
        long long int deg_denum = degree.getDenominator();
        if ((deg_denum / 2) * 2 == deg_denum && (arg_num < 0 || arg_denum < 0))
            throw Exception();
        bool succesfully = false;
        //в случае, к примеру, таком 2^3c, deg_denum = 1 и succesfully будет true, так что оно в итоге приведется к 8^c и вернется обратно в
        //takeArgumentsMultipliersIntoDegree(), но это просто лишние действия
            if (deg_denum != 1 || this->degree->getId() != FRACTAL)
                arg_num = root(arg_num, deg_denum, succesfully);
            if (succesfully)
                arg_denum = root(arg_denum, deg_denum, succesfully);
            if (succesfully)
                degree = degree * degree.getDenominator();
            if (succesfully)
                arg_num = power(arg_num, degree.getNumerator(), succesfully);
            if (succesfully)
                arg_denum = power(arg_denum, degree.getNumerator(), succesfully);
             degree = 1;
             //(-2)^2c = 2^2c
             if (degree.getNumerator() % 2 == 0 && arg_num < 0)
                 arg_num *= -1;
        if (succesfully)
        {

            if (is_fractal)
            {
                auto args = static_cast<Fractal*>(this->degree.get())->getFractal();
                this->degree = abs_ex(new Fractal(args.first, args.second, degree));
                this->degree = this->degree->downcast();
            }
            else
                this->degree = abs_ex(new Number(degree));
            this->argument = abs_ex(new Number(arg_num, arg_denum));
        }
    }
    if (this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isInteger() &&
            (this->degree->getId() != NUMBER || (this->degree->getId() == NUMBER && !static_cast<Number*>(this->degree.get())->isOne() &&
                                                 !static_cast<Number*>(this->degree.get())->isZero())))
    {
        this->takeArgumentsMultipliersIntoDegree();
    }
    if (this->argument->getId() == POLYNOMIAL)
    {
        bool has_vars = !static_cast<Polynomial*>(this->argument.get())->getSetOfVariables().empty();
        if (has_vars)
            this->reducePolynomialArgument();
        if (this->argument->getId() == POLYNOMIAL)
            this->transformPolynomialDegree(has_vars);
    }
    if (this->argument->getId() == ABSOLUTE_VALUE && this->degree->getId() == NUMBER)
    {
        if (static_cast<Number*>(this->degree.get())->getNumerator() % 2 == 0)
            this->argument = static_cast<AbsoluteValue*>(this->argument.get())->open();
    }
    if (this->argument->getId() == ABSOLUTE_VALUE && this->degree->getId() == FRACTAL)
    {
        if (static_cast<Fractal*>(this->degree.get())->getCoefficient().getNumerator() % 2 == 0)
            this->argument = static_cast<AbsoluteValue*>(this->argument.get())->open();
    }
    //assert(false);
    this->simplified = true;
}
void Degree::reducePolynomialArgument()
{
    assert(this->argument->getId() == POLYNOMIAL);
    Number red_coe = static_cast<Polynomial*>(this->argument.get())->reduce();
    if (red_coe.compareWith(1) != 0)
    {
        //преобразование выражения вида 3a + 3b в 3(a+b). затем оно разделится downcasting-ом на 2 разные степени
        this->argument = *this->argument * red_coe;
    }
}
void Degree::transformPolynomialDegree(bool has_vars)
{
    //in simplified()
   // this->reducePolynomialArgument();
    //if (this->argument->getId() != POLYNOMIAL)
    //    return;
    if (this->argument->getId() == POLYNOMIAL && has_vars)
    {
        bool is_root = (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->getDenominator() > 1)
                || (this->degree->getId() == FRACTAL && static_cast<Fractal*>(this->degree.get())->getCoefficient().getDenominator() > 1);
        if (!is_root)
            return;
        auto res = static_cast<Polynomial*>(this->argument.get())->tryToDistinguishFullDegree();
        if (res != nullptr)
        {
            this->argument = std::move(res);
            this->simplify();
        }
        return;
    }
    //return;







    if (this->degree->getId() == NUMBER || this->degree->getId() == FRACTAL)
    {
        long long int denum = (this->degree->getId() == NUMBER ? static_cast<Number*>(this->degree.get())->getDenominator() : static_cast<Fractal*>(this->degree.get())->getCoefficient().getDenominator());
        auto rt = static_cast<Polynomial*>(this->argument.get())->tryToTakeRoot(has_vars ? denum : 2);
        if (rt == nullptr && has_vars)
        {
            auto degrs_of_root = v_factorize(denum, false);
            if (degrs_of_root.size() > 1)
            {
                for (auto &it : degrs_of_root)
                {
                    auto root = static_cast<Polynomial*>(this->argument.get())->tryToTakeRoot(it);
                    if (root != nullptr)
                    {
                        this->argument = std::move(root);
                        if (this->degree->getId() == NUMBER)
                            *static_cast<Number*>(this->degree.get()) *= it;
                        else
                            static_cast<Fractal*>(this->degree.get())->setCoefficinet(static_cast<Fractal*>(this->degree.get())->getCoefficient() * it);
                    }
                }
            }
        }
        else if (rt != nullptr)
        {
            this->argument = std::move(rt);
            if (this->degree->getId() == NUMBER)
                *static_cast<Number*>(this->degree.get()) *= denum;
            else
                static_cast<Fractal*>(this->degree.get())->setCoefficinet(static_cast<Fractal*>(this->degree.get())->getCoefficient().getNumerator());
        }
        if (!has_vars && this->argument->getId() == POLYNOMIAL)
            this->reducePolynomialArgument();
        /*long long int num = (this->degree->getId() == NUMBER ? static_cast<Number*>(this->degree.get())->getNumerator() : static_cast<Fractal*>(this->degree.get())->getCoefficient().getNumerator());
        if (num > 1 && num < 20)
            try {
            this->argument = static_cast<Polynomial*>(this->argument.get())->toDegree(num);
            if (this->degree->getId() == NUMBER)
                *static_cast<Number*>(this->degree.get()) /= num;
            else
                static_cast<Fractal*>(this->degree.get())->setCoefficinet(Number(1) / static_cast<Fractal*>(this->degree.get())->getCoefficient().getDenominator());
        } catch (const Exception &) {}*/
    }
}
bool Degree::isDegreeOfNumberThatLessThanOne()
{
    return this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->getNumerator() == 1;
}
std::set<int> Degree::getSetOfPolyVariables() const
{
    return this->argument->getSetOfPolyVariables();
}
std::set<int> Degree::getSetOfVariables() const
{
    std::set<int> result = this->argument->getSetOfVariables();
    std::set<int> deg_set = this->degree->getSetOfVariables();
    result.insert(deg_set.begin(), deg_set.end());
    return result;
}

std::set<QString> Degree::getSetOfFunctions() const
{
    return this->argument->getSetOfFunctions();
}
Number Degree::getMaxDegreeOfVariable(int id)
{
    Number deg = this->argument->getMaxDegreeOfVariable(id);
    if (!deg.isCorrect())
        return deg;

    if (this->degree->getId() != NUMBER)
        return Number::makeErrorNumber();
    return deg * (*static_cast<Number*>(this->degree.get()));
}
//мне кажется, я это не использую, ибо формулы сокращенного умножения мне было реализовывать лень
bool Degree::canTurnIntoPolynomial()
{
    return this->argument->getId() == POLYNOMIAL && this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isInteger() &&
            static_cast<Number*>(this->degree.get())->compareWith(1) > 0 && static_cast<Number*>(this->degree.get())->compareWith(61) < 0;
}


void Degree::_qDebugOut()
{
    qDebug() << "DEGREE EXPRESSION: ";
    qDebug() << "Degree:";
    this->degree->_qDebugOut();
    qDebug() << "End of degree;";
    qDebug() << "Argument:";
    this->argument->_qDebugOut();
    qDebug() << "End of argument;";
    qDebug() << "END OF DEGREE;";
}
void Degree::takeArgumentsMultipliersIntoDegree()
{
    //in simplified()
    //it is like 8^c = 2^3c
    //it must be downcasted
    if (this->argument.get()->getId() == NUMBER)
    {
        assert(static_cast<Number*>(this->argument.get())->isInteger());
        Number *arg = static_cast<Number*>(this->argument.get());
        if (arg->isZero() || arg->isOne())
            return;

        bool is_negative = arg->getNumerator() < 0;
        auto num_mults = factorize(abs(arg->getNumerator()));


        auto gcd_of_mults = [](std::map<long long int, int> & mults)->int {
            if (mults.size() == 1)
                return mults.begin()->second;
            int gcf = gcd(mults.begin()->second, (++mults.begin())->second);
            for (auto it = ++++mults.begin(); it != mults.end(); ++it)
                gcf = gcd(gcf, it->second);
            return gcf;
        };
        auto make_new_num = [](std::map<long long int, int> & mults, int gcf) {
            long long int num = 1;
            for (auto &it : mults)
            {
                bool s;
                num *= power(it.first, it.second / gcf, s);
            }
            return num;
        };
        int gcf = gcd_of_mults(num_mults);
        while (is_negative && gcf % 2 == 0)
            gcf /= 2;
        abs_ex degr_multiplier = abs_ex(new Number(gcf));
        this->degree = this->degree * degr_multiplier;
        *arg = make_new_num(num_mults, gcf) * (is_negative ? -1 : 1);

    }
}
Number Degree::reduce()
{
    //it must be downcasted
    //Это значит, что argument гарантированно целый

    if (this->argument.get()->getId() == NUMBER && this->degree.get()->getId() == NUMBER)
    {
        NONCONST
        //это этап определения степеней. он отличается от takeArgumentsMultipliersIntoDegree() тем, что в случае 2*2*3*3*3, (и им подобным), та функция ничего не сделает
        Number *arg = static_cast<Number*>(this->argument.get());
        assert(arg->isInteger());
        bool is_arg_negative = arg->getNumerator() < 0;
        Number degr = *static_cast<Number*>(this->degree.get());
        if (degr.getDenominator() == 1)
            return 1;
        auto num_mults = factorize(abs(arg->getNumerator()));
        Number reducable = 1;
        Number new_arg = 1;
        for (auto &it : num_mults)
        {
            bool s;
            if (it.second > degr.getDenominator())
            {
                reducable = reducable * power(it.first, (it.second / degr.getDenominator()), s);
                it.second = it.second % degr.getDenominator();
            }

            new_arg = new_arg * power(it.first, it.second, s);
        }
        *arg = new_arg;

        //а здесь работа идет с самой степенью. если она больше единицы, то ее целые ее части можно убрать из-под корня. 2^(3/2) = 2 * 2^(1/2)
        if (degr.getNumerator() / degr.getDenominator() >= 1)
        {
            bool s;
            reducable = reducable * power(arg->getNumerator(), (degr.getNumerator() / degr.getDenominator()), s);
            Number new_deg = degr - (degr.getNumerator() / degr.getDenominator());
            if (this->degree->getId() == NUMBER)
                *static_cast<Number*>(this->degree.get()) = new_deg;
            else
                static_cast<Fractal*>(this->degree.get())->setCoefficinet(new_deg);
        }
        //отрицательный аргумент при четной степени невозможен, так что выноситься всегда будет число того же знака, что и под корнем
        if (is_arg_negative)
            reducable = reducable * -1;
        return reducable;
    }
    return 1;
}
QString Degree::makeStringOfExpression() const
{
    QString result;
    if (this->degree->getId() != NUMBER || static_cast<Number*>(this->degree.get())->isInteger())
    {
        if (this->argument->getId() != POLYNOMIAL)
            result = this->argument->makeStringOfExpression() + "^";
        else
            result = "(" + this->argument->makeStringOfExpression() + ")^";
        if (this->degree->getId() != POLYNOMIAL)
            result += this->degree->makeStringOfExpression();
        else
            result += "(" + this->degree->makeStringOfExpression() + ")";
    }
    else
    {
        QString denum, num;
        denum.setNum(static_cast<Number*>(this->degree.get())->getDenominator());
        num.setNum(static_cast<Number*>(this->degree.get())->getNumerator());
        if (denum != "2")
            result += "root(" + denum + "," + this->argument->makeStringOfExpression();
        else
            result += "sqrt(" + this->argument->makeStringOfExpression();

        result += ")";
        if (num != "1")
            result += "^" + num;
    }
    return result;
}

QString Degree::makeWolframString() const
{
    QString result;
    if (this->degree->getId() != NUMBER || static_cast<Number*>(this->degree.get())->isInteger())
    {
        if (this->argument->getId() != POLYNOMIAL)
            result = this->argument->makeWolframString() + "^";
        else
            result = "(" + this->argument->makeWolframString() + ")^";
        if (this->degree->getId() != POLYNOMIAL)
            result += this->degree->makeWolframString();
        else
            result += "(" + this->degree->makeWolframString() + ")";
    }
    else
    {
        if (*this->degree == *half)
            result = "Sqrt[" + this->argument->makeWolframString() + "]";
        else if (*this->degree == *(one/three))
            result = "CubeRoot[" + this->argument->makeWolframString() + "]";
        else
        {
            if (argument->getId() == POLYNOMIAL)
                result = "(" + argument->makeWolframString() + ")^(" + this->degree->makeWolframString() + ")";
            else
                result = argument->makeWolframString() + "^(" + degree->makeWolframString() + ")";
        }
    }
    return result;
}
bool Degree::isRootNumeric()
{
    return this->degree->getId() == NUMBER || (this->degree->getId() == FRACTAL && static_cast<Fractal*>(this->degree.get())->getFractal().second->empty());
}
long long int Degree::getRootValue()
{
    assert(this->isRootNumeric());
    if (this->degree->getId() == NUMBER)
        return static_cast<Number*>(this->degree.get())->getDenominator();
    if (this->degree->getId() == FRACTAL)
        return static_cast<Fractal*>(this->degree.get())->getCoefficient().getDenominator();

}
abs_ex takeDegreeOf(const abs_ex & argument, const abs_ex & degree)
{
    return Degree(argument, degree).downcast();
}
abs_ex takeDegreeOf(abs_ex && argument, abs_ex && degree)
{
    return Degree(argument, degree).downcast();
}
abs_ex takeDegreeOf(const abs_ex & argument, Number degree)
{
    return Degree(argument, degree).downcast();
}
abs_ex takeDegreeOf(abs_ex && argument, Number degree)
{
    return Degree(argument, degree).downcast();
}
abs_ex takeDegreeOf(Number argument, Number degree)
{
    return Degree(abs_ex(new Number(argument)),
                  abs_ex(new Number(degree))).downcast();
}
double Degree::getApproximateValue()
{
    assert(this->getSetOfVariables().empty());
    return pow(this->argument->getApproximateValue(), this->degree->getApproximateValue());
}
int Degree::getPositionRelativelyZeroIfHasVariables()
{
    if (this->degree->getId() != NUMBER)
        return 0;

    int degr = static_cast<Number*>(this->degree.get())->getNumerator();
    if (degr % 2 == 0)
        return 1;
    int root = static_cast<Number*>(this->degree.get())->getDenominator();
    if (root % 2 == 0)
        return 1;
    if (this->argument->getPositionRelativelyZero() > 0)
        return 1;
    return 0;
}
double Degree::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    return pow(this->argument->getApproximateValue(choosing_value_rule), this->degree->getApproximateValue(choosing_value_rule));
}

abs_ex Degree::changeSomePartOn(QString part, abs_ex &on_what)
{
  //  NONCONST
    if (this->argument->makeStringOfExpression() == part)
    {
        abs_ex cop = copy(on_what);
        this->argument.swap(cop);
        return cop;
    }
    return this->argument->changeSomePartOn(part, on_what);
}

abs_ex Degree::changeSomePartOnExpression(QString part, abs_ex &on_what)
{
    NONCONST
        if (this->argument->makeStringOfExpression() == part)
        {
            abs_ex cop = copy(on_what);
            this->argument.swap(cop);
            return cop;
        }
        return this->argument->changeSomePartOn(part, on_what);
}

abs_ex Degree::derivative(int var) const
{
    return takeDegreeOf(copy(this->argument), this->degree - one) * (this->argument * this->degree->derivative(var) * ln(this->argument) + this->degree * this->argument->derivative(var));
}

abs_ex Degree::antiderivative(int var) const
{
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    /*if (this->argument->getId() == var && !this->degree->hasVariable(var))
        return takeDegreeOf(copy(this->argument), this->degree + one) / (this->degree + one);
    if (!this->argument->hasVariable(var) && this->degree->getId() == var)
        return takeDegreeOf(this->argument, this->degree) / ln(this->argument);*/
    auto ln_f = checkIfItsLinearFunction(this->argument, var);
    if (ln_f.first != nullptr && !this->degree->hasVariable(var))
    {
        return one / ln_f.first * takeDegreeOf(copy(this->argument), this->degree + one) / (this->degree + one);
    }
    ln_f = checkIfItsLinearFunction(this->degree, var);
    if (ln_f.first != nullptr && !this->argument->hasVariable(var))
        return one / ln_f.first * takeDegreeOf(this->argument, this->degree) / ln(this->argument);
    if (isSqrt(this->degree) && !this->argument->hasVariable(var))
    {
        auto ln_f = checkIfItsLinearFunction(Degree::getArgumentOfDegree(this->degree.get()), var);
        if (ln_f.first != nullptr)
        {
            auto a = std::move(ln_f.first);
            auto b = std::move(ln_f.second);
            return (two * copy(this) * (ln(argument) * degree - one))/a/pow(ln(argument), 2);
        }
    }

    if (this->argument->getId() == SINUS && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (arg / two - one/four * sin(two * arg));
    }
    if (this->argument->getId() == COSINUS && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (arg / two + one/four*sin(two * arg));
    }
    if (this->argument->getId() == TANGENT && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (this->argument - arg);
    }
    if (this->argument->getId() == COTANGENT && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (minus_one * arg - this->argument);
    }
    if (this->argument->getId() == LOGARITHM && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (arg * (pow(this->argument, 2) - two*this->argument + two));
    }

    if (this->argument->getId() == SINUS && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());

        return one / ln_f.first / numToAbs(12) * (cos(three*arg) - numToAbs(9)*cos(arg));
    }
    if (this->argument->getId() == COSINUS && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first / numToAbs(12) * (sin(three*arg) + numToAbs(9)*sin(arg));
    }
    if (this->argument->getId() == TANGENT && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (one / (two * takeDegreeOf(cos(arg), 2)) + ln(abs(cos(arg))));
    }
    if (this->argument->getId() == COTANGENT && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (minus_one / (two *takeDegreeOf(sin(arg), 2)) - ln(abs(sin(arg))));
    }
    if (this->argument->getId() == LOGARITHM && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfFunction(this->argument.get());
        return one / ln_f.first * (arg * (takeDegreeOf(this->argument, 3) - three*takeDegreeOf(this->argument, 2)
                                          + numToAbs(6)*this->argument - numToAbs(6)));
    }
    //должен быть больше нуля, иначе оно бы распалось в downcast-е
    if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isInteger())
    {
        if (this->argument->getId() == SINUS)
        {
            ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = getArgumentOfFunction(argument.get());
            //перед рекурсивным интегралом мы домножаем на ln_f.first, т. к. в нем уже как бы сразу da
            //и поэтому нельзя, чтобы там повторно выносилось 1/a(что будет делаться), вот и компенсируем домножением
            return one/ln_f.first *(-one/degree * pow(argument, degree - one) * cos(arg) +
                                    (degree - one)/degree * ln_f.first * pow(argument, degree - two)->antiderivative(var));
        }
        if (this->argument->getId() == COSINUS)
        {
            ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = getArgumentOfFunction(argument.get());
            //перед рекурсивным интегралом мы домножаем на ln_f.first, т. к. в нем уже как бы сразу da
            //и поэтому нельзя, чтобы там повторно выносилось 1/a(что будет делаться), вот и компенсируем домножением
            return one/ln_f.first *(one/degree * pow(argument, degree - one) * sin(arg) +
                                    (degree - one)/degree  * ln_f.first* pow(argument, degree - two)->antiderivative(var));
        }
        if (this->argument->getId() == TANGENT)
        {
            ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
            if (ln_f.first == nullptr)
                return nullptr;
            //перед рекурсивным интегралом мы домножаем на ln_f.first, т. к. в нем уже как бы сразу da
            //и поэтому нельзя, чтобы там повторно выносилось 1/a(что будет делаться), вот и компенсируем домножением
            return one/ln_f.first * (pow(argument, degree - one)/(degree - one) -  ln_f.first*pow(argument, degree - two)->antiderivative(var));
        }
        if (this->argument->getId() == COTANGENT)
        {
            ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
            if (ln_f.first == nullptr)
                return nullptr;
            //перед рекурсивным интегралом мы домножаем на ln_f.first, т. к. в нем уже как бы сразу da
            //и поэтому нельзя, чтобы там повторно выносилось 1/a(что будет делаться), вот и компенсируем домножением
            return one/ln_f.first * (-pow(argument, degree - one)/(degree - one) -  ln_f.first* pow(argument, degree - two)->antiderivative(var));
        }

    }
    if (*this->degree == *half)
    {

        auto qc_f = checkIfItsQuadraticFunction(argument, var);
        auto x = abs_ex(new Variable(getVariable(var)));

        if (*qc_f[1] == *zero)
        {
            int pos = qc_f[0]->getPositionRelativelyZero();
            if (pos > 0)
            {
                auto div = std::move(qc_f[0]);
                qc_f[0] = copy(one);
                qc_f[2] = qc_f[2]/div;
                int pos_a = qc_f[2]->getPositionRelativelyZero();
                if (pos_a > 0)
                    return sqrt(div) * (x/two * sqrt(argument) + qc_f[2]/two * ln(abs(x + sqrt(argument))));
                if (pos_a < 0)
                    return sqrt(div) * (x/two * sqrt(argument) - qc_f[2]/two * ln(abs(x + sqrt(argument))));
                return nullptr;
            }
            if (pos < 0)
            {
                auto div = -std::move(qc_f[0]);
                qc_f[0] = copy(minus_one);
                qc_f[2] = qc_f[2]/div;
                int pos_a = qc_f[2]->getPositionRelativelyZero();
                if (pos_a > 0)
                    return sqrt(div) * (x/two * sqrt(argument) + qc_f[2]/two * asin(x/sqrt(qc_f[2])));
                return nullptr;
            }
        }
    }
    return nullptr;
}

void Degree::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->argument->setSimplified(simpl);
    this->degree->setSimplified(simpl);
}

std::set<abs_ex > Degree::getTrigonometricalFunctions() const
{
    std::set<abs_ex> result = this->argument->getTrigonometricalFunctions();
    auto deg = this->degree->getTrigonometricalFunctions();
    for (auto &it : deg)
        result.insert(copy(it));
    return result;
}

long long Degree::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    if (argument->getId() == var)
    {
        if (degree->getId() == NUMBER)
            return static_cast<Number*>(degree.get())->getDenominator();
        return 0;
    }
    return lcm(argument->getLcmOfDenominatorsOfDegreesOfVariable(var),
               degree->getLcmOfDenominatorsOfDegreesOfVariable(var));

}

long long Degree::getGcdOfNumeratorsOfDegrees(int var) const
{
    if (argument->getId() == var)
    {
        if (degree->getId() == NUMBER)
            return static_cast<Number*>(degree.get())->getNumerator();
        return 0;
    }
    long long arg = argument->getGcdOfNumeratorsOfDegrees(var);
    if (arg == 0)
        return degree->getGcdOfNumeratorsOfDegrees(var);
    long long deg = degree->getGcdOfNumeratorsOfDegrees(var);
    if (deg == 0)
        return arg;
    return gcd(arg, deg);
}
FunctionRange rangeOfEvenHyperbola(FunctionRange & arg_range, int deg)
{
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (it.min() == nullptr && (isZero(it.max()) || it.canBeBiggerThanZero()))
            return FunctionRange(FunctionRangeSegment(zero, nullptr, false, false));
        if (it.min() == nullptr)
        {
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(zero, pow(it.max(), deg), false, it.isMaxIncluded()));
        }
        else if (it.canBeLowerThanZero() && (isZero(it.max()) || it.canBeBiggerThanZero()))
        {    if (isZero(it.max()))
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(pow(it.min(), deg), nullptr, it.isMinIncluded(), false));
            else
            {
                if (bigger(pow(it.min(), deg), pow(it.max(), deg)))
                    result.addSegmentWithoutNormilizing(FunctionRangeSegment(pow(it.max(), deg), nullptr,
                                                                             it.isMaxIncluded(), false));
                else
                    result.addSegmentWithoutNormilizing(FunctionRangeSegment(pow(it.min(), deg), nullptr, it.isMinIncluded(), false));
            }
        }
        else if (it.canBeLowerThanZero())
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(pow(it.min(), deg), pow(it.max(), deg),
                                                                     it.isMinIncluded(), it.isMaxIncluded()));
        else if (isZero(it.min()) && it.max() != nullptr)
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(pow(it.max(), deg), nullptr,
                                                                     it.isMaxIncluded(), false));
        else if (isZero(it.min()))
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(zero, nullptr, false, false));
        else if (it.max() != nullptr)
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(pow(it.max(), deg), pow(it.min(), deg),
                                                                     it.isMaxIncluded(), it.isMinIncluded()));
        else
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(zero, pow(it.min(), deg),
                                                                     false, it.isMinIncluded()));
    }
    result.normalize();
    return result;
}
FunctionRange getRangeOfOddHyperbola(FunctionRange & arg_range, int deg)
{
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (it.min() == nullptr && it.max() == nullptr)
        {
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(nullptr, zero, false, false));
            result.addSegmentWithoutNormilizing(FunctionRangeSegment(zero, nullptr, false, false));
            return result;
        }
        if (it.min() == nullptr)
        {
            if (isZero(it.max()))
                result.addSegmentWithoutNormilizing(FunctionRangeSegment(nullptr, zero, false, false));
            else if (it.canBeBiggerThanZero())
            {
                result.addSegmentWithoutNormilizing(nullptr, zero, false, false);
                result.addSegmentWithoutNormilizing( pow(it.max(), deg), nullptr, it.isMaxIncluded(), false);
            }
            else
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), zero, it.isMaxIncluded(), false);
        }
        else if (it.canBeLowerThanZero())
        {
            if (isZero(it.max()))
                result.addSegmentWithoutNormilizing(nullptr, pow(it.min(), deg),false, it.isMinIncluded());
            else if (it.max() == nullptr)
            {
                result.addSegmentWithoutNormilizing(nullptr, pow(it.min(), deg), false, it.isMinIncluded());
                result.addSegmentWithoutNormilizing(zero, nullptr, false, false);
            }
            else if (it.canBeBiggerThanZero())
            {
                result.addSegmentWithoutNormilizing(nullptr, pow(it.min(), deg), false, it.isMinIncluded());
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), nullptr, it.isMaxIncluded(), false);
            }
            else
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), pow(it.min(), deg), it.isMaxIncluded(), it.isMinIncluded());
        }
        else if (isZero(it.min()))
        {
            if (isZero(it.max()))
                continue;
            else if (it.max() == nullptr)
                result.addSegmentWithoutNormilizing(zero, nullptr, false, false);
            else
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), nullptr, it.isMaxIncluded(), false);
        }
        else
        {
            if (it.max() == nullptr)
                result.addSegmentWithoutNormilizing(zero, pow(it.min(), deg), false, it.isMinIncluded());
            else
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), pow(it.min(), deg),
                                                    it.isMaxIncluded(), it.isMinIncluded());
        }
    }
    result.normalize();
    return result;
}
FunctionRange getRangeOfOddParabola(FunctionRange& arg_range, int deg)
{
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        FunctionRangeSegment segm;
        if (it.min() == nullptr)
            segm.setMin(nullptr, false);
        else
            segm.setMin(pow(it.min(), deg), it.isMinIncluded());

        if (it.max() == nullptr)
            segm.setMax(nullptr, false);
        else
            segm.setMax(pow(it.max(), deg), it.isMaxIncluded());
        result.addSegmentWithoutNormilizing(segm);
    }
    //тут не нужно нормализовывать
    return result;
}
FunctionRange getRangeOfEvenParabola(FunctionRange& arg_range, int deg)
{
    FunctionRange result;
    for (auto &it : arg_range.getSegments())
    {
        if (it.max() == nullptr && it.min() == nullptr)
            return FunctionRange(FunctionRangeSegment(zero, nullptr, true, false));
        if (it.min() == nullptr)
        {
            if (isZero(it.max()))
                return FunctionRange(FunctionRangeSegment(zero, nullptr, it.isMaxIncluded(), false));
            else if (it.canBeBiggerThanZero())
                return FunctionRange(FunctionRangeSegment(zero, nullptr, true, false));
            else
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), nullptr, it.isMaxIncluded(), false);
        }
        else if (it.canBeLowerThanZero())
        {
            if (isZero(it.max()))
                result.addSegmentWithoutNormilizing(zero, pow(it.min(), deg), it.isMaxIncluded(), it.isMinIncluded());
            else if (it.canBeBiggerThanZero())
            {
                if (it.max() == nullptr)
                    return FunctionRange(FunctionRangeSegment(zero, nullptr, true, false));
                if (bigger(it.max(), it.min()))
                    result.addSegmentWithoutNormilizing(zero, pow(it.max(), deg), true, it.isMaxIncluded());
                else
                    result.addSegmentWithoutNormilizing(zero, pow(it.min(), deg), true, it.isMinIncluded());
            }
            else
                result.addSegmentWithoutNormilizing(pow(it.max(), deg), pow(it.min(), deg), it.isMaxIncluded(),
                                                    it.isMinIncluded());
        }
        else
        {
            if (it.max() == nullptr)
                result.addSegmentWithoutNormilizing(pow(it.min(), deg), nullptr, it.isMinIncluded(), false);
            else
                result.addSegmentWithoutNormilizing(pow(it.min(), deg), pow(it.max(), deg), it.isMinIncluded(), it.isMaxIncluded());
        }

    }
    result.normalize();
    return result;
}
FunctionRange getRangeOfExponentialFunctionOfLowerOneArg(const abs_ex & arg, FunctionRange deg_range)
{
    FunctionRange result;
    for (auto &it : deg_range.getSegments())
    {
        if (it.min() == nullptr && it.max() == nullptr)
            return FunctionRange(zero, nullptr, false, false);
        FunctionRangeSegment segm;
        if (it.min() == nullptr)
            segm.setMax(nullptr, false);
        else
            segm.setMax(pow(arg, it.min()), it.isMinIncluded());

        if (it.max() == nullptr)
            segm.setMin(zero, false);
        else
            segm.setMin(pow(arg, it.max()), it.isMaxIncluded());
    }
    result.normalize();
    return result;
}
FunctionRange getRangeOfExponentialFunctionOfBiggerOneArg(const abs_ex & arg, FunctionRange deg_range)
{
    FunctionRange result;
    for (auto &it : deg_range.getSegments())
    {
        FunctionRangeSegment segm;
        if (it.min() == nullptr)
            segm.setMin(zero, false);
        else
            segm.setMin(pow(arg, it.min()), it.isMinIncluded());

        if (it.max() == nullptr)
            segm.setMax(nullptr, false);
        else
            segm.setMax(pow(arg, it.max()), it.isMaxIncluded());
        result.addSegmentWithoutNormilizing(segm);

    }
    //опять же normalize не нужен
    return result;
}
FunctionRange Degree::getRange() const
{
    if (hasIntersections(this->argument->getSetOfVariables(), this->degree->getSetOfVariables()))
        return FunctionRange::getErrorRange();
    FunctionRange arg_range = this->argument->getRange();
    if (arg_range.isError())
        return arg_range;
    FunctionRange deg_range = this->degree->getRange();
    if (deg_range.isError())
        return deg_range;
    if (deg_range.isPoint() && arg_range.isPoint())
        return FunctionRange(FunctionRangeSegment(pow(arg_range.getPoint(), deg_range.getPoint()),pow(arg_range.getPoint(), deg_range.getPoint()),
                             true, true));
    FunctionRange result;
    if (deg_range.isPoint() && deg_range.getPoint()->getId() == NUMBER
            && static_cast<Number*>(deg_range.getPoint().get())->isInteger())
    {
        int deg = static_cast<Number*>(deg_range.getPoint().get())->getNumerator();
        if (deg == 0)
            return FunctionRange(FunctionRangeSegment(one, one, true, true));
        if (deg < 0 && (-deg) % 2 == 0)
        {
            return rangeOfEvenHyperbola(arg_range, deg);
        }
        else if (deg < 0)
            return getRangeOfOddHyperbola(arg_range, deg);
        else if (deg > 0 && deg % 2 == 0)
            return getRangeOfOddParabola(arg_range, deg);
        else
            return getRangeOfEvenParabola(arg_range, deg);
    }
    if (arg_range.isPoint())
    {
        if (isZero(arg_range.getPoint()))
            return FunctionRange(zero, zero, true, true);
        if (*arg_range.getPoint() == *one)
            return FunctionRange(one, one, true, true);
        if (lower(arg_range.getPoint(), zero))
            return FunctionRange::getErrorRange();
        if (lower(arg_range.getPoint(), one))
            return getRangeOfExponentialFunctionOfLowerOneArg(argument, deg_range);
        return getRangeOfExponentialFunctionOfBiggerOneArg(argument, deg_range);
    }
    auto segmBetweenZeroAndOne = [](const FunctionRangeSegment & arg, const FunctionRangeSegment & deg)->FunctionRangeSegment
    {
        FunctionRangeSegment segm;
        if (deg.min() == nullptr)
            segm.setMax(nullptr, false);
        else if (deg.canBeLowerThanZero())
            segm.setMax(pow(arg.min(), deg.min()), arg.isMinIncluded() && deg.isMinIncluded());
        else
            segm.setMax(pow(arg.max(), deg.min()), arg.isMaxIncluded() && deg.isMinIncluded());

        if (deg.max() == nullptr)
            segm.setMin(zero, false);
        else if (deg.canBeBiggerThanZero())
            segm.setMin(pow(arg.min(), deg.max()), arg.isMinIncluded() && deg.isMaxIncluded());
        else
            segm.setMin(pow(arg.max(), deg.max()), arg.isMaxIncluded() && deg.isMaxIncluded());
        return segm;
    };
    auto segmMoreThanOne = [](const FunctionRangeSegment & arg, const FunctionRangeSegment & deg)->FunctionRangeSegment
    {
        FunctionRangeSegment segm;
        if (deg.min() == nullptr)
            segm.setMin(zero, false);
        else if (deg.canBeLowerThanZero())
            segm.setMin(pow(arg.max(), deg.min()), arg.isMaxIncluded() && deg.isMinIncluded());
        else
            segm.setMin(pow(arg.min(), deg.min()), arg.isMinIncluded() && deg.isMinIncluded());

        if (deg.max() == nullptr)
            segm.setMax(nullptr, false);
        else if (deg.canBeBiggerThanZero())
            segm.setMax(pow(arg.max() , deg.max()), arg.isMaxIncluded() && deg.isMaxIncluded());
        else
            segm.setMax(pow(arg.min(), deg.max()), arg.isMinIncluded() && deg.isMaxIncluded());
        return segm;
    };
    for (auto &it : arg_range.getSegments())
    {
        auto min = copy(it.min());
        bool is_min_included = it.isMinIncluded();
        if (isZero(it.max()))
        {
            result.addSegmentWithoutNormilizing(zero, zero, true, true);
            continue;
        }
        if (it.max() != nullptr && lower(it.max(), zero))
            continue;

        if (min == nullptr || lower(min, zero))
        {
            min = copy(zero);
            is_min_included = true;
        }
        bool max_lower_one = lower(it.max(), one);
        bool min_lower_one = lower(it.min(), one);
        for (auto &it1 : deg_range.getSegments())
        {
            if (max_lower_one)
                result.addSegmentWithoutNormilizing(segmBetweenZeroAndOne(FunctionRangeSegment(min, it.max(),
                                                                                               is_min_included, it.isMaxIncluded()), it1));
            else if (min_lower_one)
            {
                result.addSegmentWithoutNormilizing(segmBetweenZeroAndOne(
                                                        FunctionRangeSegment(min, one, is_min_included, false), it1));
                result.addSegmentWithoutNormilizing(segmMoreThanOne(
                                                        FunctionRangeSegment(one, it.max(), false, it.isMaxIncluded()), it1));
                result.addSegmentWithoutNormilizing(one, one, true, true);
            }
            else
                result.addSegmentWithoutNormilizing(segmMoreThanOne(FunctionRangeSegment(min, it.max(), is_min_included, it.isMaxIncluded()),
                                                                    it1));
        }
    }
    result.normalize();
    return result;

}

bool Degree::hasDifferential() const
{
    return this->argument->hasDifferential() || this->degree->hasDifferential();
}

abs_ex takeDegreeOf(abs_ex &&argument, const abs_ex &degree)
{
    return takeDegreeOf(std::move(argument), copy(degree));
}

abs_ex sqrt(const abs_ex &arg)
{
    return takeDegreeOf(arg, half);
}

abs_ex sqrt(abs_ex &&arg)
{
    return takeDegreeOf(std::move(arg), half);
}

abs_ex sqr(const abs_ex &arg)
{
    return takeDegreeOf(arg, two);
}

abs_ex sqr(abs_ex &&arg)
{
    return takeDegreeOf(std::move(arg), two);
}

abs_ex pow(const abs_ex &arg, const abs_ex &deg)
{
    return takeDegreeOf(arg, deg);
}

abs_ex pow(abs_ex &&arg, const abs_ex &deg)
{
    return takeDegreeOf(std::move(arg), deg);
}

abs_ex pow(abs_ex &&arg, abs_ex &&deg)
{
    return takeDegreeOf(std::move(arg), std::move(deg));
}

abs_ex pow(const abs_ex &arg, Number deg)
{
    return takeDegreeOf(arg, deg);
}

abs_ex pow(abs_ex &&arg, Number deg)
{
    return takeDegreeOf(std::move(arg), deg);
}

abs_ex pow(Number arg, Number deg)
{
    return takeDegreeOf(arg, deg);
}

bool isSqrt(const abs_ex &expr)
{
    return expr->getId() == DEGREE && *Degree::getDegreeOfExpression(expr.get()) == *half;
}

abs_ex pow(const abs_ex &arg, abs_ex &&deg)
{
    return takeDegreeOf(arg, std::move(deg));
}
