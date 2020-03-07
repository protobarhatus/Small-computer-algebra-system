#include "fractal.h"
#include "assert.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
#include "algorithm"
#include "degree.h"
#include "polynomial.h"
#include "exception.h"
#include <cmath>
Fractal::Fractal() : coefficient(1)
{

}
AlgebraExpression Fractal::getId() const
{
    return FRACTAL;
}
Fractal::Fractal(AbstractExpression * num, AbstractExpression * denum, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(makeAbstractExpression(num->getId(), num));
    this->pushBackToDenominator(makeAbstractExpression(denum->getId(), denum));
    this->simplify();
}
Fractal::Fractal(fractal_argument * num, fractal_argument * denum, Number coe) : coefficient(coe)
{
    for (auto &it : *num)
    {
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    for (auto &it : *denum)
    {
        this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    }
    this->simplify();
}
Fractal::Fractal(fractal_argument && num, fractal_argument && denum, Number coe) : coefficient(coe)
{
    for (auto &it : num)
        this->pushBackToNumerator(std::move(it));
    for (auto &it : denum)
        this->pushBackToDenominator(std::move(it));
    this->simplify();
}
Fractal::Fractal(fractal_argument * num, Number coe) : coefficient(coe)
{
    for (auto &it : *num)
    {
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    this->simplify();
}
Fractal::Fractal(AbstractExpression * num, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(makeAbstractExpression(num->getId(), num));
    this->simplify();
}
Fractal::Fractal(Number coe) : coefficient(coe)
{

}
bool Fractal::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == FRACTAL);
    Fractal * frac = static_cast<Fractal*>(const_cast<AbstractExpression*>(&right));
    if (less(&this->coefficient, &frac->coefficient))
        return true;
    if (less(&frac->coefficient, &this->coefficient))
        return false;
    if (this->numerator.size() < frac->numerator.size())
        return true;
    if (this->numerator.size() > frac->numerator.size())
        return false;
    auto it1 = frac->numerator.begin();
    for (auto &it : this->numerator)
    {
        if (less(it.get(), it1->get()))
            return true;
        if (less(it1->get(), it.get()))
            return false;
        ++it1;
    }
    if (this->denominator.size() < frac->denominator.size())
        return true;
    if (this->denominator.size() > frac->denominator.size())
        return false;
    auto it2 = frac->denominator.begin();
    for (auto &it : this->denominator)
    {
        if (less(it.get(), it2->get()))
            return true;
        if (less(it2->get(), it.get()))
            return false;
        ++it2;
    }
    return false;
}
bool Fractal::operator==(AbstractExpression &right)
{
    if (right.getId() != FRACTAL)
        return false;
    Fractal * frac = static_cast<Fractal*>(&right);
    if (this->coefficient != frac->coefficient)
        return false;
    if (this->numerator.size() != frac->numerator.size() || this->denominator.size() != frac->denominator.size())
        return false;
    auto it1 = frac->numerator.begin();
    for (auto &it : this->numerator)
    {
        if (*it1->get() != *it.get())
            return false;
        ++it1;
    }
    auto it2 = frac->denominator.begin();
    for (auto &it : this->denominator)
    {
        if (*it.get() != *it2->get())
            return false;
        ++it2;
    }
    return true;
}

bool Fractal::isLetterPartEquallTo(Fractal *frac)
{
    if (this->numerator.size() != frac->numerator.size() ||
            this->denominator.size() != frac->denominator.size())
        return false;
    auto it1 = frac->numerator.begin();
    for (auto &it : this->numerator)
    {
        if (*it1->get() != *it.get())
            return false;
        ++it1;
    }
    auto it2 = frac->denominator.begin();
    for (auto &it : this->denominator)
    {
        if (*it.get() != *it2->get())
            return false;
        ++it2;
    }
    return true;
}
Fractal::Fractal(const Fractal & right) : coefficient(right.coefficient)
{
    for (auto &it : right.numerator)
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right.denominator)
        this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    this->simplified = right.simplified;
}
Fractal::Fractal(Fractal && expr) : coefficient(expr.coefficient)
{
    for (auto &it : expr.numerator)
        this->pushBackToNumerator(std::move(it));
    for (auto &it : expr.denominator)
        this->pushBackToDenominator(std::move(it));
    this->simplified = expr.simplified;
}
std::pair<fractal_argument *, fractal_argument *> Fractal::getFractal() const
{
    *const_cast<bool*>(&this->simplified) = false;
    return std::pair<fractal_argument *, fractal_argument *>(const_cast<fractal_argument*>(&this->numerator), const_cast<fractal_argument*>(&this->denominator));
}
Number Fractal::getCoefficient()
{
    return this->coefficient;
}
bool Fractal::canDowncastTo(AlgebraExpression expr)
{
    return (this->denominator.empty() && this->coefficient.getDenominator() == 1 && this->coefficient.getNumerator() == 1 && this->numerator.size() == 1 &&
            (this->numerator.begin()->get()->getId() == expr || (this->numerator.begin()->get()->getId() > 0 && expr > 0))) ||
            (expr == NUMBER && this->numerator.empty() && this->denominator.empty()) ||
            (expr == NUMBER && this->coefficient.getNumerator() == 0);
}
std::unique_ptr<AbstractExpression> Fractal::downcastTo(AlgebraExpression expr)
{
    assert(this->canDowncastTo(expr));
    if (expr == NUMBER)
    {
        return makeAbstractExpression(NUMBER, &this->coefficient);
    }
    return makeAbstractExpression(expr, this->numerator.begin()->get());
}
bool Fractal::isZero()
{
    return this->coefficient.getNumerator() == 0;
}
void Fractal::setCoefficinet(Number num)
{
    NONCONST
    this->coefficient = num;
}
fractal_argument operator*(fractal_argument & fmult, fractal_argument & smult)
{
    fractal_argument result;
    for (auto &it : fmult)
        result.push_back(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : smult)
        result.push_back(makeAbstractExpression(it->getId(), it.get()));
    //std::sort(result.begin(), result.end(), &AbstractExpression::less);
    Fractal frac(&result);
    result = std::move(*frac.getFractal().first);
    return result;
}

