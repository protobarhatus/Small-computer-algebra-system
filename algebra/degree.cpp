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
Degree::Degree(std::unique_ptr<AbstractExpression> && iargument, std::unique_ptr<AbstractExpression> && idegree)
{
    this->argument = std::move(iargument);
    this->degree = std::move(idegree);
    this->simplify();
}
Degree::Degree(const std::unique_ptr<AbstractExpression> & arg, const std::unique_ptr<AbstractExpression> & deg)
{
    this->argument = makeAbstractExpression(arg->getId(), arg.get());
    this->degree = makeAbstractExpression(deg->getId(), deg.get());
    this->simplify();
}
Degree::Degree(const std::unique_ptr<AbstractExpression> & arg, Number deg)
{
    this->argument = makeAbstractExpression(arg->getId(), arg.get());
    this->degree = std::unique_ptr<AbstractExpression>(new Number(deg));
    this->simplify();
}
Degree::Degree(std::unique_ptr<AbstractExpression> && arg, Number deg)
{
    this->argument = std::move(arg);
    this->degree = std::unique_ptr<AbstractExpression>(new Number(deg));
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
bool Degree::canDowncastTo(AlgebraExpression expr)
{
    if (expr == FRACTAL && this->argument->getId() == FRACTAL)
        return true;
    if (expr == FRACTAL && this->degree->getPositionRelativelyZero() < 0)
        return true;
    if (expr == FRACTAL && this->argument->getId() == NUMBER && !static_cast<Number*>(this->argument.get())->isInteger())
        return true;
    if (expr == FRACTAL && this->argument->getId() == NUMBER && factorize(abs(static_cast<Number*>(this->argument.get())->getNumerator())).size() != 1)
        return true;
    //there must be check on limit but i'll do it later
    //giving a set of expressions in parenthesses (like (a+b)^2 = (a+b)(a+b)) is in another function. operations with Numbers as argument must be realized in simplify()
    if (expr == NUMBER && this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isZero())
        return true;
    if (expr == NUMBER && this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isOne())
        return true;
    if (this->degree->getId() == NUMBER && static_cast<Number*>(this->degree.get())->isOne() && (expr == this->argument->getId() || (expr > 0 && this->argument->getId() > 0)))
        return true;
    if (*this->argument == *getEuler() && this->degree->getId() == LOGARITHM && ((expr > 0 && static_cast<Logarithm*>(this->degree.get())->getArgument()->getId() > 0) ||
                                                                                 expr == static_cast<Logarithm*>(this->degree.get())->getArgument()->getId()))
        return true;
    return false;
}
std::unique_ptr<AbstractExpression> Degree::downcastTo(AlgebraExpression expr)
{

    assert(canDowncastTo(expr));
    if (*this->argument == *getEuler() && this->degree->getId() == LOGARITHM && ((expr > 0 && static_cast<Logarithm*>(this->degree.get())->getArgument()->getId() > 0) ||
                                                                                 expr == static_cast<Logarithm*>(this->degree.get())->getArgument()->getId()))
        return static_cast<Logarithm*>(this->degree.get())->getArgumentMoved();
    if ((this->argument->getId() == NUMBER && static_cast<Number*>(this->argument.get())->isOne()) || static_cast<Number*>(this->degree.get())->isZero())
        return std::unique_ptr<AbstractExpression>(new Number(1));

    if (expr == FRACTAL && this->argument->getId() == NUMBER && !static_cast<Number*>(this->argument.get())->isInteger())
    {
        fractal_argument num, denum;
        if (static_cast<Number*>(this->argument.get())->getNumerator() != 1)
        {
            Number num_coe = static_cast<Number*>(this->argument.get())->getNumerator();
            num.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(NUMBER, &num_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
            num.begin()->get()->simplify();
        }
        Number denom_coe = static_cast<Number*>(this->argument.get())->getDenominator();
        denum.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(NUMBER, &denom_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        denum.begin()->get()->simplify();
        return std::unique_ptr<AbstractExpression>(new Fractal(&num, &denum));
    }    
    else if (expr == FRACTAL && this->argument->getId() == NUMBER)
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
                num.push_back(std::unique_ptr<AbstractExpression>(new Degree(std::unique_ptr<AbstractExpression>(new Number(power(it.first, it.second, s))),
                                                                             makeAbstractExpression(this->degree->getId(), this->degree.get()))));
            }
            return std::unique_ptr<AbstractExpression>(new Fractal(std::move(num), std::move(den), is_negative ? -1 : 1));
        }
    }
    else if (expr == FRACTAL && this->argument->getId() == FRACTAL)
    {
        auto fract = static_cast<Fractal*>(this->argument.get())->getFractal();

        //though this degree suppose to be destroyed after this function, I would use safe copy instead of moving in denumenator of fractal

        fractal_argument num, denum;
        Number num_coe = static_cast<Fractal*>(this->argument.get())->getCoefficient().getNumerator();
        Number denom_coe = Number( static_cast<Fractal*>(this->argument.get())->getCoefficient().getDenominator());
        if (!num_coe.isOne())
            num.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(NUMBER, &num_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        for (auto &it : *fract.first)
        {
            num.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(it->getId(), it.get()), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        }
        if (!denom_coe.isOne())
            denum.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(NUMBER, &denom_coe), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        for (auto &it : *fract.second)
        {
            denum.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(it->getId(), it.get()), makeAbstractExpression(this->degree->getId(), this->degree.get()))));
        }
        auto fr =  std::unique_ptr<AbstractExpression>(new Fractal(&num, &denum));
        fr->simplify();
        return fr;
    }
    if (expr == FRACTAL && this->degree->getPositionRelativelyZero() < 0)
    {
        return one/takeDegreeOf(std::move(argument), -degree);
    }
    return makeAbstractExpression(expr, this->argument.get());
}
AbstractExpression * Degree::getArgumentOfDegree(AbstractExpression *expr)
{
    if (expr->getId() == DEGREE)
        return static_cast<Degree*>(expr)->argument.get();
    else
        return expr;

}
std::unique_ptr<AbstractExpression> Degree::getDegreeOfExpression(AbstractExpression *expr)
{
    if (expr->getId() == DEGREE)
        return makeAbstractExpression(static_cast<Degree*>(expr)->degree->getId(), static_cast<Degree*>(expr)->degree.get());
    else
        return std::unique_ptr<AbstractExpression>(new Number(1));
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
            this->argument = std::unique_ptr<AbstractExpression>(new AbsoluteValue(std::move(argument)));
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
                this->degree = std::unique_ptr<AbstractExpression>(new Fractal(args.first, args.second, degree));
                this->degree = this->degree->downcast();
            }
            else
                this->degree = std::unique_ptr<AbstractExpression>(new Number(degree));
            this->argument = std::unique_ptr<AbstractExpression>(new Number(arg_num, arg_denum));
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
        std::unique_ptr<AbstractExpression> degr_multiplier = std::unique_ptr<AbstractExpression>(new Number(gcf));
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
        result = this->argument->makeStringOfExpression() + "^";
        result += this->degree->makeStringOfExpression();
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
        if (num != "1")
            result += "^" + num;
        result += ")";
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
std::unique_ptr<AbstractExpression> takeDegreeOf(const std::unique_ptr<AbstractExpression> & argument, const std::unique_ptr<AbstractExpression> & degree)
{
    return Degree(argument, degree).downcast();
}
std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> && argument, std::unique_ptr<AbstractExpression> && degree)
{
    return Degree(argument, degree).downcast();
}
std::unique_ptr<AbstractExpression> takeDegreeOf(const std::unique_ptr<AbstractExpression> & argument, Number degree)
{
    return Degree(argument, degree).downcast();
}
std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> && argument, Number degree)
{
    return Degree(argument, degree).downcast();
}
std::unique_ptr<AbstractExpression> takeDegreeOf(Number argument, Number degree)
{
    return Degree(std::unique_ptr<AbstractExpression>(new Number(argument)),
                  std::unique_ptr<AbstractExpression>(new Number(degree))).downcast();
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
    return this->argument->getPositionRelativelyZero();
}
double Degree::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    return pow(this->argument->getApproximateValue(choosing_value_rule), this->degree->getApproximateValue(choosing_value_rule));
}