std::unique_ptr<Fractal> Fractal::makeZeroFractal()
{
    return std::make_unique<Fractal>((Number(0)));
}
bool operator==(fractal_argument & left, fractal_argument & right)
{
    if (left.size() != right.size())
        return false;
    fractal_argument::iterator it1 = right.begin();
    for (auto &it : left)
    {
        if (*it1->get() != *it.get())
            return false;
    }
    return true;
}
bool Fractal::isDenominatorEqualToOne()
{
    return this->denominator.empty() && this->coefficient.getDenominator() == 1;
}
std::unique_ptr<Fractal> Fractal::operator+(const std::unique_ptr<Fractal> & sec_sum)
{
    if (this->denominator == sec_sum->denominator && this->coefficient == sec_sum->coefficient)
    {
        std::unique_ptr<AbstractExpression> newnum_ptr = *static_cast<AbstractExpression*>(new Fractal(&this->numerator, this->coefficient.getNumerator())) +
                *static_cast<AbstractExpression*>(new Fractal(&sec_sum->numerator, sec_sum->coefficient.getNumerator()));
        fractal_argument newnum;
        newnum.push_back(std::move(newnum_ptr));
        std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&newnum, &this->denominator, Number(1, this->coefficient.getDenominator())/*, this->coefficient * sec_sum->coefficient*/));
        result->simplify();
        return result;
    }
    /*
    fractal_argument left_sum = this->numerator * (sec_sum->denominator / this->denominator);
    fractal_argument right_sum = sec_sum->numerator * this->denominator;
    std::unique_ptr<AbstractExpression> newnum_ptr = *static_cast<AbstractExpression*>(new Fractal(&left_sum)) +
            *static_cast<AbstractExpression*>(new Fractal(&right_sum));
    fractal_argument newnum;
    newnum.push_back(std::move(newnum_ptr));
    fractal_argument newdenum = this->denominator * sec_sum->denominator;
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&newnum, &newdenum, this->coefficient * sec_sum->coefficient));
    result->simplify();*/

    std::unique_ptr<AbstractExpression> this_denum = std::unique_ptr<AbstractExpression>(new Fractal(&this->denominator));
    std::unique_ptr<AbstractExpression> this_num_denum = std::unique_ptr<AbstractExpression>(new Number(this->coefficient.getDenominator()));
    this_denum = this_denum * this_num_denum;
    std::unique_ptr<AbstractExpression> sec_sum_denum = std:: unique_ptr<AbstractExpression>(new Fractal(&sec_sum->denominator));
    std::unique_ptr<AbstractExpression> sec_sum_num_denum = std::unique_ptr<AbstractExpression>(new Number(sec_sum->coefficient.getDenominator()));
    sec_sum_denum = sec_sum_denum * sec_sum_num_denum;

    std::unique_ptr<AbstractExpression> this_num = std::unique_ptr<AbstractExpression>(new Fractal(&this->numerator));
    std::unique_ptr<AbstractExpression> this_num_num = std::unique_ptr<AbstractExpression>(new Number(this->coefficient.getNumerator()));
    this_num = this_num * this_num_num;
    std::unique_ptr<AbstractExpression> sec_sum_num = std::unique_ptr<AbstractExpression>(new Fractal(&sec_sum->numerator));
    std::unique_ptr<AbstractExpression> sec_sum_num_num = std::unique_ptr<AbstractExpression>(new Number(sec_sum->coefficient.getNumerator()));
    sec_sum_num = sec_sum_num * sec_sum_num_num;
    std::unique_ptr<AbstractExpression> left_sum = this_num * sec_sum_denum;
    std::unique_ptr<AbstractExpression> right_sum = sec_sum_num * this_denum;

    return std::unique_ptr<Fractal>(new Fractal((left_sum + right_sum).get(), (this_denum * sec_sum_denum).get()));
}
std::unique_ptr<Fractal> Fractal::operator-(const std::unique_ptr<Fractal> &subtrahend)
{
    return *this + subtrahend * std::unique_ptr<Fractal>(new Fractal(-1));
}
void Fractal::simplify()
{
    SIM_IF_NEED
    for (auto &it: this->numerator)
    {
        it->simplify();
        it = it->downcast();
    }
    for (auto &it: this->denominator)
    {
        it->simplify();
        it = it->downcast();
    }
    this->takeCommonPartOfPolynomials();
    for (fractal_argument::iterator it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == FRACTAL)
        {
            this->addFractal(static_cast<Fractal*>(it->get()));
            it = this->numerator.erase(it);
        }
        else
            ++it;
    }
    for (fractal_argument::iterator it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == FRACTAL)
        {
            this->addFractal(static_cast<Fractal*>(it->get()), true);
            it = this->denominator.erase(it);
        }
        else
            ++it;
    }
    this->coefficient.simplify();
    //std::sort(this->numerator.begin(), this->numerator.end(), &AbstractExpression::less);
   // std::sort(this->denominator.begin(), this->denominator.end(), &AbstractExpression::less);

    this->numerator.sort(&AbstractExpression::lessToSort);
    this->denominator.sort(&AbstractExpression::lessToSort);
    this->multiplyIrrationalSums();
    this->getRidOfIrrationalityInDenominator();
    this->reduceDegrees();
    this->reduceSameMembers();
    this->reduceMembersWithDowngradingDegree();
    this->reduceComparibleDegrees();
//    this->getRidOfRootsInDenominator();
    this->turnDegreesIntoList();
    this->reducePolynomialsCoefficient();
    //ye, its extra, but it is unimportant becouse fractals in requlary geometric are small. it's maden for reduced polynomial.
    this->reduceSameMembers();
    this->reducePolynomials();
    this->pullNumbersIntoCoefficient();
    this->setSameMembersIntoDegree();
    this->castTrigonometry();
    this->simplified = true;
}
void Fractal::reduceDegrees()
{
    NONCONST
    for (auto &it : this->numerator)
    {
        if (it->getId() == DEGREE)
            this->setCoefficinet( this->coefficient * static_cast<Degree*>(it.get())->reduce());
    }
    for (auto &it : this->denominator)
    {
        if (it->getId() == DEGREE)
            this->setCoefficinet( this->coefficient / static_cast<Degree*>(it.get())->reduce());
    }
}
void Fractal::pullNumbersIntoCoefficient()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == NUMBER)
        {
            this->setCoefficinet( this->coefficient * *static_cast<Number*>(it->get()));
            it = this->numerator.erase(it);
        }
        else
            ++it;
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == NUMBER)
        {
            this->setCoefficinet( this->coefficient / *static_cast<Number*>(it->get()));
            it = this->denominator.erase(it);
        }
        else
            ++it;
    }
    this->coefficient.simplify();
}
void Fractal::reduceSameMembers()
{
    NONCONST
    //numerator and denominator must be sorted
    fractal_argument::iterator to_start_in_denominator = this->denominator.begin();
    bool has_erased = false;
    for (fractal_argument::iterator it1 = this->numerator.begin(); it1 != this->numerator.end();)
    {
        fractal_argument::iterator it2 = to_start_in_denominator;
        while (it2 != this->denominator.end() && !has_erased)
        {
            if (*it1->get() == *it2->get())
            {
                it1 = this->numerator.erase(it1);
                has_erased = true;
                it2 = this->denominator.erase(it2);
                to_start_in_denominator = it2;
            }
            else
                ++it2;
        }
        if (has_erased)
            has_erased = false;
        else
            ++it1;
    }
}