std::unique_ptr<AbstractExpression> Degree::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
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

std::unique_ptr<AbstractExpression> Degree::changeSomePartOnExpression(QString part, std::unique_ptr<AbstractExpression> &on_what)
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

std::unique_ptr<AbstractExpression> Degree::derivative(int var) const
{
    return takeDegreeOf(copy(this->argument), this->degree - one) * (this->argument * this->degree->derivative(var) * ln(this->argument) + this->degree * this->argument->derivative(var));
}

std::unique_ptr<AbstractExpression> Degree::antiderivative(int var) const
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

    if (this->argument->getId() == SINUS && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (arg / two - one/four * sin(two * arg));
    }
    if (this->argument->getId() == COSINUS && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (arg / two + one/four*sin(two * arg));
    }
    if (this->argument->getId() == TANGENT && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (this->argument - arg);
    }
    if (this->argument->getId() == COTANGENT && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (minus_one * arg - this->argument);
    }
    if (this->argument->getId() == LOGARITHM && *this->degree == *two)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (arg * (pow(this->argument, 2) - two*this->argument + two));
    }

    if (this->argument->getId() == SINUS && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());

        return one / ln_f.first / numToAbs(12) * (cos(three*arg) - numToAbs(9)*cos(arg));
    }
    if (this->argument->getId() == COSINUS && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first / numToAbs(12) * (sin(three*arg) + numToAbs(9)*sin(arg));
    }
    if (this->argument->getId() == TANGENT && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (one / (two * takeDegreeOf(cos(arg), 2)) + ln(abs(cos(arg))));
    }
    if (this->argument->getId() == COTANGENT && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
        return one / ln_f.first * (minus_one / (two *takeDegreeOf(sin(arg), 2)) - ln(abs(sin(arg))));
    }
    if (this->argument->getId() == LOGARITHM && *this->degree == *three)
    {
        ln_f = checkIfItsFunctionOfLinearArgument(this->argument, var);
        if (ln_f.first == nullptr)
            return nullptr;
        auto arg = getArgumentOfTrigonometricalFunction(this->argument.get());
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
            auto arg = getArgumentOfTrigonometricalFunction(argument.get());
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
            auto arg = getArgumentOfTrigonometricalFunction(argument.get());
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

std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> &&argument, const std::unique_ptr<AbstractExpression> &degree)
{
    return takeDegreeOf(std::move(argument), copy(degree));
}