void Fractal::reduceMembersWithDowngradingDegree()
{
    NONCONST
    //this method assumes that reduceSameMembers was called before it, so one of reducing member should be degree and they won't dissapeare both
    //(except case that might happen if there can be situation when the same value is written in differen ways)
    //numerator and denominator must be sorted
   // fractal_argument::iterator to_start_in_denominator = this->denominator.begin();
    bool has_erased = false;
    for (fractal_argument::iterator it1 = this->numerator.begin(); it1 != this->numerator.end();)
    {
        fractal_argument::iterator it2 = this->denominator.begin();
        while (it2 != this->denominator.end() && it1 != this->numerator.end())
        {
           if (canReduceWithDowngradingDegree(it1->get(), it2->get()))
           {
               std::unique_ptr<AbstractExpression> newDegree = *Degree::getDegreeOfExpression(it1->get()) - *Degree::getDegreeOfExpression(it2->get());
               std::unique_ptr<AbstractExpression> arg = makeAbstractExpression(Degree::getArgumentOfDegree(it1->get())->getId(), Degree::getArgumentOfDegree(it1->get()));
               has_erased = true;
               it1 = this->numerator.erase(it1);
               it2 = this->denominator.erase(it2);

               if (newDegree->getId() == NUMBER && (static_cast<Number*>(newDegree.get()))->compareWith(0) < 0)
               {
                   std::unique_ptr<AbstractExpression> multiplier(new Number(-1));
                   this->pushBackToDenominator(std::unique_ptr<AbstractExpression>(new Degree(std::move(arg), newDegree * multiplier)));
               }
               else
                   this->pushBackToNumerator(std::unique_ptr<AbstractExpression>(new Degree(std::move(arg), std::move(newDegree))));
           }
           else
               ++it2;
        }
        if (has_erased)
            has_erased = false;
        else
            ++it1;
    }
}
void Fractal::reduceComparibleDegrees()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end(); )
    {
        if (it->get()->getId() == DEGREE && static_cast<Degree*>(it->get())->canDowncastTo(NUMBER))
        {
            auto deg = it->get()->downcastTo(NUMBER);
            Number * num = static_cast<Number*>(deg.get());

                this->setCoefficinet( this->coefficient * *num);
                it = this->numerator.erase(it);

        }
        else
            ++it;
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end(); )
    {
        if (it->get()->getId() == DEGREE && static_cast<Degree*>(it->get())->canDowncastTo(NUMBER))
        {
            auto deg = it->get()->downcastTo(NUMBER);
            Number * num = static_cast<Number*>(deg.get());
            if (num->isZero())
                throw Exception();
            else
            {
                this->setCoefficinet( this->coefficient * *num);
                it = this->denominator.erase(it);
            }
        }
        else
            ++it;
    }
}
void Fractal::turnDegreesIntoList()
{
    NONCONST
    for (fractal_argument::iterator it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == DEGREE)
        {
            Degree * it_deg = static_cast<Degree*>(it->get());
            if (it_deg->canGetListOfArguments())
            {
                auto list = std::move(it_deg->getListOfArguments());
                if (!list.second)
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToNumerator(std::move(it1));
                }
                else
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToDenominator(std::move(it1));
                }
                it = this->numerator.erase(it);
            }
            else
                ++it;
        }
        else
            ++it;
    }
    for (fractal_argument::iterator it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == DEGREE)
        {
            Degree * it_deg = static_cast<Degree*>(it->get());
            if (it_deg->canGetListOfArguments())
            {
                auto list = it_deg->getListOfArguments();
                if (list.second)
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToNumerator(std::move(it1));
                }
                else
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToDenominator(std::move(it1));
                }
                it = this->denominator.erase(it);
            }
            else
                ++it;
        }
        else
            ++it;
    }
}
bool Fractal::isPolynomial()
{
    return this->denominator.empty() && this->numerator.size() == 1 && this->numerator.begin()->get()->getId() == POLYNOMIAL;
}
void Fractal::addFractal(Fractal *fractal, bool as_denominator)
{
    NONCONST
    if (as_denominator)
    {
        this->setCoefficinet( this->coefficient / fractal->coefficient);
        for (auto &it : fractal->numerator)
            this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
        for (auto &it : fractal->denominator)
            this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    else
    {
        this->setCoefficinet( this->coefficient * fractal->coefficient);
        for (auto &it : fractal->numerator)
            this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
        for (auto &it : fractal->denominator)
            this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    }
}
void Fractal::reducePolynomialsCoefficient()
{
    NONCONST
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL)
            this->setCoefficinet( this->coefficient * static_cast<Polynomial*>(it.get())->reduce());

    }
    for (auto &it : this->denominator)
    {
        if (it->getId() == POLYNOMIAL)
            this->setCoefficinet( this->coefficient / static_cast<Polynomial*>(it.get())->reduce());
    }
}
std::set<int> Fractal::getSetOfPolyVariables() const
{
    std::set<int> set;
    for (auto &it : this->numerator)
    {
        std::set<int> s = it->getSetOfPolyVariables();
        set.insert(s.begin(), s.end());
    }
    for (auto &it : this->denominator)
    {
        std::set<int> s = it->getSetOfPolyVariables();
        set.insert(s.begin(), s.end());
    }

    return set;
}
std::set<int> Fractal::getSetOfVariables() const
{
    std::set<int> set;
    for (auto &it : this->numerator)
    {
        std::set<int> s = it->getSetOfVariables();
        set.insert(s.begin(), s.end());
    }
    for (auto &it : this->denominator)
    {
        std::set<int> s = it->getSetOfVariables();
        set.insert(s.begin(), s.end());
    }

    return set;
}
Number Fractal::getMaxDegreeOfVariable(int id)
{
    Number deg(std::numeric_limits<long long int>::min() + 1);
    bool has_var = false;
    for (auto &it : this->numerator)
    {
        Number it_deg = it->getMaxDegreeOfVariable(id);
        if (it_deg.isCorrect())
        {
            has_var = true;
            if (it_deg.compareWith(deg) > 0)
                deg = it_deg;
        }
        else
            return Number::makeErrorNumber();
    }
    for (auto &it : this->denominator)
    {
        Number it_deg = it->getMaxDegreeOfVariable(id) * -1;
        if (it_deg.isCorrect())
        {
            has_var = true;
            if (it_deg.compareWith(deg) > 0)
                deg = it_deg;
        }
        else
            return Number::makeErrorNumber();
    }
    if (has_var)
        return deg;
    return 0;
}

std::unique_ptr<Fractal> Fractal::getFractalWithoutVariable(int id)
{
    std::unique_ptr<Fractal> result = std::make_unique<Fractal>(this);
    for (auto it = result->numerator.begin(); it != result->numerator.end();)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == id)
            it = result->numerator.erase(it);
        else
            ++it;
    }
    for (auto it = result->denominator.begin(); it != result->denominator.end();)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == id)
            it = result->denominator.erase(it);
        else
            ++it;
    }
    return result;
}

void Fractal::reducePolynomials()
{
    NONCONST
    for (auto it1 = this->numerator.begin(); it1 != this->numerator.end(); ++it1)
    {
        for (auto it2 = this->denominator.begin(); it2 != this->denominator.end(); ++it2)
        {

            if (it1->get()->getId() == POLYNOMIAL && it2->get()->getId() == POLYNOMIAL)
            {
                auto gcf = gcd(static_cast<Polynomial*>(it1->get()), static_cast<Polynomial*>(it2->get()));

                auto it1_pol = static_cast<Polynomial*>(it1->get())->divide(gcf.get()).first;
                if (it1_pol == nullptr)
                    continue;
                auto it1_expr = makeAbstractExpression(POLYNOMIAL, it1_pol.get());
                it1->swap(it1_expr);
                *it1 = it1->get()->downcast();
                auto it2_pol = static_cast<Polynomial*>(it2->get())->divide(gcf.get()).first;
                auto it2_expr = makeAbstractExpression(POLYNOMIAL, it2_pol.get());
                it2->swap(it2_expr);
                *it2 = it2->get()->downcast();
            }
        }
    }
    this->reduceSameMembers();
}
std::unique_ptr<Fractal> Fractal::operator*(const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient * right->coefficient);
    for (auto &it : right->numerator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right->denominator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator*(const Fractal & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient * right.coefficient);

    for (auto &it : right.numerator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right.denominator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator/(const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient / right->coefficient);
    for (auto &it : right->numerator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right->denominator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator/(const Fractal & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient / right.coefficient);
    for (auto &it : right.numerator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right.denominator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
bool Fractal::canTurnIntoPolynomWithOpeningParentheses()
{
    if (!this->denominator.empty() || this->coefficient.getDenominator() != 1)
        return false;
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL || (it->getId() == DEGREE && static_cast<Degree*>(it.get())->canTurnIntoPolynomial()))
            return true;
    }
    return false;
}
std::unique_ptr<Polynomial> Fractal::turnIntoPolynomWithOpeningParentheses()
{
    //in this function, if fractal has several polynoms, it has to do those operations with 1. another will turning is polynom->simplify() which call this function
    assert(canTurnIntoPolynomWithOpeningParentheses());
  //  qDebug() << this->makeStringOfExpression();
    std::unique_ptr<Fractal> frac_without_polynom = std::unique_ptr<Fractal>(new Fractal(this));
    frac_without_polynom->turnDegreesIntoList();
    std::unique_ptr<Polynomial> polynom;
    for (auto it = frac_without_polynom->numerator.begin(); it != frac_without_polynom->numerator.end(); ++it)
    {
        if (it->get()->getId() == POLYNOMIAL)
        {
            polynom = std::unique_ptr<Polynomial>(new Polynomial(it->get()));
            frac_without_polynom->numerator.erase(it);
            break;
        }

    }
 //   qDebug() << frac_without_polynom->makeStringOfExpression();
  //  qDebug() << "POLYNOMS_MONOMS: ";
 //   for (auto &it : polynom->getMonomialsPointers())
  //      qDebug() << "    " << it->makeStringOfExpression();
    std::list<Fractal*> polynoms_monoms = polynom->getMonomialsPointers();
    polynom.release();
    polynom = std::unique_ptr<Polynomial>(new Polynomial());
    for (auto &it : polynoms_monoms)
    {
        polynom->addMonomial(*it * frac_without_polynom);
    }
   // qDebug() << "RES_POL: " << polynom->makeStringOfExpression();

    return polynom;
}
void Fractal::setSameMembersIntoDegree()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end(); ++it)
    {
        for (auto it1 = it; it1 != this->numerator.end();)
        {
            if (it1 == it)
                ++it1;
            else
            {
                if (*Degree::getArgumentOfDegree(it->get()) == *Degree::getArgumentOfDegree(it1->get()))
                {
                    AbstractExpression * degree = Degree::getArgumentOfDegree(it->get());
                     auto degr = std::unique_ptr<AbstractExpression>(new Degree(std::unique_ptr<AbstractExpression>(makeAbstractExpression(degree->getId(), degree)),
                        Degree::getDegreeOfExpression(it->get()) + Degree::getDegreeOfExpression(it1->get())));
                     degr = degr->downcast();
                     it->swap(degr);
                     it1 = this->numerator.erase(it1);
                }
                else
                    ++it1;
            }
        }
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end(); ++it)
    {
        for (auto it1 = it; it1 != this->denominator.end();)
        {
            if (it1 == it)
                ++it1;
            else
            {
                if (*Degree::getArgumentOfDegree(it->get()) == *Degree::getArgumentOfDegree(it1->get()))
                {
                     AbstractExpression * degree = Degree::getArgumentOfDegree(it->get());
                     auto degr = std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(degree->getId(), degree),
                        Degree::getDegreeOfExpression(it->get()) + Degree::getDegreeOfExpression(it1->get())));
                     degr = degr->downcast();
                     it->swap(degr);
                     it1 = this->denominator.erase(it1);
                }
                else
                    ++it1;
            }
        }
    }
    this->pullNumbersIntoCoefficient();
}
void Fractal::_qDebugOut()
{
    qDebug() << "FRACTAL:";
    qDebug() << "Coefficient:";
    this->coefficient._qDebugOut();
    qDebug() << "End of coefficient;";
    qDebug() << "Numerator: <list of members>";
    for (auto &it : this->numerator)
    {
        it->_qDebugOut();
        qDebug() << "#####";
    }
    qDebug() << "End of numerator;";
    qDebug() << "Denominator: <list of members>";
    for (auto &it : this->denominator)
    {
        it->_qDebugOut();
        qDebug() << "#####";
    }
    qDebug() << "End of denominator;";
    qDebug() << "END OF FRACTAL;";
}
bool Fractal::lessFrac(const std::unique_ptr<Fractal> &left, const std::unique_ptr<Fractal> &right)
{

    if (left->getId() != right->getId())
    {
        if ((left->getId() < 0 && right->getId() < 0) || (left->getId() > 0 && right->getId() > 0))
            return left->getId() < right->getId();
        else
        {
            return left->getId() > right->getId();
        }
    }
    else
        return *left < *right;

}
Fractal::Fractal(std::unique_ptr<AbstractExpression> && num, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(std::move(num));
    this->simplify();
}
QString Fractal::makeStringOfExpression()
{
    QString result = "(";
    if (!this->coefficient.isOne())
        result += this->coefficient.makeStringOfExpression();
    for (auto &it : this->numerator)
        result += "*" + it->makeStringOfExpression();
    if (result.size() == 1)
        result += "1";
    for (auto &it : this->denominator)
        result += "/" + it->makeStringOfExpression();
    result += ")";
    if (result[1] == "*")
        result = result.remove(1, 1);
    return result;
}
//i need this prefix becouse for some reasons in definition of Fractal::findCommonPart this overloading is not seen
fractal_argument _findCommonPart(fractal_argument & first, fractal_argument & second)
{
    fractal_argument result;
    for (auto &it1 : first)
    {
        for (auto &it2 : second)
        {
            if (*Degree::getArgumentOfDegree(it1.get()) == *Degree::getArgumentOfDegree(it2.get()))
            {
                auto deg1 = Degree::getDegreeOfExpression(it1.get());
                auto deg2 = Degree::getDegreeOfExpression(it2.get());
                std::unique_ptr<AbstractExpression> deg_to_set;
                if (*deg1 == *deg2)
                    deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                else if (deg1->getId() == NUMBER && deg2->getId() == NUMBER)
                    deg_to_set = std::unique_ptr<AbstractExpression>(new Number(static_cast<Number*>(deg1.get())->compareWith(*static_cast<Number*>(deg2.get())) < 0 ?
                                                                                    *static_cast<Number*>(deg1.get()) : *static_cast<Number*>(deg2.get())));
                else if (deg1->getId() == NUMBER)
                    deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                else if (deg2->getId() == NUMBER)
                    deg_to_set = makeAbstractExpression(deg2->getId(), deg2.get());
                else
                {
                    auto sub_result = deg1 - deg2;
                    if (sub_result->getId() == NUMBER)
                    {
                        if (static_cast<Number*>(sub_result.get())->compareWith(0) > 0)
                            deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                        else
                            deg_to_set = makeAbstractExpression(deg2->getId(), deg2.get());
                    }
                    else
                    {
                        auto div_result = deg1 / deg2;
                        if (div_result->getId() == NUMBER)
                        {
                            if (static_cast<Number*>(div_result.get())->compareWith(1) > 0)
                                deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                            else
                                deg_to_set = makeAbstractExpression(deg2->getId(), deg2.get());
                        }
                        else //здесь уже пофиг
                            deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                    }
                }
                 if (deg_to_set->getId() == NUMBER && static_cast<Number*>(deg_to_set.get())->isOne())
                    result.push_back(makeAbstractExpression(Degree::getArgumentOfDegree(it1.get())->getId(), Degree::getArgumentOfDegree(it1.get())));
                else
                    result.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(Degree::getArgumentOfDegree(it1.get())->getId(),
                                                                                                                 Degree::getArgumentOfDegree(it1.get())), deg_to_set)));
            }
        }
    }
    return result;

}
void Fractal::takeCommonPartOfPolynomials()
{
   NONCONST
    for (auto &it : this->numerator)
        if (it->getId() == POLYNOMIAL)
            this->pushBackToNumerator(static_cast<Polynomial*>(it.get())->reduceCommonPart()->downcast());
    for (auto &it : this->denominator)
        if (it->getId() == POLYNOMIAL)
            this->pushBackToDenominator(static_cast<Polynomial*>(it.get())->reduceCommonPart()->downcast());
}
std::unique_ptr<AbstractExpression> Fractal::findCommonPart(Fractal *frac)
{
    std::unique_ptr<AbstractExpression> result(new Fractal);
    Fractal * fr_res = static_cast<Fractal*>(result.get());
    fr_res->setCoefficinet(Number(gcd(this->coefficient.getNumerator(), frac->coefficient.getNumerator()), gcd(this->coefficient.getDenominator(), frac->coefficient.getDenominator())));
    if (this->coefficient.compareWith(0) < 0 && frac->coefficient.compareWith(0) < 0)
        fr_res->setCoefficinet( fr_res->coefficient * -1);
    fr_res->numerator = _findCommonPart(this->numerator, frac->numerator);
    fr_res->denominator = _findCommonPart(this->denominator, frac->denominator);
    return result;
}
void multiplyIrrationalSumsInFractal(fractal_argument * arg)
{
    std::unique_ptr<AbstractExpression> result(nullptr);
    bool has_found_first = false;
    fractal_argument::iterator first;

    for (auto it = arg->begin(); it != arg->end();)
    {
        bool has_erased_it = false;
        if (it->get()->getId() == POLYNOMIAL && static_cast<Polynomial*>(it->get())->isIrrationalSum())
        {
            if (!has_found_first)
            {
                first = it;
                has_found_first = true;
            }
            else
            {
                std::unique_ptr<AbstractExpression> zero(new Number(0));
                if (result == nullptr)
                {
                    result = (*first * *it) + zero;
                    arg->erase(first);
                }
                else
                    result = result * *it + zero;
                it = arg->erase(it);
                has_erased_it = true;
            }
        }
        if (!has_erased_it)
            ++it;
    }
    if (result != nullptr)
        arg->push_back(std::move(result));
}
void Fractal::multiplyIrrationalSums()
{
    NONCONST
    //the problem is that multiplyIrrationalSumsInFractal() calls operator * that will call this function again, so, this condition predicts recursion
    if (this->denominator.empty() && this->coefficient.getDenominator() == 1)
        return;
    multiplyIrrationalSumsInFractal(&this->numerator);
    multiplyIrrationalSumsInFractal(&this->denominator);
    this->takeCommonPartOfPolynomials();
}
void Fractal::getRidOfIrrationalityInDenominator()
{
    NONCONST
    //after multipliing irrational sums there should be only one such sum in denom
    bool has_done_multipliing = true;
    while (has_done_multipliing)
    {
        has_done_multipliing = false;
        for (auto it = this->denominator.begin(); it != this->denominator.end(); ++it)
            if (it->get()->getId() == POLYNOMIAL && static_cast<Polynomial*>(it->get())->isIrrationalSum())
            {
                auto res = static_cast<Polynomial*>(it->get())->multiplyIrrationalSumOnAppropriateFormula();
              //  qDebug()<<"F: " << res.first->makeStringOfExpression();
              //  qDebug() <<"S: " << res.second->makeStringOfExpression();
                this->pushBackToNumerator(std::move(res.second));
                *it = std::move(res.first);
                has_done_multipliing = true;
                break;
            }

    }

    this->multiplyIrrationalSums();

}
std::unique_ptr<Fractal> operator*(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    return *left * right;
}
std::unique_ptr<Fractal> operator/(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    return *left / right;
}
void Fractal::sortVariablesInIncreasingOfTheirId()
{
    NONCONST
    this->numerator.sort([](const std::unique_ptr<AbstractExpression> & a, const std::unique_ptr<AbstractExpression> & b)->bool{
        return Degree::getArgumentOfDegree(a.get())->getId() <  Degree::getArgumentOfDegree(b.get())->getId();
    });
}
void Fractal::pushBackToNumerator(std::unique_ptr<AbstractExpression> && expr)
{
    NONCONST
    this->numerator.push_back(std::move(expr));
}
void Fractal::pushBackToDenominator(std::unique_ptr<AbstractExpression> && expr)
{
    NONCONST
    this->denominator.push_back(std::move(expr));
}
//функция пока не прижилась, наверно уберу.
void Fractal::getRidOfRootsInDenominator()
{
    NONCONST
    bool done_something = false;
    for (auto &it : this->denominator)
    {
        if (it->getId() == DEGREE && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->getDenominator() != 1)
        {
            done_something = true;
            Number  deg = *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get());
            long long int new_den_deg = deg.getNumerator() / deg.getDenominator() + 1;
            std::unique_ptr<AbstractExpression> mult_deg_pt(new Number(Number(new_den_deg)-deg));
            std::unique_ptr<AbstractExpression> den_deg(new Number(new_den_deg));
           // it = std::unique_ptr<AbstractExpression> (new Degree(std::unique_ptr<AbstractExpression>(
          //                                                            Degree::getArgumentOfDegree(it.get())),
         //                                                        std::move(den_deg)));
            AbstractExpression * arg = Degree::getArgumentOfDegree(it.get());
            it = std::unique_ptr<AbstractExpression> (new Degree(makeAbstractExpression(arg->getId(), arg),
                                                                std::move(den_deg)));
             arg = Degree::getArgumentOfDegree(it.get());
            this->numerator.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(arg->getId(), arg),
                                                                                     std::move(mult_deg_pt))));
        }
    }
    if (done_something)
        this->simplify();
}
double Fractal::getApproximateValue()
{
    assert(this->getSetOfVariables().empty());
    double value = this->coefficient.getApproximateValue();
    for (auto &it : this->numerator)
        value *= it->getApproximateValue();
    for (auto &it : this->denominator)
        value /= it->getApproximateValue();
    return value;
}
int Fractal::getPositionRelativelyZeroIfHasVariables()
{
    bool is_negative = this->coefficient.compareWith(0) < 0;
    for (auto &it : this->numerator)
    {
        int pos = it->getPositionRelativelyZero();
        if (pos == 0)
            return 0;
        if (pos < 0)
            is_negative = !is_negative;
    }
    for (auto &it : this->denominator)
    {
        int pos = it->getPositionRelativelyZero();
        if (pos == 0)
            return 0;
        if (pos < 0)
            is_negative = !is_negative;
    }
    return is_negative ? -1 : 1;
}
double Fractal::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    double value = this->coefficient.getApproximateValue();
    for (auto &it : this->numerator)
        value *= it->getApproximateValue(choosing_value_rule);
    for (auto &it : this->denominator)
    {
        double ret_val = it->getApproximateValue(choosing_value_rule);
        if (ret_val == 0.0)
            return std::nan("1");
        value /= ret_val;
    }
    return value;
}