std::unique_ptr<AbstractExpression> sqrt(const std::unique_ptr<AbstractExpression> &arg)
{
    return takeDegreeOf(arg, half);
}

std::unique_ptr<AbstractExpression> sqrt(std::unique_ptr<AbstractExpression> &&arg)
{
    return takeDegreeOf(std::move(arg), half);
}

std::unique_ptr<AbstractExpression> sqr(const std::unique_ptr<AbstractExpression> &arg)
{
    return takeDegreeOf(arg, two);
}

std::unique_ptr<AbstractExpression> sqr(std::unique_ptr<AbstractExpression> &&arg)
{
    return takeDegreeOf(std::move(arg), two);
}

std::unique_ptr<AbstractExpression> pow(const std::unique_ptr<AbstractExpression> &arg, const std::unique_ptr<AbstractExpression> &deg)
{
    return takeDegreeOf(arg, deg);
}

std::unique_ptr<AbstractExpression> pow(std::unique_ptr<AbstractExpression> &&arg, const std::unique_ptr<AbstractExpression> &deg)
{
    return takeDegreeOf(std::move(arg), deg);
}

std::unique_ptr<AbstractExpression> pow(std::unique_ptr<AbstractExpression> &&arg, std::unique_ptr<AbstractExpression> &&deg)
{
    return takeDegreeOf(std::move(arg), std::move(deg));
}

std::unique_ptr<AbstractExpression> pow(const std::unique_ptr<AbstractExpression> &arg, Number deg)
{
    return takeDegreeOf(arg, deg);
}

std::unique_ptr<AbstractExpression> pow(std::unique_ptr<AbstractExpression> &&arg, Number deg)
{
    return takeDegreeOf(std::move(arg), deg);
}

std::unique_ptr<AbstractExpression> pow(Number arg, Number deg)
{
    return takeDegreeOf(arg, deg);
}