void Fractal::intoAcceptedSignForm()
{
    bool change_sign = false;
    for (auto &it : this->numerator)
        if (it->getId() == POLYNOMIAL)
            if (static_cast<Polynomial*>(it.get())->isNeedToChangeSignToLeadTheAcceptedForm())
            {
                static_cast<Polynomial*>(it.get())->changeSign();
                change_sign = !change_sign;
            }
    for (auto &it : this->denominator)
        if (it->getId() == POLYNOMIAL)
            if (static_cast<Polynomial*>(it.get())->isNeedToChangeSignToLeadTheAcceptedForm())
            {
                static_cast<Polynomial*>(it.get())->changeSign();
                change_sign = !change_sign;
            }
    if (change_sign)
        this->coefficient *= -1;
}

void Fractal::convertTrigonometricalFunctionsByFormulas(const std::map<QString, TrigonometricalFunctionsCastType> &instructions)
{
    auto convert = [&instructions](abs_ex & convertible)
    {
        if (!isDegreeOfTrigonometricalFunction(convertible))
            return;
        auto convert_type = instructions.find(getStringArgumentOfTrigonometricalFunction(convertible.get()))->second;
        if (convert_type == NONE)
            return;
        if (convert_type == COSINUS_TO_SINUS || convert_type == SINUS_TO_COSINUS || convert_type == TANGENT_TO_COSINUS || convert_type == COSINUS_TO_TANGENT ||
        convert_type == COTANGENT_TO_SINUS || convert_type == SINUS_TO_COTANGENT)
        {
            auto degree = Degree::getDegreeOfExpression(convertible.get());
            if (degree->getId() != NUMBER)
                return;
            Number deg = *static_cast<Number*>(degree.get());
            if (!(deg / 2).isInteger())
                return;
            auto arg = Degree::getArgumentOfDegree(convertible.get());
            abs_ex res;
            switch (convert_type) {
                case COSINUS_TO_SINUS:
                    if (arg->getId() != COSINUS)
                        return;
                    res = cosinusToSinus(std::move(convertible));
                break;
                case SINUS_TO_COSINUS:
                    if (arg->getId() != SINUS)
                        return;
                    res = sinusToCosinus(std::move(convertible));
                break;
                case TANGENT_TO_COSINUS:
                    if (arg->getId() != TANGENT)
                        return;
                    res = tangentToCosinus(std::move(convertible));
                break;
                case COSINUS_TO_TANGENT:
                    if (arg->getId() != COSINUS)
                        return;
                    res = cosinusToTangent(std::move(convertible));
                break;
                case COTANGENT_TO_SINUS:
                    if (arg->getId() != COTANGENT)
                        return;
                    res = cotangentToSinus(std::move(convertible));
                break;
                case SINUS_TO_COTANGENT:
                    if (arg->getId() != SINUS)
                        return;
                    res = sinusToCotangent(std::move(convertible));
                break;
            }
           convertible = takeDegreeOf(res, deg / 2);
        }
        if (convert_type == TANGENTS_AND_COTANGENTS_TO_FRACTAL || convert_type == TANGENT_TO_COTANGENT || convert_type == COTANGENT_TO_TANGENT)
        {
            auto arg = Degree::getArgumentOfDegree(convertible.get());
            if (arg->getId() != TANGENT && arg->getId() != COTANGENT)
                return;
            switch(convert_type)
            {
                case TANGENTS_AND_COTANGENTS_TO_FRACTAL:
                    if (arg->getId() == TANGENT)
                        convertible = tangentToFractal(std::move(convertible));
                    else
                        convertible = cotangentToFractal(std::move(convertible));
                break;
                case TANGENT_TO_COTANGENT:
                    if (arg->getId() != TANGENT)
                        return;
                    convertible = tangentToCotangent(std::move(convertible));
                break;
                case COTANGENT_TO_TANGENT:
                    if (arg->getId() != COTANGENT)
                        return;
                    convertible = cotangentToTangent(std::move(convertible));
                break;

            }
        }
    };
    for (auto &it : this->numerator)
    {
        convert(it);
    }
    for (auto &it : this->denominator)
    {
        convert(it);
    }
}

void Fractal::checkTrigonometricalFunctionsItHas(std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> > &params)
{
    auto check = [&params](abs_ex & it)->void {
        AbstractExpression * arg = Degree::getArgumentOfDegree(it.get());
        if (it->getId() == DEGREE &&  (Degree::getDegreeOfExpression(it.get())->getId() == NUMBER && *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get()) == 2))
        {
            Degree * deg = static_cast<Degree*>(it.get());
            if (deg->getArgumentOfDegree(deg)->getId() == SINUS )
                std::get<0>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
            else if (deg->getArgumentOfDegree(deg)->getId() == COSINUS)
                std::get<2>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
            else if (deg->getArgumentOfDegree(deg)->getId() == TANGENT)
                std::get<4>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
            else if (deg->getArgumentOfDegree(deg)->getId() == COTANGENT)
                std::get<6>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        }
        else if (arg->getId() == SINUS)
            std::get<1>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        else if (arg->getId() == COSINUS)
            std::get<3>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        else if (arg->getId() == TANGENT)
            std::get<5>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        else if (arg->getId() == COTANGENT)
            std::get<7>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
    };
    for (auto &it : this->numerator)
    {
        check(it);
    }
    for (auto &it : this->denominator)
    {
        check(it);
    }
}

void Fractal::castTrigonometry()
{

    this->casted_trigonometry = false;
    auto rules = this->getTrigonometricalFunctionToTurnIntoIt();
    bool has_to_cast = false;
    for (auto &it : rules)
    {
        if (it.second != NONE)
        {
            has_to_cast = true;
            break;
        }
    }
    if (!has_to_cast)
        return;
    this->casted_trigonometry = true;
    this->convertTrigonometricalFunctionsByFormulas(rules);
    this->simplified = false;

    this->simplify();
}

std::map<QString, TrigonometricalFunctionsCastType> Fractal::getTrigonometricalFunctionToTurnIntoIt()
{
    std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> params;
    this->checkTrigonometricalFunctionsItHas(params);
    return chooseConversionRules(params);
}

std::unique_ptr<Fractal> operator+(const std::unique_ptr<Fractal> &left, const std::unique_ptr<Fractal> &right)
{
    return *left + right;
}
std::unique_ptr<Fractal> operator-(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    return *left - right;
}

std::unique_ptr<AbstractExpression> toAbsEx(std::unique_ptr<Fractal> &expr)
{
    return abs_ex(new Fractal(expr.get()));
}

std::unique_ptr<AbstractExpression> toAbsEx(std::unique_ptr<Fractal> &&expr)
{
    return abs_ex(expr.release());
}
