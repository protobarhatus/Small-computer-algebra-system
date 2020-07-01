#include "fractal.h"
#include "assert.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
#include "algorithm"
#include "degree.h"
#include "polynomial.h"
#include "exception.h"
#include <cmath>
#include "cosinus.h"
#include "sinus.h"
#include "abstractexpression.h"
#include "logarithm.h"
#include "absolutevalue.h"
#include "tangent.h"
#include "cotangent.h"
#include "variablesdistributor.h"
#include "arctangent.h"
#include "arcsinus.h"
#include "constant.h"
#include "solving_equations.h"
#include "differential.h"
#include "solving_differential_equations.h"
#include "factorization.h"
Fractal::Fractal() : coefficient(1)
{

}
AlgebraExpression Fractal::getId() const
{
    return FRACTAL;
}
Fractal::Fractal(AbstractExpression * num, AbstractExpression * denum, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(copy(num));
    this->pushBackToDenominator(copy(denum));
    this->simplify();
}
Fractal::Fractal(const fractal_argument * num, const fractal_argument * denum, Number coe) : coefficient(coe)
{
    for (auto &it : *num)
    {
        this->pushBackToNumerator(copy(it));
    }
    for (auto &it : *denum)
    {
        this->pushBackToDenominator(copy(it));
    }
    this->simplify();
}

Fractal::Fractal(const fractal_argument &num, const fractal_argument &denum, Number coe) : coefficient(coe)
{
    for (auto &it : num)
    {
        this->pushBackToNumerator(copy(it));
    }
    for (auto &it : denum)
    {
        this->pushBackToDenominator(copy(it));
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

Fractal::Fractal(fractal_argument &&num, Number coe) : coefficient(coe)
{
    for (auto &it : num)
        this->pushBackToNumerator(std::move(it));
    this->simplify();
}

Fractal::Fractal(abs_ex &&num, abs_ex &&denum, Number coe)
{
    this->pushBackToNumerator(std::move(num));
    this->pushBackToDenominator(std::move(denum));
    this->coefficient = coe;
    this->simplify();
}
Fractal::Fractal(fractal_argument * num, Number coe) : coefficient(coe)
{
    for (auto &it : *num)
    {
        this->pushBackToNumerator(copy(it));
    }
    this->simplify();
}

Fractal::Fractal(const fractal_argument &num, Number coe) : coefficient(coe)
{
    for (auto &it : num)
    {
        this->pushBackToNumerator(copy(it));
    }
    this->simplify();
}
Fractal::Fractal(AbstractExpression * num, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(copy(num));
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
        this->pushBackToNumerator(copy(it));
    for (auto &it : right.denominator)
        this->pushBackToDenominator(copy(it));
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
Number Fractal::getCoefficient() const
{
    return this->coefficient;
}
bool Fractal::canDowncastTo()
{
    if (this->isOnlyVarsIntegratingConstantsThatCanBeChanged())
        return true;
    return (this->denominator.empty() && this->coefficient.getDenominator() == 1 && this->coefficient.getNumerator() == 1 && this->numerator.size() == 1) ||
            (this->numerator.empty() && this->denominator.empty()) ||
            (this->coefficient.getNumerator() == 0);
}
abs_ex Fractal::downcastTo()
{
    assert(this->canDowncastTo());
    if (this->numerator.empty() && this->denominator.empty())
    {
        return toAbsEx(this->coefficient);
    }
    if (this->coefficient.isZero())
        return copy(zero);
    if (this->coefficient.isOne() && this->denominator.empty() && this->numerator.size() == 1 &&
            isIntegratingConstantAndCanChangeIt(this->numerator.begin()->get()->getId()))
            return integratingConstantExpr(this->numerator.begin()->get()->getId(), this->getRange());
    if (this->isOnlyVarsIntegratingConstantsThatCanBeChanged())
    {
      //  qDebug() << this->toString();
        return integratingConstantExpr(this->getRange());
    }
    return std::move( *this->numerator.begin());
}
bool Fractal::isZero() const
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
        result.push_back(copy(it));
    for (auto &it : smult)
        result.push_back(copy(it));
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
        abs_ex newnum_ptr = toAbsEx(static_cast<AbstractExpression*>(new Fractal(&this->numerator, this->coefficient.getNumerator()))) +
                toAbsEx(static_cast<AbstractExpression*>(new Fractal(&sec_sum->numerator, sec_sum->coefficient.getNumerator())));
        fractal_argument newnum;
        newnum.push_back(std::move(newnum_ptr));
        std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&newnum, &this->denominator, Number(1, this->coefficient.getDenominator())/*, this->coefficient * sec_sum->coefficient*/));
        result->simplify();
        return result;
    }
    /*
    fractal_argument left_sum = this->numerator * (sec_sum->denominator / this->denominator);
    fractal_argument right_sum = sec_sum->numerator * this->denominator;
    abs_ex newnum_ptr = *static_cast<AbstractExpression*>(new Fractal(&left_sum)) +
            *static_cast<AbstractExpression*>(new Fractal(&right_sum));
    fractal_argument newnum;
    newnum.push_back(std::move(newnum_ptr));
    fractal_argument newdenum = this->denominator * sec_sum->denominator;
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&newnum, &newdenum, this->coefficient * sec_sum->coefficient));
    result->simplify();*/

    abs_ex this_denum = abs_ex(new Fractal(&this->denominator));
    abs_ex this_num_denum = abs_ex(new Number(this->coefficient.getDenominator()));
    this_denum = this_denum * this_num_denum;
    abs_ex sec_sum_denum = std:: unique_ptr<AbstractExpression>(new Fractal(&sec_sum->denominator));
    abs_ex sec_sum_num_denum = abs_ex(new Number(sec_sum->coefficient.getDenominator()));
    sec_sum_denum = sec_sum_denum * sec_sum_num_denum;

    abs_ex this_num = abs_ex(new Fractal(&this->numerator));
    abs_ex this_num_num = abs_ex(new Number(this->coefficient.getNumerator()));
    this_num = this_num * this_num_num;
    abs_ex sec_sum_num = abs_ex(new Fractal(&sec_sum->numerator));
    abs_ex sec_sum_num_num = abs_ex(new Number(sec_sum->coefficient.getNumerator()));
    sec_sum_num = sec_sum_num * sec_sum_num_num;
    abs_ex left_sum = this_num * sec_sum_denum;
    abs_ex right_sum = sec_sum_num * this_denum;

    return std::unique_ptr<Fractal>(new Fractal((left_sum + right_sum).get(), (this_denum * sec_sum_denum).get()));
}
std::unique_ptr<Fractal> Fractal::operator-(const std::unique_ptr<Fractal> &subtrahend)
{
    return *this + subtrahend * std::unique_ptr<Fractal>(new Fractal(-1));
}
void Fractal::simplify()
{
    SIM_IF_NEED
      //      qDebug() << this->toString();
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
    //qDebug() << this->toString();
    this->takeCommonPartOfPolynomials();
   // qDebug() << this->toString();
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
   // qDebug() << this->toString();
    this->pullNumbersIntoCoefficient();
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
   // qDebug() << this->toString();
    this->takeCommonPartOfPolynomials();
    for (auto &it: this->numerator)
        it = it->downcast();
    for (auto &it: this->denominator)
        it = it->downcast();
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
    this->reduceSameMembers();
    this->pullNumbersIntoCoefficient();
    this->setSameMembersIntoDegree();
    this->castTrigonometry();
    this->castTrigonometryArguments();
    if (this->hasIntegratingConstantMultiplierThatCanBeChanged())
    {
        this->pullSomeMultipliersIntoIntegratingConstant();
        if (this->numerator.size() > 0 && isIntegratingConstantAndCanChangeIt(this->numerator.back()->getId()) &&
                VariablesDistributor::get().getVariablesDefinition(this->numerator.back()->getId())->getRange().isSymmetricRelativelyZero())
            this->takeAwayAbsoluteValues();
    }
    this->numerator.sort(&AbstractExpression::lessToSort);
    this->denominator.sort(&AbstractExpression::lessToSort);
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
               abs_ex newDegree = Degree::getDegreeOfExpression(it1->get()) - Degree::getDegreeOfExpression(it2->get());
               abs_ex arg = copy(Degree::getArgumentOfDegree(it1->get()));
               has_erased = true;
               it1 = this->numerator.erase(it1);
               it2 = this->denominator.erase(it2);

               if (newDegree->getId() == NUMBER && (static_cast<Number*>(newDegree.get()))->compareWith(0) < 0)
               {
                   abs_ex multiplier(new Number(-1));
                   this->pushBackToDenominator(abs_ex(new Degree(std::move(arg), newDegree * multiplier)));
               }
               else
                   this->pushBackToNumerator(abs_ex(new Degree(std::move(arg), std::move(newDegree))));
           }
           else if (Degree::getArgumentOfDegree(it1->get())->getId() == ABSOLUTE_VALUE &&
                    Degree::getDegreeOfExpression(it2->get())->getId() == NUMBER &&
                    (static_cast<Number*>(Degree::getDegreeOfExpression(it2->get()).get())->getNumerator() % 2 == 0 ||
                     static_cast<Number*>(Degree::getDegreeOfExpression(it2->get()).get())->getDenominator() % 2 == 0) &&
                    *static_cast<AbsoluteValue*>(Degree::getArgumentOfDegree(it1->get()))->getExpression() ==
                    *Degree::getArgumentOfDegree(it2->get()))
           {
               auto deg1 = Degree::getDegreeOfExpression(it1->get());
               auto deg2 = Degree::getDegreeOfExpression(it2->get());
               auto deg_diff = deg1 - deg2;
               if (*deg_diff == *zero)
               {
                   ++it2;
                   continue;
               }
               if (deg_diff->getPositionRelativelyZero() >= 0)
                   this->pushBackToNumerator(pow(abs(copy(Degree::getArgumentOfDegree(it2->get()))), deg_diff));
               else
                   this->pushBackToDenominator(pow(abs(copy(Degree::getArgumentOfDegree(it2->get()))), -deg_diff));
               has_erased = true;
               it1 = this->numerator.erase(it1);
               it2 = this->denominator.erase(it2);

           }
           else if (Degree::getArgumentOfDegree(it2->get())->getId() == ABSOLUTE_VALUE &&
                    Degree::getDegreeOfExpression(it1->get())->getId() == NUMBER &&
                    (static_cast<Number*>(Degree::getDegreeOfExpression(it1->get()).get())->getNumerator() % 2 == 0 ||
                     static_cast<Number*>(Degree::getDegreeOfExpression(it1->get()).get())->getDenominator() % 2 == 0) &&
                    *static_cast<AbsoluteValue*>(Degree::getArgumentOfDegree(it2->get()))->getExpression() ==
                    *Degree::getArgumentOfDegree(it1->get()))
           {
               auto deg1 = Degree::getDegreeOfExpression(it1->get());
               auto deg2 = Degree::getDegreeOfExpression(it2->get());
               auto deg_diff = deg1 - deg2;;
               if (*deg_diff == *zero)
               {
                   ++it2;
                   continue;
               }
               if (deg_diff->getPositionRelativelyZero() >= 0)
                   this->pushBackToNumerator(pow(abs(copy(Degree::getArgumentOfDegree(it1->get()))), deg_diff));
               else
                   this->pushBackToDenominator(pow(abs(copy(Degree::getArgumentOfDegree(it1->get()))), -deg_diff));
               has_erased = true;
               it1 = this->numerator.erase(it1);
               it2 = this->denominator.erase(it2);

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
    /*NONCONST
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
    }*/
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
            this->pushBackToDenominator(std::move(it));
        for (auto &it : fractal->denominator)
            this->pushBackToNumerator(std::move(it));
    }
    else
    {
        this->setCoefficinet( this->coefficient * fractal->coefficient);
        for (auto &it : fractal->numerator)
            this->pushBackToNumerator(std::move(it));
        for (auto &it : fractal->denominator)
            this->pushBackToDenominator(std::move(it));
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
        auto s = it->getSetOfVariables();
        for (auto &it1 : s)
            if (has(set, it1))
                set.erase(it1);
    }
    /*for (auto &it : this->denominator)
    {
        std::set<int> s = it->getSetOfPolyVariables();
        set.insert(s.begin(), s.end());
    }*/

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

std::set<QString> Fractal::getSetOfFunctions() const
{

    std::set<QString> set;
    auto merge = [&set](const std::set<QString> & s) {
        for (auto &it : s)
            set.insert(it);
    };
    for (auto &it : this->numerator)
    {
        auto itset = it->getSetOfFunctions();
        merge(itset);
       // set.merge(itset);
    }
    for (auto &it : this->denominator)
    {
        auto itset = it->getSetOfFunctions();
        merge(itset);
       // set.merge(itset);
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

std::unique_ptr<Fractal> Fractal::getFractalWithoutVariable(int id) const
{
    //std::unique_ptr<Fractal> result = std::make_unique<Fractal>(this);
    std::unique_ptr<Fractal> result = toFrac(copy(this));
    for (auto it = result->numerator.begin(); it != result->numerator.end();)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == id)
            it = result->numerator.erase(it);
        else
        {
            auto set = it->get()->getSetOfVariables();
            if (set.find(id) == set.end())
                ++it;
            else
                it = result->numerator.erase(it);
        }

    }
    for (auto it = result->denominator.begin(); it != result->denominator.end();)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == id)
            it = result->denominator.erase(it);
        else
        {
            auto set = it->get()->getSetOfVariables();
            if (set.find(id) == set.end())
                ++it;
            else
                it = result->denominator.erase(it);
        }
    }
    return result;
}

void Fractal::reducePolynomials()
{
    NONCONST
    bool did_something = false;
    for (auto it1 = this->numerator.begin(); it1 != this->numerator.end(); ++it1)
    {
        for (auto it2 = this->denominator.begin(); it2 != this->denominator.end(); ++it2)
        {

            if (it1->get()->getId() == POLYNOMIAL && it2->get()->getId() == POLYNOMIAL)
            {
                try {
                    auto gcf = gcd(static_cast<Polynomial*>(it1->get()), static_cast<Polynomial*>(it2->get()));
                if (gcf == nullptr)
                    continue;
                //qDebug() << gcf->makeStringOfExpression();
                    auto it1_pol = static_cast<Polynomial*>(it1->get())->divide(gcf.get()).first;
                    if (it1_pol == nullptr)
                        continue;
                    abs_ex it1_expr (it1_pol.release());
                    it1->swap(it1_expr);
                    *it1 = it1->get()->downcast();
                    auto it2_pol = static_cast<Polynomial*>(it2->get())->divide(gcf.get()).first;
                    abs_ex it2_expr (it2_pol.release());
                    it2->swap(it2_expr);
                    *it2 = it2->get()->downcast();
                    did_something = true;
                }
                catch(Exception) {}
            }
        }
    }
    if (did_something)
        this->simplify();
   // this->reduceSameMembers();
}
/*std::unique_ptr<Fractal> Fractal::operator*(const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient * right->coefficient);
    for (auto &it : right->numerator)
        result->pushBackToNumerator(copy(it));
    for (auto &it : right->denominator)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator*(const Fractal & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient * right.coefficient);

    for (auto &it : right.numerator)
        result->pushBackToNumerator(copy(it));
    for (auto &it : right.denominator)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator/(const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient / right->coefficient);
    for (auto &it : right->numerator)
        result->pushBackToDenominator(copy(it));
    for (auto &it : right->denominator)
        result->pushBackToNumerator(copy(it));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator/(const Fractal & right) const
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient / right.coefficient);
    for (auto &it : right.numerator)
        result->pushBackToDenominator(copy(it));
    for (auto &it : right.denominator)
        result->pushBackToNumerator(copy(it));
    result->simplify();
    return result;
}*/
bool Fractal::canTurnIntoPolynomWithOpeningParentheses() const
{
    if (!this->denominator.empty() || this->coefficient.getDenominator() != 1)
        return false;
    auto set = this->getSetOfVariables();

    //if (!set.empty() && isIntegratingConstanThatCant(*--this->getSetOfVariables().end()))
     //       return false;
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL || (it->getId() == DEGREE && static_cast<Degree*>(it.get())->canTurnIntoPolynomial()))
            return true;
    }
    return false;
}

bool Fractal::canTurnIntoPolynomWithOpeningParentheses(bool is_fractional_coefficient_allowed)
{
    if (!this->denominator.empty() || (this->coefficient.getDenominator() != 1 && !is_fractional_coefficient_allowed))
        return false;
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL || (it->getId() == DEGREE && static_cast<Degree*>(it.get())->canTurnIntoPolynomial()))
            return true;
    }
    return false;
}
std::unique_ptr<Polynomial> Fractal::turnIntoPolynomWithOpeningParentheses(bool fractional_coefficients) const
{
    //in this function, if fractal has several polynoms, it has to do those operations with 1. another will turning is polynom->simplify() which call this function
    //assert(canTurnIntoPolynomWithOpeningParentheses());
  //  qDebug() << this->makeStringOfExpression();
    std::unique_ptr<Fractal> frac_without_polynom = std::unique_ptr<Fractal>(new Fractal(*this));
    frac_without_polynom->turnDegreesIntoList();
    std::unique_ptr<Polynomial> polynom;
    for (auto it = frac_without_polynom->numerator.begin(); it != frac_without_polynom->numerator.end(); ++it)
    {
        if (it->get()->getId() == POLYNOMIAL)
        {
            polynom = std::unique_ptr<Polynomial>(new Polynomial(*static_cast<Polynomial*>(it->get())));
            frac_without_polynom->numerator.erase(it);
            break;
        }

    }
 //   qDebug() << frac_without_polynom->makeStringOfExpression();
  //  qDebug() << "POLYNOMS_MONOMS: ";
 //   for (auto &it : polynom->getMonomialsPointers())
  //      qDebug() << "    " << it->makeStringOfExpression();
    //bool fractional_coefficients = polynom->isFractionalCoefficientsAllowed();
    std::list<Fractal*> polynoms_monoms = polynom->getMonomialsPointers();
    polynom.release();
    polynom = std::unique_ptr<Polynomial>(new Polynomial());

    polynom->setFractionalCoefficientsAllowed(fractional_coefficients);
    for (auto &it : polynoms_monoms)
    {
        polynom->addMonomial(std::move(*it) * *frac_without_polynom);
    }

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
                     auto degr = abs_ex(new Degree(copy(degree),
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
                     auto degr = abs_ex(new Degree(copy(degree),
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
Fractal::Fractal(abs_ex && num, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(std::move(num));
    this->simplify();
}

Fractal &Fractal::operator=(Fractal &&frac)
{
    this->numerator = std::move(frac.numerator);
    this->denominator = std::move(frac.denominator);
    this->coefficient = frac.coefficient;
    this->simplified = frac.simplified;
    this->simplify();
    return *this;
}
QString Fractal::makeStringOfExpression() const
{
    QString result = "(";
    if (!this->coefficient.isOne())
        result += this->coefficient.makeStringOfExpression();
    for (auto &it : this->numerator)
        if (it->getId() != POLYNOMIAL)
            result += "*" + it->makeStringOfExpression();
        else
            result += "(" + it->makeStringOfExpression() + ")";
    if (result.size() == 1)
        result += "1";
    for (auto &it : this->denominator)
        if (it->getId() != POLYNOMIAL)
            result += "/" + it->makeStringOfExpression();
        else
            result += "/(" + it->makeStringOfExpression() + ")";
    result += ")";
    if (result[1] == "*")
        result = result.remove(1, 1);
    return result;
}

QString Fractal::makeWolframString() const
{
    QString result ;
    if (!this->coefficient.isOne())
        result += this->coefficient.makeWolframString();
    for (auto &it : this->numerator)
        if (it->getId() != POLYNOMIAL)
            result += "*" + it->makeWolframString();
        else
            result += "(" + it->makeWolframString() + ")";
    if (result.size() == 0)
        result += "1";
    for (auto &it : this->denominator)
        if (it->getId() != POLYNOMIAL)
            result += "/" + it->makeWolframString();
        else
            result += "/(" + it->makeWolframString() + ")";
    if (result[0] == "*")
        result = result.remove(0, 1);
    return result;
}

QString Fractal::toString() const
{
    QString result ;
    if (!this->coefficient.isOne())
        result += this->coefficient.toString();
    for (auto &it : this->numerator)
        if (it->getId() != POLYNOMIAL)
            result += "*" + it->toString();
        else
            result += " (" + it->toString() + ") ";
    if (result.size() == 0)
        result += "1";
   // else if (numerator.size() > 0 && numerator.back()->getId() == POLYNOMIAL)
     //   result += ' ';
    for (auto &it : this->denominator)
        if (it->getId() != POLYNOMIAL)
            result += "/" + it->toString();
        else
            result += "/(" + it->toString() + ")";
    if (result[0] == "*")
        result = result.remove(0, 1);
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
                abs_ex deg_to_set;
                if (*deg1 == *deg2)
                    deg_to_set = std::move(deg1);
                else if (deg1->getId() == NUMBER && deg2->getId() == NUMBER)
                    deg_to_set = abs_ex(new Number(static_cast<Number*>(deg1.get())->compareWith(*static_cast<Number*>(deg2.get())) < 0 ?
                                                                                    *static_cast<Number*>(deg1.get()) : *static_cast<Number*>(deg2.get())));
                else if (deg1->getId() == NUMBER)
                    deg_to_set = std::move(deg1);
                else if (deg2->getId() == NUMBER)
                    deg_to_set = std::move(deg2);
                else
                {
                    auto sub_result = deg1 - deg2;
                    if (sub_result->getId() == NUMBER)
                    {
                        if (static_cast<Number*>(sub_result.get())->compareWith(0) > 0)
                            deg_to_set = std::move(deg1);
                        else
                            deg_to_set = std::move(deg2);
                    }
                    else
                    {
                        auto div_result = deg1 / deg2;
                        if (div_result->getId() == NUMBER)
                        {
                            if (static_cast<Number*>(div_result.get())->compareWith(1) > 0)
                                deg_to_set = std::move(deg1);
                            else
                                deg_to_set = std::move(deg2);
                        }
                        else //здесь уже пофиг
                            deg_to_set = std::move(deg1);
                    }
                }
                 if (deg_to_set->getId() == NUMBER && static_cast<Number*>(deg_to_set.get())->isOne())
                    result.push_back(copy(Degree::getArgumentOfDegree(it1.get())));
                else
                    result.push_back(abs_ex(new Degree(copy(Degree::getArgumentOfDegree(it1.get())), deg_to_set)));
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
abs_ex Fractal::findCommonPart(Fractal *frac)
{
    abs_ex result(new Fractal);
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
    abs_ex result(nullptr);
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
                abs_ex zero(new Number(0));
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

void Fractal::sortVariablesInIncreasingOfTheirId()
{
    NONCONST
    this->numerator.sort([](const abs_ex & a, const abs_ex & b)->bool{
        return Degree::getArgumentOfDegree(a.get())->getId() <  Degree::getArgumentOfDegree(b.get())->getId();
    });
}
void Fractal::pushBackToNumerator(abs_ex && expr)
{
    NONCONST
    this->numerator.push_back(std::move(expr));
}
void Fractal::pushBackToDenominator(abs_ex && expr)
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
            abs_ex mult_deg_pt(new Number(Number(new_den_deg)-deg));
            abs_ex den_deg(new Number(new_den_deg));
           // it = abs_ex (new Degree(abs_ex(
          //                                                            Degree::getArgumentOfDegree(it.get())),
         //                                                        std::move(den_deg)));
            AbstractExpression * arg = Degree::getArgumentOfDegree(it.get());
            it = abs_ex (new Degree(copy(arg),
                                                                std::move(den_deg)));
             arg = Degree::getArgumentOfDegree(it.get());
            this->numerator.push_back(abs_ex(new Degree(copy(arg),
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
    NONCONST
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

abs_ex Fractal::changeSomePartOn(QString part, abs_ex &on_what)
{
  //  NONCONST
    abs_ex its_part = nullptr;
    auto add = [&on_what, &its_part](abs_ex & part)->void {
        if (its_part == nullptr)
            its_part = std::move(part);
        part = copy(on_what);
    };
    auto add_r = [&its_part](abs_ex && part)->void {
        if (its_part == nullptr && part != nullptr)
            its_part = std::move(part);
    };
    for (auto &it : this->numerator)
    {
        if (it->makeStringOfExpression() == part)
        {
            add(it);

        }
        else
            add_r(it->changeSomePartOn(part, on_what));

    }
    for (auto &it : this->denominator)
    {
        if (it->makeStringOfExpression() == part)
        {
            add(it);

        }
        else
            add_r(it->changeSomePartOn(part, on_what));

    }
    return its_part;
}

abs_ex Fractal::changeSomePartOnExpression(QString part, abs_ex &on_what)
{
    NONCONST
           return changeSomePartOn(part, on_what);
}

std::vector<std::pair<abs_ex, Number>> Fractal::getTrigonometryMultipliersArgumentsCopyAndItsDegrees()
{
  //  assert(this->denominator.size() == 0);
    std::vector<std::pair<abs_ex, Number>> res;
    for (auto &it : this->numerator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            res.push_back({getArgumentOfFunction(it), *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())});

        }
    }
    for (auto &it : this->denominator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            res.push_back({getArgumentOfFunction(it), *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())});

        }
    }
    return res;
}

void Fractal::convertTrigonometricalMultipliersToDifferentArgument(const std::map<QString, TrigonometricalFunctionsArgumentsCastType> &instructions)
{

    for (auto &it : numerator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            NONCONST
            it = convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfFunction(it)->makeStringOfExpression())->second);
            //qDebug() << "RES: " << convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfTrigonometricalFunction(it)->makeStringOfExpression())->second)->makeStringOfExpression();
            //qDebug() << "IN FR: " << it->makeStringOfExpression();
        }

    }
    for (auto &it : denominator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            NONCONST
            it = convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfFunction(it)->makeStringOfExpression())->second);
            //qDebug() << "RES: " << convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfTrigonometricalFunction(it)->makeStringOfExpression())->second)->makeStringOfExpression();
            //qDebug() << "IN FR: " << it->makeStringOfExpression();
        }

    }
}

abs_ex Fractal::derivative(int var) const
{
    if (this->coefficient != 1)
    {
        abs_ex fr(new Fractal(*this));
        abs_ex coe(new Number(this->coefficient));
        fr = fr / coe;
        return coe * fr->derivative(var);
    }
    if (!this->denominator.empty())
    {
        abs_ex f(new Fractal(this->numerator, Number(this->coefficient.getNumerator())));
        f = f->downcast();
        abs_ex g(new Fractal(this->denominator, Number(this->coefficient.getDenominator())));
        g = g->downcast();
        return (f->derivative(var) * g - f * g->derivative(var)) / takeDegreeOf(g, 2);

    }
    if (this->numerator.size() == 1)
        return (*this->numerator.begin())->derivative(var);
    if (this->numerator.size() == 0)
        return copy(zero);
    assert(this->numerator.size() > 1);
    abs_ex f = copy(*this->numerator.begin());
    fractal_argument other_mults;
    for (auto it = std::next(this->numerator.begin()); it != this->numerator.end(); ++it)
    {
        other_mults.push_back(copy(*it));
    }
    abs_ex g (new Fractal(std::move(other_mults)));
    g = g->downcast();
    return f->derivative(var) * g + g->derivative(var) * f;
}
abs_ex integrateByParts(const abs_ex & u, const abs_ex & dv, int var)
{
    abs_ex v = dv->antiderivative(var);
    if (v == nullptr)
        return nullptr;
    abs_ex du = u->derivative(var);
    if (!has((u*dv / (v*du))->getSetOfVariables(), var))
        return nullptr;
    abs_ex int_v_du = integrate(v*du*D(abs_ex(new Variable(getVariable(var)))));
    if (int_v_du == nullptr)
        return nullptr;
   // qDebug() << u->toString();
    //qDebug() << v->toString();
   // qDebug() << int_v_du->toString();
   // qDebug() << (u*v)->toString();
   // qDebug() << (u*v - int_v_du)->toString();
    return u*v - int_v_du;
}
abs_ex Fractal::getAntiderivativeByParts(int var) const
{
    //множители, в которых нет переменной var, тут не должны присутствовать

    Fractal* cop = new Fractal(*this);
    abs_ex cop_p(cop);
    //произведение логарифма на остальное
    //qDebug() << this->makeStringOfExpression();
    for (auto it = cop->numerator.begin(); it != cop->numerator.end(); ++it)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == LOGARITHM)
        {
            abs_ex u = std::move(*it);
            cop->numerator.erase(it);
            return integrateByParts(u, cop_p->downcast(), var);
        }
    }

    //произведение многочлена на экспоненциальную/тригонометрическую/арктригонометрическую функцию
    bool has_expon_or_trig_or_arc = false;
    abs_ex full_polynom = copy(one);
    for (auto it = cop->numerator.begin(); it != cop->numerator.end();)
    {
        if (checkIfItsPolynom(*it, var).size() > 0)
        {
            full_polynom = std::move(full_polynom) * std::move(*it);
            it = cop->numerator.erase(it);
        }
        else
        {

            if (isDegreeOfTrigonometricalFunction(*it) || isDegreeOfArcTrigonometricalFunction(*it) ||
                    isExponentialFunction(*it, var))
                has_expon_or_trig_or_arc = true;
            ++it;
        }
    }
    //интеграл многочлена деленного на тригонометрию/обратную тригонометрию в элементарных
    //функциях не выражается, а вот экспоненциальная спокойно берется
    for (auto &it : cop->denominator)
        if (isExponentialFunction(it, var))
        {
            has_expon_or_trig_or_arc = true;
            break;
        }
    if (has_expon_or_trig_or_arc && full_polynom->getId() != NUMBER)
        return integrateByParts(full_polynom, cop_p->downcast(), var);
    return nullptr;
}

abs_ex Fractal::getAntiderivativeOfLogarithmFunctionDividedOnX(int var) const
{
    abs_ex linear_function = nullptr;
    std::pair<abs_ex, abs_ex> ln_f;
    for (auto &it : this->denominator)
    {
        ln_f = ::checkIfItsLinearFunction(it, var);
        if (ln_f.first != nullptr)
        {
            linear_function = copy(it);
            break;
        }
    }
    if (linear_function == nullptr)
        return nullptr;
    abs_ex rest_of_frac = copy(this) * linear_function;
    abs_ex t = systemVarExpr();
   // qDebug() << linear_function->makeStringOfExpression();
    //qDebug() << rest_of_frac->makeStringOfExpression();
    setUpExpressionIntoVariable(rest_of_frac, (pow(getEuler(), t) - ln_f.second)/ln_f.first,var);
   // qDebug() << rest_of_frac->makeStringOfExpression();
    auto integr = rest_of_frac->antiderivative(t->getId());
    if (integr == nullptr)
        return nullptr;
   // qDebug() << integr->makeStringOfExpression();
    setUpExpressionIntoVariable(integr, ln(linear_function), t->getId());
   // qDebug() << integr->makeStringOfExpression();
    return integr/ln_f.first;
}

abs_ex Fractal::getAntiderivativeOfFunctionWithNonOneGcdOfVarExceptVarMultiplier(int var) const
{
    abs_ex var_multiplier = nullptr;
    Fractal cop = *this;
    for (auto it = cop.numerator.begin(); it != cop.numerator.end(); ++it)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == var)
        {
            if (Degree::getDegreeOfExpression(it->get())->getId() != NUMBER ||
                    !static_cast<Number*>(Degree::getDegreeOfExpression(it->get()).get())->isInteger())
                return nullptr;
            var_multiplier = std::move(*it);
            cop.numerator.erase(it);
            break;
        }
    }
    if (var_multiplier == nullptr)
        return nullptr;
 //   qDebug() << cop.toString();
    int gcd_of_nums = cop.getGcdOfNumeratorsOfDegrees(var);
    if (gcd_of_nums == 1)
        return nullptr;
    int deg = static_cast<Number*>(Degree::getDegreeOfExpression(var_multiplier.get()).get())->getNumerator();
    if (gcd_of_nums > deg + 1)
    {
        auto facts = factorize(gcd_of_nums);
        int deg_of_change = 0;
        for (auto rit = facts.rbegin(); rit != facts.rend(); ++rit)
            if (rit->first <= deg + 1)
            {
                deg_of_change = rit->first;
                break;
            }
        if (deg_of_change <= 1)
            return nullptr;
        abs_ex frac_to_integr = copy(&cop);
        abs_ex t = systemVarExpr();
        setUpExpressionIntoVariable(frac_to_integr, pow(t, one/numToAbs(deg_of_change)), var);
        abs_ex integr = frac_to_integr->antiderivative(t->getId());
        if (integr == nullptr)
            return nullptr;
        setUpExpressionIntoVariable(integr, pow(getVariableExpr(var), numToAbs(deg_of_change)), t->getId());
        return integr/numToAbs(deg_of_change);
    }
    if (gcd_of_nums == deg + 1)
    {
        abs_ex frac_to_integr = copy(&cop);
        abs_ex t = systemVarExpr();
        setUpExpressionIntoVariable(frac_to_integr, pow(t, one/numToAbs(deg + 1)), var);
        abs_ex integr = frac_to_integr->antiderivative(t->getId());
        if (integr == nullptr)
            return nullptr;
        setUpExpressionIntoVariable(integr, pow(getVariableExpr(var), numToAbs(deg + 1)), t->getId());
        return integr/numToAbs(deg + 1);
    }
    if ((deg - (gcd_of_nums - 1)) % gcd_of_nums == 0)
    {
        abs_ex frac_to_integr = copy(&cop);
        abs_ex t = systemVarExpr();
        setUpExpressionIntoVariable(frac_to_integr, pow(t, one/numToAbs(gcd_of_nums )), var);
        frac_to_integr = std::move(frac_to_integr) * pow(t, (deg - (gcd_of_nums - 1))/gcd_of_nums);
        abs_ex integr = frac_to_integr->antiderivative(t->getId());
        if (integr == nullptr)
            return nullptr;
        setUpExpressionIntoVariable(integr, pow(getVariableExpr(var), numToAbs(gcd_of_nums )), t->getId());
        return integr/numToAbs(gcd_of_nums);
    }
    auto facts = factorize(gcd_of_nums);
    for (auto rit = facts.rbegin(); rit != facts.rend(); ++rit)
    {
        if ((deg - (rit->first - 1)) % rit->first == 0)
        {
            abs_ex frac_to_integr = copy(&cop);
            abs_ex t = systemVarExpr();
            setUpExpressionIntoVariable(frac_to_integr, pow(t, one/numToAbs(rit->first )), var);
            frac_to_integr = std::move(frac_to_integr) * pow(t, (deg - (rit->first - 1))/rit->first);
            abs_ex integr = frac_to_integr->antiderivative(t->getId());
            if (integr == nullptr)
                return nullptr;
            setUpExpressionIntoVariable(integr, pow(getVariableExpr(var), numToAbs(rit->first )), t->getId());
            return integr/numToAbs(rit->first );
        }
    }
    return nullptr;
}

abs_ex Fractal::getAntiderivariveByBriningFunctionIntoDifferential(int var) const
{
    //сейчас рассматриваем ситуацию 1/(ax+b) * f(ln(ax+b)) либо 1/x * f(ln(x))
    abs_ex res = getAntiderivativeOfLogarithmFunctionDividedOnX(var);
    if (res != nullptr)
        return res;
    //рассмотрим функцию по типу x/(x^4 + x^2 + 1)
    res = getAntiderivativeOfFunctionWithNonOneGcdOfVarExceptVarMultiplier(var);
    if (res != nullptr)
        return res;
    return nullptr;
}

abs_ex Fractal::getAntiderivativeOfIrrationalFunction(int var) const
{
    //сначала чекаем на корень какой-либо степени от линейной функции
    abs_ex res = getAntiderivativeOfFunctionWithRootOfLinearFunction(var);
    if (res != nullptr)
        return res;

    return nullptr;
}

abs_ex Fractal::getAntiderivativeOfFunctionWithRootOfLinearFunction(int var) const
{
    std::pair<abs_ex, abs_ex> ln_f;
    abs_ex linear_func;
    int root;
    for (auto &it : this->numerator)
    {
        if (it->getId() == DEGREE)
        {
            auto deg = Degree::getDegreeOfExpression(it.get());
            if (deg->getId() == NUMBER)
            {
                Number num = *static_cast<Number*>(deg.get());
                if (num.getDenominator() == 1)
                    continue;
                ln_f = ::checkIfItsLinearFunction(Degree::getArgumentOfDegree(it.get()), var);
                if (ln_f.first == nullptr)
                    continue;
                linear_func = copy(Degree::getArgumentOfDegree(it.get()));
                root = num.getDenominator();
                break;
            }
        }
    }
    if (linear_func != nullptr)
        for (auto &it : this->denominator)
        {
            if (it->getId() == DEGREE)
            {
                auto deg = Degree::getDegreeOfExpression(it.get());
                if (deg->getId() == NUMBER)
                {
                    Number num = *static_cast<Number*>(deg.get());
                    if (num.getDenominator() == 1)
                        continue;
                    ln_f = ::checkIfItsLinearFunction(Degree::getArgumentOfDegree(it.get()), var);
                    if (ln_f.first == nullptr)
                        continue;
                    linear_func = copy(Degree::getArgumentOfDegree(it.get()));
                    root = num.getDenominator();
                    break;
                }
            }
        }
    if (linear_func == nullptr)
        return nullptr;
    abs_ex t;
    if (root % 2 == 0)
        t = systemVarExpr(zero, nullptr, true, false);
    else
        t = systemVarExpr();
    abs_ex new_x = (pow(t, root) - ln_f.second)/ln_f.first;
    abs_ex new_frac = copy(this);
   // qDebug() << new_frac->makeStringOfExpression();
   // qDebug() << new_x->makeStringOfExpression();
    setUpExpressionIntoVariable(new_frac, new_x, var);
    new_frac = new_frac * new_x->derivative(t->getId());
   // qDebug() << new_frac->makeStringOfExpression();
    abs_ex integr = new_frac->antiderivative(t->getId());
    if (integr == nullptr)
        return nullptr;
   // qDebug() << integr->makeStringOfExpression();
    setUpExpressionIntoVariable(integr, pow(linear_func, Number(1, root)), t->getId());
    return integr;
}

abs_ex Fractal::getAntiderivativeOfFunctionThatHaveExponentialExpression(int var) const
{
    abs_ex expon = this->tryToFindExponentialFunction(var);
    //qDebug() << expon->toString();
    if (expon == nullptr)
        return nullptr;
    abs_ex base = copy(Degree::getArgumentOfDegree(expon.get()));
    auto ln_f = ::checkIfItsLinearFunction(Degree::getDegreeOfExpression(expon.get()), var);
    assert(ln_f.first != nullptr);

    abs_ex t = systemVarExpr(zero, nullptr, false, false);
    abs_ex x_change = (ln(t)/ln(base) - ln_f.second)/ln_f.first;
    abs_ex frac_to_integr = copy(this);
   // qDebug() << x_change->toString();
   // qDebug() << frac_to_integr->toString();
    setUpExpressionIntoVariable(frac_to_integr, x_change, var);
    // qDebug() << frac_to_integr->toString();
    frac_to_integr = std::move(frac_to_integr) * x_change->derivative(t->getId());
  //  qDebug() << frac_to_integr->toString();
    auto integr = frac_to_integr->antiderivative(t->getId());
    if (integr == nullptr)
        return nullptr;
    //qDebug() << integr->toString();
    setUpExpressionIntoVariable(integr, expon, t->getId());
  //  qDebug() << integr->toString();
    return integr;
}

//да, это будет очередная большая функция. Здесь нет сложного поведения, только проверка на различные табличные интегралы
//рассчитываем на то, что сработало takeCommonPartOfPolynomial()
//еще рассчитываем на то, что все полиномы которые можно разложить - разложены

abs_ex Fractal::antiderivative(int var) const
{
   // qDebug() << this->makeStringOfExpression();
    if (!has(this->getSetOfVariables(), var))
        return abs_ex(new Variable(getVariable(var))) * copy(this);
    auto fr_without_var = this->getFractalWithoutVariable(var);
    if (!fr_without_var->isOne())
    {
        auto integr = (*this / *fr_without_var)->downcast()->antiderivative(var);
        if (integr == nullptr)
            return nullptr;
        return toAbsEx(fr_without_var) * integr;
    }
    if (this->canTurnIntoPolynomWithOpeningParentheses())
    {
        return this->turnIntoPolynomWithOpeningParentheses(false)->antiderivative(var);
    }
    auto integ = this->tableAntiderivative(var);
    if (integ != nullptr)
        return integ;
    abs_ex x(new Variable(getVariable(var)));
    if (checkIfItsRationalFunction(var))
    {
        if (this->isNumeratorsDegreeBiggerThanDenominatorsDegree(var))
        {
            Fractal cop = *this;
            cop.expandNumerator();
            cop.expandDenominator();
            auto num = toPolynomialPointer(*cop.numerator.begin());
            auto denom = toPolynomialPointer(*cop.denominator.begin());
            auto div_res = num->divide(denom.get());
            if (div_res.first != nullptr)
            {
                auto integr_one = div_res.first->antiderivative(var);
                auto integr_two = (copy(div_res.second.get()) / *cop.denominator.begin())->antiderivative(var);
                if (integr_one != nullptr && integr_two != nullptr)
                    return integr_one + integr_two;
            }
        }
        Fractal cop = *this;
        int prev_num_size = cop.numerator.size();
        cop.bringRationalFunctionIntoFormToDecay();
        int current_num_size = cop.numerator.size();
        if (cop.denominator.size() > 1 || (cop.denominator.size() == 1 && cop.denominator.begin()->get()->getId() ==
                                           DEGREE && cop.numerator.size() == 1 &&
                                           cop.numerator.begin()->get()->getMaxDegreeOfVariable(var).compareWith(
                                               Degree::getArgumentOfDegree(cop.denominator.begin()->get())
                                               ->getMaxDegreeOfVariable(var)) >= 0 &&
                                          Degree::getArgumentOfDegree(cop.denominator.begin()->get())->getMaxDegreeOfVariable(var) != 1))
        {
           // qDebug() << cop.toString();
            auto split_res = cop.splitIntoSumOfElementaryFractals(var);
            auto integral_res = copy(zero);
            for (auto &it : split_res)
            {
              //  qDebug() << it->makeStringOfExpression();
                auto integr = it->antiderivative(var);
                if (integr == nullptr)
                    return nullptr;
                integral_res = integral_res + integr;
            }
            return integral_res;
        }
        else if (prev_num_size > current_num_size)
            return cop.antiderivative(var);
    }
    else
    {
        Fractal cop = *this;
        cop.expandNumerator();
        if (cop.numerator.size() == 1 && cop.numerator.begin()->get()->getId() == POLYNOMIAL)
        {
            auto monoms = static_cast<Polynomial*>(cop.numerator.begin()->get())->getMonoms();
            abs_ex result = copy(zero);
            fractal_argument empty;
            for (auto &it : *monoms)
            {
                abs_ex its_result = (toAbsEx(it)/abs_ex(new Fractal(&empty, &this->denominator)))->antiderivative(var);
                if (its_result == nullptr)
                    return nullptr;
                result = std::move(result) + std::move(its_result);
            }

        }
    }
    //пробуем интегрирование по частям
    auto p_int = this->getAntiderivativeByParts(var);
    if (p_int != nullptr)
        return p_int;
    auto ds_int = this->getAntiderivariveByBriningFunctionIntoDifferential(var);
    if (ds_int != nullptr)
        return ds_int;
    auto irrat_arg = this->getAntiderivativeOfIrrationalFunction(var);
    if (irrat_arg != nullptr)
        return irrat_arg;
    auto expon_arg = this->getAntiderivativeOfFunctionThatHaveExponentialExpression(var);
    if (expon_arg != nullptr)
        return expon_arg;
    auto trig_arg = this->checkIfCanDoUniversalTrigonometricSubstitution(var);
    if (trig_arg.first != nullptr)
    {
        auto arg = trig_arg.first * x + std::move(trig_arg.second);
        abs_ex cop (new Fractal(*this));
        abs_ex t (new Variable(systemVar()));
        abs_ex sinus_change = two*t/(one + t*t);
        cop->changeSomePartOnExpression(sin(arg)->makeStringOfExpression(), sinus_change);
        //qDebug() << cop->makeStringOfExpression();
        abs_ex cosinus_change = (one - t*t)/(one + t*t);
        cop->changeSomePartOnExpression(cos(arg)->makeStringOfExpression(), cosinus_change);
       // qDebug() << cop->makeStringOfExpression();
        cop->changeSomePartOnExpression(tan(arg/two)->makeStringOfExpression(), t);
        abs_ex cotangent_change = one/t;
        cop->changeSomePartOnExpression(cot(arg/two)->makeStringOfExpression(), cotangent_change);
        cop->setSimplified(false);

        if (!has(cop->getSetOfVariables(), var))
        {
            cop = cop * two/(one + t*t);
       // qDebug() << cop->makeStringOfExpression();
            auto res =  one/trig_arg.first * cop->antiderivative(t->getId());
            if (res != nullptr)
            {
                std::map<int, abs_ex> funcs;
                funcs.insert({t->getId(), tan(arg/two)});
                replaceSystemVariablesToExpressions(res, funcs);
                return res;
            }
        }
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
        auto integr = cop->antiderivative(t->getId());
        if (integr != nullptr)
        {
            setUpExpressionIntoVariable(integr, pow(x, one/numToAbs(lcm_of_denoms)), t->getId());
            return integr;
        }

    }
    return nullptr;
}

std::pair<abs_ex, abs_ex> Fractal::checkIfItIsLinearFunction(int var) const
{
    bool has_first_deg_var = false;
    bool has_other_deg_var = false;
    for (auto &it : numerator)
    {
        if (Degree::getArgumentOfDegree(it.get())->getId() != var)
            continue;
        auto degr = Degree::getDegreeOfExpression(it.get());
        if (*degr == *one)
            has_first_deg_var = true;
        else
        {
            has_other_deg_var = true;
            break;
        }
    }
    for (auto &it : denominator)
        if (Degree::getArgumentOfDegree(it.get())->getId() == var)
        {
            has_other_deg_var = true;
            break;
        }
    if (has_other_deg_var || !has_first_deg_var)
        return {nullptr, nullptr};
    return {toAbsEx(this->getFractalWithoutVariable(var)), copy(zero)};
}

bool Fractal::isOne() const
{
    return this->numerator.empty() && this->denominator.empty() && this->coefficient.isOne();
}

bool Fractal::checkIfItsRationalFunction(int var) const
{
    for (auto &it : this->numerator)
    {
        auto deg = Degree::getDegreeOfExpression(it.get());
        if (deg->getId() != NUMBER)
            return false;
        if (!static_cast<Number*>(deg.get())->isInteger())
            return false;
        auto pol = ::checkIfItsPolynom(Degree::getArgumentOfDegree(it.get()), var);
        if (pol.size() == 0)
            return false;
    }
    for (auto &it : this->denominator)
    {
        auto deg = Degree::getDegreeOfExpression(it.get());
        if (deg->getId() != NUMBER)
            return false;
        if (!static_cast<Number*>(deg.get())->isInteger())
            return false;
        auto pol = ::checkIfItsPolynom(Degree::getArgumentOfDegree(it.get()), var);
        if (pol.size() == 0)
            return false;
    }
    return true;
}

void Fractal::expandNumerator()
{
    abs_ex new_num = abs_ex(new Fractal(this->getFractal().first)) + zero;
    this->numerator.clear();
    this->numerator.push_back(std::move(new_num));
}

void Fractal::expandDenominator()
{
    abs_ex new_num = abs_ex(new Fractal(this->getFractal().second)) + zero;
    this->denominator.clear();
    this->denominator.push_back(std::move(new_num));
}

void Fractal::factorizeDenominator()
{
    std::list<abs_ex> new_denominator;
    for (auto it = this->denominator.begin(); it != this->denominator.end(); ++it)
    {
        auto deg = Degree::getDegreeOfExpression(it->get());
        auto factorize_result =
                factorizePolynom(copy(Degree::getArgumentOfDegree(it->get())));

        for (auto &it : factorize_result.first)
            new_denominator.push_back(pow(it, deg));
        this->coefficient /= factorize_result.second;
    }
    this->denominator = std::move(new_denominator);
}

void Fractal::bringRationalFunctionIntoFormToDecay()
{
    this->expandNumerator();
    this->factorizeDenominator();
    this->intoAcceptedSignForm();
    this->setSameMembersIntoDegree();
}

std::list<abs_ex > Fractal::splitIntoSumOfElementaryFractals(int var)
{
  //  qDebug() << this->makeStringOfExpression();
    //это должна быть рациональная функция, но я не хочу вставлять assert
    abs_ex numerator_expression = copy(zero);
    std::vector<int> uknown_numerators_variables;
    abs_ex denominator_expression(new Fractal(this->denominator));

    for (auto &it : this->denominator)
    {
        int deg = static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->getNumerator();
        int denum_deg = Degree::getArgumentOfDegree(it.get())
                ->getMaxDegreeOfVariable(var).getNumerator();
        abs_ex x(new Variable(getVariable(var)));
        for (int i = 1; i <= deg; ++i)
        {
            abs_ex its_num = copy(zero);
            for (int j = 0; j < denum_deg; ++j)
            {
                abs_ex its_var(new Variable(systemVar()));
                uknown_numerators_variables.push_back(its_var->getId());
                its_num = its_num + its_var * pow(x, j);
            }

            numerator_expression = numerator_expression + its_num * (denominator_expression / pow(copy(Degree::getArgumentOfDegree(it.get())), i));
        }
    }
   // qDebug() << this->numerator.begin()->get()->makeStringOfExpression();
    auto numerator_coefs = checkIfItsPolynom(*this->numerator.begin(), var);



    auto equation_vectors = checkIfItsPolynom(numerator_expression, var);



    assert(equation_vectors.size() >= numerator_coefs.size());
    int old_size = numerator_coefs.size();
    numerator_coefs.resize(equation_vectors.size());
    for (int i = old_size; i < numerator_coefs.size(); ++i)
        numerator_coefs[i] = copy(zero);
    for (int i = 0; i < equation_vectors.size(); ++i)
        equation_vectors[i] = std::move(equation_vectors[i]) - std::move(numerator_coefs[i]);
    auto equations_result = solveSystemOfEquations(equation_vectors, uknown_numerators_variables);

    std::list<abs_ex> result;
    int equations_result_index = 0;
    for (auto &it : this->denominator)
    {
        int deg = static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->getNumerator();
        int denum_deg = Degree::getArgumentOfDegree(it.get())
                ->getMaxDegreeOfVariable(var).getNumerator();
        abs_ex x(new Variable(getVariable(var)));
        for (int i = 1; i <= deg; ++i)
        {
            abs_ex its_num = copy(zero);
            for (int j = 0; j < denum_deg; ++j)
            {
                its_num = its_num + std::move(*equations_result[equations_result_index].begin()) * pow(x, j);
                ++equations_result_index;
            }
            result.push_back(its_num /pow(copy(Degree::getArgumentOfDegree(it.get())), i));
        }
    }

    for (auto &it : result)
        it = it * abs_ex(new Number(this->coefficient));
    return result;
}

std::unique_ptr<Polynomial> Fractal::toPolynomWithFractionalCoefficients()
{
    std::unique_ptr<Polynomial> result(new Polynomial);
    result->setFractionalCoefficientsAllowed(true);
    std::unique_ptr<Fractal> num (new Fractal(&this->numerator, this->coefficient.getNumerator()));
    std::unique_ptr<Fractal> denom(new Fractal(&this->denominator, this->coefficient.getDenominator()));
    if (num->canTurnIntoPolynomWithOpeningParentheses())
        result = num->turnIntoPolynomWithOpeningParentheses(true);
    else
        result->addMonomial(std::move(num));
    auto res_monoms = result->getMonomialsPointers();
    for (auto &it : res_monoms)
        *it = std::move(*(*it / *denom));

    return result;
}

long long int Fractal::getLcmOfDenominatorsOfDegreesOfVariable(int var) const
{
    long long int res = 1;
    for (auto &it : this->numerator)
    {
        if (Degree::getArgumentOfDegree(it.get())->getId() == var)
        {
            auto deg = Degree::getDegreeOfExpression(it.get());
            if (deg->getId() == NUMBER)
                res = lcm(res, static_cast<Number*>(deg.get())->getDenominator());
            else
                return 0;
        }
        else
            res = lcm(res, it->getLcmOfDenominatorsOfDegreesOfVariable(var));
    }
    for (auto &it : this->denominator)
    {
        if (Degree::getArgumentOfDegree(it.get())->getId() == var)
        {
            auto deg = Degree::getDegreeOfExpression(it.get());
            if (deg->getId() == NUMBER)
                res = lcm(res, static_cast<Number*>(deg.get())->getDenominator());
            else
                return 0;
        }
        else
            res = lcm(res, it->getLcmOfDenominatorsOfDegreesOfVariable(var));
    }
    return res;
}

long long Fractal::getGcdOfNumeratorsOfDegrees(int var) const
{
    auto zgcd = [](long long a, long long b)->long long {
        if (a == 0)
            return b;
        if (b == 0)
            return a;
        return gcd(a, b);
    };
    long long int res = 0;
    for (auto &it : this->numerator)
        res = zgcd(res, it->getGcdOfNumeratorsOfDegrees(var));
    for (auto &it : this->denominator)
        res = zgcd(res, it->getGcdOfNumeratorsOfDegrees(var));
    return res;
}

void Fractal::setSimplified(bool simpl)
{
    this->simplified = simpl;
    this->coefficient.setSimplified(simpl);
    for (auto &it : this->numerator)
        it->setSimplified(simpl);
    for (auto &it : this->denominator)
        it->setSimplified(simpl);
}

std::set<abs_ex > Fractal::getTrigonometricalFunctions() const
{
    std::set<abs_ex> result;
    for (auto &it : this->numerator)
    {
        auto set = it->getTrigonometricalFunctions();
        for (auto &it1 : set)
            result.insert(copy(it1));
    }
    for (auto &it : this->denominator)
    {
        auto set = it->getTrigonometricalFunctions();
        for (auto &it1 : set)
            result.insert(copy(it1));
    }
    return result;
}

abs_ex Fractal::tableAntiderivative(int var) const
{
    abs_ex x(new Variable(getVariable(var)));
    if (numerator.size() == 0 && denominator.size() == 1)
    {
        // 1/(ax+b)
        auto ln_f = checkIfItsLinearFunction(*denominator.begin(), var);
        if (ln_f.first != nullptr)
            return one / ln_f.first * ln(abs(*denominator.begin()));
        if ((*denominator.begin())->getId() == SINUS)
        { // 1/sin(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Sinus*>(denominator.begin()->get())->getArgumentsCopy();
            return one / ln_f.first * ln(abs(tan(arg/two)));
        }
        if ((*denominator.begin())->getId() == COSINUS)
        { // 1/cos(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Cosinus*>(denominator.begin()->get())->getArgumentsCopy();
            return one / ln_f.first * ln(abs(tan(arg/two + getPi()/four)));
        }
        if ((*denominator.begin())->getId() == TANGENT)
        { // 1/tan(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Tangent*>(denominator.begin()->get())->getArgumentsCopy();
            return one / ln_f.first * ln(abs(sin(arg)));
        }
        if ((*denominator.begin())->getId() == COTANGENT)
        { // 1/ctg(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Cotangent*>(denominator.begin()->get())->getArgumentsCopy();
            return minus_one / ln_f.first * ln(abs(cos(arg)));
        }
        if ((*denominator.begin())->getId() == DEGREE)
        {
            auto deg = Degree::getDegreeOfExpression(denominator.begin()->get());
            auto arg = Degree::getArgumentOfDegree(denominator.begin()->get());
            if (!arg->hasVariable(var))
            { // 1/a^x
                ln_f = checkIfItsLinearFunction(deg, var);
                if (ln_f.first == nullptr)
                    return nullptr;
                return minus_one/ln_f.first * takeDegreeOf(copy(arg), (minus_one) * (deg)) / ln(arg);
            }
            if (deg->hasVariable(var))
                return nullptr;
            if (*deg == *two)
            {
                if (arg->getId() == SINUS)
                {// 1/sin(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    return minus_one / ln_f.first * cot(copy(static_cast<Sinus*>(arg)->getArgumentsCopy()));
                }
                if (arg->getId() == COSINUS)
                {//  1/cos(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    return one/ln_f.first * tan(copy(static_cast<Cosinus*>(arg)->getArgumentsCopy()));
                }
                if (arg->getId() == TANGENT)
                {// 1/tan(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    auto x = static_cast<Tangent*>(arg)->getArgumentsCopy();
                    return one/ln_f.first * minus_one * (x + cot(x));
                }
                if (arg->getId() == COTANGENT)
                {// 1/cot(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    auto x = static_cast<Tangent*>(arg)->getArgumentsCopy();
                    return one/ln_f.first * (tan(x) - x);
                }
                auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                if (qc_f[0] != nullptr)
                {// 1/(ax^2 + bx+c)^2

                    auto D = sqr(qc_f[1]) - four * qc_f[0]*qc_f[2];
                    if (*D == *zero)
                    {
                        auto t = -qc_f[1]/two/qc_f[0];
                        return one/three/qc_f[0]/pow(t - x, 3);
                    }
                    if (D->getPositionRelativelyZero() > 0)
                    {// 1/a/(x-w)^2/(x-u)^2
                        auto w = (-qc_f[1] + sqrt(D))/two/qc_f[0];
                        auto u = (-qc_f[1] - sqrt(D))/two/qc_f[0];
                        return ((u - w)/(u - x) + (u - w)/(w - x) -two*ln(abs(u - x)) + two*ln(abs(w - x)))
                                /qc_f[0]/pow(u - w, 3);
                    }
                    auto d = arg->derivative(var);
                    return -(d / copy(arg) + four*qc_f[0] * atan(d/sqrt(-D))/sqrt(-D)) / D;
                }


            }

            if (*deg == *three)
            {
                auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                if (qc_f[0] != nullptr)
                {
                    auto D = sqr(qc_f[1]) - four * qc_f[0]*qc_f[2];
                    if (*D == *zero)
                    {
                        auto t = -qc_f[1]/two/qc_f[0];
                        return one/qc_f[0]/numToAbs(5)/pow(t - x, 5);
                    }
                    if (D->getPositionRelativelyZero() > 0)
                    {
                        auto w = (-qc_f[1] + sqrt(D))/two/qc_f[0];
                        auto u = (-qc_f[1] - sqrt(D))/two/qc_f[0];
                        auto u_w = u - w;
                        auto x_u = x - u;
                        auto x_w = x - w;
                        return (-sqr(u_w)/sqr(x_u) + sqr(u_w)/sqr(x_w) + numToAbs(6)*u_w/x_u + numToAbs(6)*u_w/x_w +
                                numToAbs(12)*ln(x_u) - numToAbs(12)*ln(x_w))/two/qc_f[0]/pow(u_w, 5);
                    }
                    abs_ex &a = qc_f[0];
                    abs_ex &b = qc_f[1];
                    abs_ex &c = qc_f[2];
                    return (-(((b + two* a* x)* (sqr(b) - numToAbs(6)* a* b* x - two* a* (numToAbs(5)* c + three* a *sqr(x))))/sqr(c +
                                                                                   x* (b + a *x))) + (
                                                                                numToAbs(24)* sqr(a)* atan((b + two* a* x)/sqrt(-D)))/sqrt(-D))/(two* sqr(D));
                }

            }
            if (deg->getId() == NUMBER && static_cast<Number*>(deg.get())->isInteger())
            {
                //ну и deg > 0 по определению, иначе быть не может
                if (arg->getId() == SINUS)
                {
                    auto ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    auto sin_arg = getArgumentOfFunction(arg);
                    //перед рекурсивным интегралом мы домножаем на ln_f.first, т. к. в нем уже как бы сразу da
                    //и поэтому нельзя, чтобы там повторно выносилось 1/a(что будет делаться), вот и компенсируем домножением
                    return one/ln_f.first * (-one/(deg - one) * cos(sin_arg)/pow(copy(arg), deg - one) +
                                             (deg - two)/(deg - one)*ln_f.first*(one/pow(copy(arg), deg-two))->antiderivative(var));
                }
                if (arg->getId() == COSINUS)
                {
                    auto ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    auto cos_arg = getArgumentOfFunction(arg);
                    //перед рекурсивным интегралом мы домножаем на ln_f.first, т. к. в нем уже как бы сразу da
                    //и поэтому нельзя, чтобы там повторно выносилось 1/a(что будет делаться), вот и компенсируем домножением
                    //я кстати не совсем уверен в этой формуле
                    //return one/ln_f.first * (one/(deg - one) * sin(cos_arg)/pow(copy(arg), deg - one) +
                      //                       (deg - two)/(deg - one)*ln_f.first*(one/pow(copy(arg), deg-two))->antiderivative(var));
                    return sin(cos_arg)/(ln_f.first * (deg - one) *pow(copy(arg), deg - one)) + (deg-two)/(deg - one) * (one/pow(copy(arg), deg - two))->antiderivative(var);
                }
                if (arg->getId() == TANGENT)
                    return pow(cot(getArgumentOfFunction(arg)), deg)->antiderivative(var);
                if (arg->getId() == COTANGENT)
                    return pow(tan(getArgumentOfFunction(arg)), deg)->antiderivative(var);
                auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                if (qc_f[0] != nullptr)
                {//  1/(ax^2+bx+c)^n == 1/((sqrt(a)x+ b/sqrt(a)/2)^2 + c - b^2/(2a))^n
                    abs_ex & k = deg;
                    if (*qc_f[0] == *one && *qc_f[1] == *zero)
                    {
                        if (qc_f[2]->getPositionRelativelyZero() >= 0)
                            return x/(two*qc_f[2] * (k - one) * pow(copy(arg), k - one)) + (two*k - three)/(two*qc_f[2]*(k - one)) * (one/pow(copy(arg), k - one))->antiderivative(var);
                        else
                            return nullptr;
                    }
                    abs_ex t (new Variable(systemVar()));
                    //если c-b^2/2/a окажется меньше нуля, то в вызове рекурсивного интеграла произойдет
                    //ассерт. Однако, если оно меньше нуля, то этот квадратный трехчлен можно разложить на множители,
                    //что должно было произойти, и тогда сюда мы не придем
                    abs_ex pre_int = one / sqrt(qc_f[0]) *
                            (one/pow(sqr(t) + qc_f[2] - sqr(qc_f[1])/(two*qc_f[0]), deg))->antiderivative(t->getId());
                    std::map<int, abs_ex> t_repl;
                    t_repl.insert({t->getId(), sqrt(qc_f[0])*x + qc_f[1]/sqrt(qc_f[0])/two});
                    replaceSystemVariablesToExpressions(pre_int, t_repl);
                    return pre_int;
                }
            }
            ln_f = checkIfItsLinearFunction(arg, var);
            if (ln_f.first != nullptr)
            { // 1/x^n
                return one/ln_f.first * takeDegreeOf(copy(arg), one - copy(deg)) / (one - copy(deg));
            }
            if (deg->getId() != NUMBER)
                return nullptr;

            Number num_d = *static_cast<Number*>(deg.get());
            if (num_d == Number(1, 2))
            {
                auto ln_f = checkIfItsLinearFunction(arg, var);
                if (ln_f.first != nullptr)
                {
                    return two/ln_f.first*sqrt(copy(arg));
                }
                auto qc_f = checkIfItsQuadraticFunction(arg, var);
                abs_ex x (new Variable(getVariable(var)));
                return one / takeDegreeOf(qc_f[0], half) * ln(abs(two*qc_f[0]*x + qc_f[1] + two*takeDegreeOf(qc_f[0]*copy(arg), half)));
            }

        }
        if ((*denominator.begin())->getId() == POLYNOMIAL)
        {
            auto arg = denominator.begin()->get();
            auto qc_f = checkIfItsQuadraticFunction(arg, var);
            if (qc_f[0] == nullptr)
                return nullptr;
            auto D = qc_f[1]*qc_f[1] - four*qc_f[0]*qc_f[2];
            abs_ex x (new Variable(getVariable(var)));
            if (::isZero(D))
                return minus_one * two / (two*qc_f[0]*x + qc_f[1]);
            //здесь воспользуемся формулами: dx/(x+w)/(x+u) -> 1/(w-u)*ln(|(x+u)/(x+w)|), ax^2+bx+c = a(x-x1)(x-x2), x1,2 = (-b+-sqrt(D))/2a
            int pos = D->getPositionRelativelyZero();
            if (pos > 0)
            {
                auto w = -(-qc_f[1] + sqrt(D))/(two*qc_f[0]);
                auto u = -(-qc_f[1] - sqrt(D))/(two*qc_f[0]);
               // qDebug() << D->makeStringOfExpression();
                //qDebug() << w->makeStringOfExpression();
               // qDebug() << u->makeStringOfExpression();
                return (one / (w - u) * ln(abs((x + u) / (x + w)))) / qc_f[0];
            }
            //if (pos < 0)
                return two * atan((two*qc_f[0]*x + qc_f[1])/sqrt(-D))/sqrt(-D);
            return nullptr;
        }
        return nullptr;
    }
    auto linearDivideQuadraticFormula = [&var](std::pair<abs_ex, abs_ex> & ln_f, std::array<abs_ex, 3> & qc_f2)->abs_ex {
        auto D = qc_f2[1]*qc_f2[1] - four*qc_f2[0]*qc_f2[2];
        abs_ex x (new Variable(getVariable(var)));
        if (::isZero(D)) // (ax+b)/(x+c)^2
        {
            //c это МИНУС КОРЕНЬ
            auto c = qc_f2[1]/two/qc_f2[0];
            return one / qc_f2[0] * ((ln_f.first*c - ln_f.second)/(x + c) + ln_f.first * ln(abs(c + x)));
        }
        //здесь воспользуемся формулами: dx/(x+w)/(x+u) -> 1/(w-u)*ln(|(x+u)/(x+w)|), ax^2+bx+c = a(x-x1)(x-x2), x1,2 = (-b+-sqrt(D))/2a
        int pos = D->getPositionRelativelyZero();
        if (pos > 0)
        {
            auto w = -(-qc_f2[1] + sqrt(D))/(two*qc_f2[0]);
            auto u = -(-qc_f2[1] - sqrt(D))/(two*qc_f2[0]);
            return one / qc_f2[0] * ((ln_f.first * u - ln_f.second) * ln(abs(u + x)) + (ln_f.second - ln_f.first * w) * ln(abs(w + x))) / (u - w);
        }

            return ((two * (two * ln_f.second * qc_f2[0] - ln_f.first * qc_f2[1]) * atan((two*qc_f2[0]*x + qc_f2[1])/sqrt(-D)))/sqrt(-D) + ln_f.first*ln(abs(x*(qc_f2[0]*x + qc_f2[1]) + qc_f2[2]))) / two / qc_f2[0];
    };
    auto quadraticDivideLinearFormula = [&var](std::array<abs_ex, 3> & qc_f, std::pair<abs_ex, abs_ex> & ln_f2)->abs_ex {
       /* auto D = qc_f[1]*qc_f[1] - four*qc_f[0]*qc_f[2];
        abs_ex x (new Variable(getVariable(var)));
        if (::isZero(D)) // (x-c)^2/(ax+b)
        {
            auto c = -qc_f[1]/two/qc_f[0];
            return qc_f[0] * (ln_f2.first * x * (ln_f2.first * (x - four * c) - two * ln_f2.second) + two * sqr(ln_f2.first * c + ln_f2.second) * ln(abs(ln_f2.first * x + ln_f2.second))) / two / takeDegreeOf(ln_f2.first, three);
        }
        //здесь воспользуемся формулами: dx/(x+w)/(x+u) -> 1/(w-u)*ln(|(x+u)/(x+w)|), ax^2+bx+c = a(x-x1)(x-x2), x1,2 = (-b+-sqrt(D))/2a
        if (D->getPositionRelativelyZero() > 0)
        {
            auto w = -(-qc_f[1] + sqrt(D))/(two*qc_f[0]);
            auto u = -(-qc_f[1] - sqrt(D))/(two*qc_f[0]);
            return qc_f[0] * (ln_f2.first * x * (ln_f2.first * (two*u + two*w + x) - two*ln_f2.second) + two * (ln_f2.second - ln_f2.first * w) * (ln_f2.second - ln_f2.first * u) * ln(abs(ln_f2.first * x + ln_f2.second))) / two / pow(ln_f2.first, three);
        }*/
        abs_ex& a = qc_f[0];
        abs_ex& b = qc_f[1];
        abs_ex& c = qc_f[2];
        abs_ex& f = ln_f2.first;
        abs_ex &g = ln_f2.second;
        abs_ex x (new Variable(getVariable(var)));
        return (two * ln(abs(f*x + g))*(g*(a*g - b*f) + c*f*f) + f*x*(a*f*x - two*a*g + two*b*f)) /two / pow(f, three);
    };
    auto quadraticDivideQuadraticFormula = [&var](std::array<abs_ex, 3> && qc_f1, std::array<abs_ex, 3> && qc_f2)->abs_ex {
      // (ax^2+bx+c)/(ex^2+fx+g)
        abs_ex a = std::move(qc_f1[0]), b = std::move(qc_f1[1]), c = std::move(qc_f1[2]), e = std::move(qc_f2[0]), f = std::move(qc_f2[1]), g = std::move(qc_f2[2]);
        abs_ex D1 = sqr(b) - four * a * c;
        abs_ex D2 = sqr(f) - four * e * g;
        abs_ex x(new Variable(getVariable(var)));
        if ((*D1 == *zero || D1->getPositionRelativelyZero() > 0) && *D2 == *zero)
        {//(x-w)(x-u)/(x-t)^2
            abs_ex w = (-b + sqrt(D1))/two/a;
            abs_ex u = (-b - sqrt(D1))/two/a;
            abs_ex t = -f/two/e;

            return a / e * ((t - u)*(t - w) / (t - x) - (-two * t + u + w)*ln(abs(t - x)) + x);
        }
        if ((*D1 == *zero || D1->getPositionRelativelyZero() > 0) && (D2->getPositionRelativelyZero() > 0))
        {
            // (x-w)(x-u)/(x-t)/(x-z)
            abs_ex w = (-b + sqrt(D1))/two/a;
            abs_ex u = (-b - sqrt(D1))/two/a;
            abs_ex t = (-f + sqrt(D2))/two/a;
            abs_ex z = (-f - sqrt(D2))/two/a;
            return a / e * ((t - u)*(t - w) * ln(abs(x - t)) + x*(t - z) + (u - z)*(z - w)*ln(abs(x - z))) / (t - z);
        }
        return (two*a*e*x + (two*(two*c*sqr(e) -b*e*f + a*(sqr(f) - two*e*g))*atan((f + two*e*x) / sqrt(-D2)))/sqrt(-D2) + (b*e - a*f)*ln(abs(g + x*(f + e*x))))/two/sqr(e);
    };

    if (numerator.size() == 1 && denominator.size() == 1)
    {
        auto num = numerator.begin()->get();
        auto denom = denominator.begin()->get();
        auto ln_f = checkIfItsLinearFunction(num, var);
        auto ln_f2 = checkIfItsLinearFunction(denom, var);
        auto x = abs_ex(new Variable(getVariable(var)));
        if (ln_f.first != nullptr && ln_f2.first != nullptr) // (ax+b)/(cx+d) dx
            return ln_f.first / ln_f2.first * x + (ln_f.second * ln_f2.first - ln_f.first * ln_f2.second) * ln(abs(*denominator.begin())) / sqr(ln_f2.first);
        if (ln_f.first != nullptr && ln_f2.first == nullptr)
        {
            auto qc_f2 = checkIfItsQuadraticFunction(denom, var);
            if (qc_f2[0] != nullptr) // (ax+b)/(cx^2+dx+e)
            {
                return linearDivideQuadraticFormula(ln_f, qc_f2);
            }

            if (denom->getId() == DEGREE)
            {
                //аналогично если дискриминант больше нуля, то трехчлен должен быть уже автоматом разложенный
                auto deg = Degree::getDegreeOfExpression(denom);
                if (*deg == *two)
                {
                    auto arg = Degree::getArgumentOfDegree(denom);
                    auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                    if (qc_f[0] != nullptr)
                    {
                        abs_ex &a = ln_f.first;
                        abs_ex &b = ln_f.second;
                        abs_ex &c = qc_f[0];
                        abs_ex &d = qc_f[1];
                        abs_ex &e = qc_f[2];
                        auto D = sqr(d) - four*c*e;
                        return ((-b *(d + two* c* x) + a* (two* e + d* x))/(e + x* (d + c* x)) + (
                         two* (-two* b* c + a* d)* atan((d + two* c* x)/sqrt(-D)))/sqrt(-D))/(D);
                    }
                }
                if (*deg == *three)
                {
                    auto arg = Degree::getArgumentOfDegree(denom);
                    auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                    if (qc_f[0] != nullptr)
                    {
                        abs_ex &a = ln_f.first;
                        abs_ex &b = ln_f.second;
                        abs_ex &c = qc_f[0];
                        abs_ex &d = qc_f[1];
                        abs_ex &e = qc_f[2];
                        auto D = sqr(d) - four*c*e;
                        return ((three* (two* b* c - a* d)* (d + two* c* x))/(
                         e + x* (d + c *x)) + ((D)* (-b* (d + two* c* x) +
                            a* (two* e + d* x)))/sqr(e + x* (d + c* x)) - (
                         numToAbs(12) *c* (numToAbs(-2)* b* c + a* d) *atan((d + two* c* x)/
                           sqrt(-D)))/sqrt(-D))/(two* sqr(D));

                    }
                }
            }
        }
        if (ln_f.first == nullptr && ln_f2.first != nullptr)
        {
            auto qc_f = checkIfItsQuadraticFunction(num, var);
            if (qc_f[0] != nullptr)
            {
                return quadraticDivideLinearFormula(qc_f, ln_f2);
            }
            if (num->getId() == DEGREE)
            {
                //аналогично если дискриминант больше нуля, то трехчлен должен быть уже автоматом разложенный
                auto deg = Degree::getDegreeOfExpression(num);
                if (*deg == *two)
                {
                    auto arg = Degree::getArgumentOfDegree(num);
                    auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                    if (qc_f[0] != nullptr)
                    {
                        abs_ex& a = qc_f[0];
                        abs_ex& b = qc_f[1];
                        abs_ex& c = qc_f[2];
                        abs_ex& d = ln_f2.first;
                        abs_ex& e = ln_f2.second;
                        return (d* x *(numToAbs(6)* sqr(b)* sqr(d)* (-two* e + d* x) +
                                     four* b* d* (numToAbs(6)* c* sqr(d) + a* (numToAbs(6)* sqr(e) - three* d* e* x + two* sqr(d)* sqr(x))) +
                                     a *(numToAbs(12)* c* sqr(d)* (-two* e + d* x) +
                                        a *(-numToAbs(12)* pow(e, 3) + numToAbs(6)* d* sqr(e)* x - four* sqr(d)* e* sqr(x) + three* pow(d, 3)* pow(x, 3)))) +
                                  numToAbs(12)* sqr(c* sqr(d) + e* (-b* d + a* e))* ln(abs(e + d *x)))/(numToAbs(12)* pow(d, 5));
                    }
                }
                if (*deg == *three)
                {
                    auto arg = Degree::getArgumentOfDegree(num);
                    auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                    if (qc_f[0] != nullptr)
                    {
                        abs_ex& a = qc_f[0];
                        abs_ex& b = qc_f[1];
                        abs_ex& c = qc_f[2];
                        abs_ex& d = ln_f2.first;
                        abs_ex& e = ln_f2.second;
                        return (one/(numToAbs(60)* pow(d, 7)))*(d* x* (numToAbs(10)* pow(b, 3)* pow(d, 3)* (numToAbs(6)* sqr(e) - three* d* e* x + two* sqr(d)* sqr(x)) +
                                                 numToAbs(15)* sqr(b)* sqr(d)* (numToAbs(6)* c* sqr(d)* (-two* e + d* x) +
                                                    a* (-numToAbs(12)* pow(e, 3) + numToAbs(6)* d* sqr(e)* x - four* sqr(d)* e* sqr(x) + three* pow(d, 3)* pow(x, 3))) +
                                                 three* b* d* (numToAbs(60)* sqr(c)* pow(d, 4) + numToAbs(20)* a* c* sqr(d)* (numToAbs(6)* sqr(e) - three* d* e* x + two* sqr(d)* sqr(x)) +
                                                    sqr(a)* (numToAbs(60)* pow(e, 4) - numToAbs(30)* d* pow(e, 3)* x + numToAbs(20)* sqr(d)* sqr(e)*sqr(x) - numToAbs(15)* pow(d, 3)* e* pow(x, 3) +
                                                       numToAbs(12)* pow(d, 4)* pow(x, 4))) +
                                                 a* (numToAbs(90)* sqr(c)* pow(d, 4)* (-two* e + d* x) +
                                                    numToAbs(15)* a* c* sqr(d)* (-numToAbs(12)* pow(e, 3) + numToAbs(6)* d* sqr(e)* x - four* sqr(d)* e* sqr(x) + three* pow(d, 3)* pow(x, 3)) +
                                                    sqr(a)* (-numToAbs(60)* pow(e, 5) + numToAbs(30)* d* pow(e, 4)* x - numToAbs(20)* sqr(d)* pow(e, 3)* sqr(x) + numToAbs(15)* pow(d, 3)* sqr(e)* pow(x, 3) -
                                                       numToAbs(12)* pow(d, 4)* e* pow(x, 4) + numToAbs(10)* pow(d, 5)* pow(x, 5)))) +
                                              numToAbs(60)* pow(c* sqr(d) + e* (-b* d + a* e), 3)* ln(abs(e + d* x)));
                    }
                }
            }
        }
        auto qc_f = checkIfItsQuadraticFunction(num, var);
        auto qc_f2 = checkIfItsQuadraticFunction(denom, var);
        if (qc_f[0] != nullptr && qc_f2[0] != nullptr)
            return quadraticDivideQuadraticFormula(std::move(qc_f), std::move(qc_f2));
        if (num->getId() == DEGREE && denom->getId() == DEGREE)
        {
            auto deg1 = Degree::getDegreeOfExpression(num);
            auto deg2 = Degree::getDegreeOfExpression(denom);
            if (*deg1 == *half && *deg2 ==*half)
            {
                auto arg_num = Degree::getArgumentOfDegree(num);
                auto arg_denom = Degree::getArgumentOfDegree(denom);
                auto ln_f1 = ::checkIfItsLinearFunction(arg_num, var);
                auto ln_f2 = ::checkIfItsLinearFunction(arg_denom, var);
                if (ln_f1.first != nullptr && ln_f2.first != nullptr)
                {
                    abs_ex & a = ln_f1.first;
                    abs_ex & b = ln_f1.second;
                    abs_ex & c = ln_f2.first;
                    abs_ex & d = ln_f2.second;
                    return one / c * sqrt(copy(arg_num) * copy(arg_denom)) - (a*d - b*c)/c/sqrt(a*c) *
                            atan(sqrt(a* copy(arg_denom) / (c * copy(arg_num))));
                }
            }
        }
        if (*num == *x && denom->getId() == DEGREE)
        {
            auto deg = Degree::getDegreeOfExpression(denom);
            if (*deg == *half)
            {
                auto qc_f = ::checkIfItsQuadraticFunction(Degree::getArgumentOfDegree(denom), var);
                if (qc_f[0] != nullptr)
                {
                    if (*qc_f[1] == *zero)
                    {
                        int pos = qc_f[0]->getPositionRelativelyZero();
                        if (pos > 0)
                        {
                            auto div = std::move(qc_f[0]);
                            qc_f[0] = copy(one);
                            qc_f[2] = qc_f[2] / div;
                            int pos_a = qc_f[2]->getPositionRelativelyZero();
                            if (pos_a > 0)
                                return one / sqrt(div) * copy(denom);
                        }
                        if (pos < 0)
                        {
                            auto div = -qc_f[0];
                            qc_f[0] = copy(minus_one);
                            qc_f[2] = qc_f[2] / div;
                            int pos_a = qc_f[2]->getPositionRelativelyZero();
                            if (pos_a > 0)
                                return minus_one/sqrt(div) * copy(denom);
                        }
                        return nullptr;
                    }
                }
            }
        }
        if (num->getId() == DEGREE && *denom == *x)
        {
            auto deg = Degree::getDegreeOfExpression(denom);
            if (*deg == *half)
            {
                auto qc_f = ::checkIfItsQuadraticFunction(Degree::getArgumentOfDegree(denom), var);
                if (qc_f[0] != nullptr)
                {
                    if (*qc_f[1] == *zero)
                    {
                        /*if (*qc_f[0] == *one)
                        {
                            int pos = qc_f[2]->getPositionRelativelyZero();
                            if (pos < 0)
                                return copy(num) + sqrt(-qc_f[2]) * asin(sqrt(-qc_f[2])/x);
                            return nullptr;
                        }
                        if (*qc_f[0] == *minus_one)
                        {
                            int pos = qc_f[2]->getPositionRelativelyZero();
                            if (pos > 0)
                                return copy(num) + sqrt(qc_f[2]) * ln(abs(x/ (sqrt(qc_f[2]) + copy(num))));
                            return nullptr;
                        }*/
                        int pos = qc_f[0]->getPositionRelativelyZero();
                        if (pos > 0)
                        {
                            auto div = std::move(qc_f[0]);
                            qc_f[0] = copy(one);
                            qc_f[2] = qc_f[2] / div;
                            int pos_a = qc_f[2]->getPositionRelativelyZero();
                            if (pos_a < 0)
                                return sqrt(div) * (copy(num) + sqrt(-qc_f[2]) * asin(sqrt(-qc_f[2])/x));
                        }
                        if (pos < 0)
                        {
                            auto div = -qc_f[0];
                            qc_f[0] = copy(minus_one);
                            qc_f[2] = qc_f[2] / div;
                            int pos_a = qc_f[2]->getPositionRelativelyZero();
                            if (pos_a > 0)
                                return sqrt(div) * (copy(num) + sqrt(qc_f[2]) * ln(abs(x/ (sqrt(qc_f[2]) + copy(num)))));
                        }
                        return nullptr;
                    }
                }
            }
        }
        if (num->getId() == LOGARITHM)
        {
            auto ln_f_log = checkIfItsFunctionOfLinearArgument(num, var);
            auto ln_f = ::checkIfItsLinearFunction(denom, var);
            if (ln_f_log.first != nullptr && ln_f.first != nullptr &&
                    ::isZero(ln_f_log.first - ln_f.first) &&
                    ::isZero(ln_f_log.second - ln_f.second))
            {
                return pow(ln(ln_f.first * x + ln_f.second), 2)/two/ln_f.first;
            }
        }
        if (denom->getId() == DEGREE)
        {
            //https://portal.tpu.ru/SHARED/r/ROZHKOVA/page-13/page-14/Tab2/RSV-HM_Lecture-37.pdf
            auto deg = Degree::getDegreeOfExpression(denom);
            if (isIntegerNumber(deg))
            {
                auto arg = Degree::getArgumentOfDegree(denom);
                auto ln_f = ::checkIfItsLinearFunction(arg, var);
                if (ln_f.first != nullptr)
                {
                    if (has(num->getSetOfVariables(), var))
                        return nullptr;
                    auto A = copy(num);
                    auto a = std::move(ln_f.first);
                    auto b = std::move(ln_f.second);
                    return A/a/(one - deg)/pow(copy(arg), deg - one);
                }
                auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                if (qc_f[0] != nullptr)
                {
                    auto pol = ::checkIfItsPolynom(num, var);
                    if (pol.size() > 2 || pol.size() == 0)
                        return nullptr;
                    if (pol.size() == 1)
                        pol.push_back(copy(zero));
                    if (*qc_f[0] != *one)
                    {
                        auto p = systemVarExpr();
                        auto q = systemVarExpr();
                        auto integr = (copy(num)/pow((x*x + p*x + q), deg));
                        if (integr == nullptr)
                            return nullptr;
                        integr = integr / qc_f[0];
                        setUpExpressionIntoVariable(integr, qc_f[1]/qc_f[0], p->getId());
                        setUpExpressionIntoVariable(integr, qc_f[2]/qc_f[0], q->getId());
                        return integr;
                    }
                    if (*qc_f[0] == *one && *qc_f[1] == *zero && qc_f[2]->getPositionRelativelyZero() > 0)
                    {
                        return pol[1] / two / pow(copy(arg), deg - one) / (one - deg) +
                                pol[0] * (one/copy(denom))->antiderivative(var);
                    }
                    if ((qc_f[2] - pow(qc_f[1], 2)/four)->getPositionRelativelyZero() <= 0)
                        return nullptr;
                    auto t = systemVarExpr();
                    auto q = systemVarExpr(zero, nullptr, true, false);
                    auto integr = ((pol[1] * t - pol[1] * qc_f[1]/two + pol[0])/pow(t*t + q, deg))->antiderivative(t->getId());
                    if (integr == nullptr)
                        return nullptr;
                    setUpExpressionIntoVariable(integr, x + qc_f[1]/two, t->getId());
                    setUpExpressionIntoVariable(integr, qc_f[2] - pow(qc_f[1], 2)/four, q->getId());
                    return integr;

                }
            }
        }
       /* auto pol = checkIfItsPolynom(num, var);
        if (pol.size() > 0 && denom->getId() == DEGREE)
        {
            auto deg_p = Degree::getDegreeOfExpression(denom);
            if (deg_p->getId() != NUMBER)
                return nullptr;
            if (static_cast<Number*>(deg_p.get())->isInteger())
            {
                int deg = static_cast<Number*>(deg_p.get())->getNumerator();
                auto arg = copy(Degree::getArgumentOfDegree(denom));
                auto ln_f = ::checkIfItsLinearFunction(arg, var);
                if (ln_f.first != nullptr)
                {
                    if (pol.size() > deg)
                        return nullptr;
                    if (pol.size() < deg)
                        while (pol.size() != deg)
                            pol.push_back(copy(zero));
                    if (deg == 3)
                    {
                        auto a = std::move(pol[2]);
                        auto b = std::move(pol[1]);
                        auto c = std::move(pol[0]);
                        auto w = std::move(ln_f.first);
                        auto f = std::move(ln_f.second);
                        return (a *f *(three* f + four*w*x) - w*(b*f + c*w + two*b*w*x) + two*a*sqr(f + w*x)*ln(f + w*x))/
                                (two*w*w*w*sqr(f + w*x));
                    }
                    if (deg == 4)
                    {
                        auto a = std::move(pol[3]);
                        auto b = std::move(pol[2]);
                        auto c = std::move(pol[1]);
                        auto u = std::move(pol[0]);
                        auto w = std::move(ln_f.first);
                        auto f = std::move(ln_f.second);
                        return (a*f*(numToAbs(11)*f*f + numToAbs(27)*f*w*x + numToAbs(18)*w*w*x*x) - w*(w*(c*f + two*u*w + three*c*w*x) +
                                two*b*(f*f + three*f*w*x + three*w*w*x*x))+ numToAbs(6)*a*pow(f + w*x,3)* ln(f + w*x))
                                /(numToAbs(6)*pow(w, 4)*pow(f + w*x, 3));
                    }
                }
            }
        }*/

    }
    if (numerator.size() == 0 && denominator.size() == 2)
    {
        auto ln_f1 = ::checkIfItsLinearFunction(*denominator.begin(), var);
        auto ln_f2 = ::checkIfItsLinearFunction(*next(denominator.begin()), var);
        if (ln_f1.first != nullptr && ln_f2.second != nullptr)
        {// 1/((a*x + b) (c*x + d))
            return (ln(abs(*denominator.begin())) - ln(abs(*next(denominator.begin()))))/(ln_f1.first * ln_f2.second - ln_f1.second * ln_f2.first);
        }
        auto den_beg = denominator.begin();
        auto den_next = next(denominator.begin());
        if (ln_f2.first != nullptr && ln_f1.first == nullptr)
        {
            std::swap(ln_f1, ln_f2);
            std::swap(den_beg, den_next);
        }
        if (ln_f1.first != nullptr)
        {//   1/((a*x + b)*(c*x^2 + d*x + e))
            auto qc_f = ::checkIfItsQuadraticFunction(*den_next, var);
            if (qc_f[0] != nullptr)
            {
                abs_ex& a = ln_f1.first;
                abs_ex& b = ln_f1.second;
                abs_ex &c = qc_f[0];
                abs_ex &e = qc_f[1];
                abs_ex &f = qc_f[2];
                //abs_ex &c = qc_f[0];
               // abs_ex &d = qc_f[1];
               // abs_ex &e = qc_f[2];
               // (a x (-2 b c + 2 a d + a c x) +
                // 2 (b^2 c - a b d + a^2 e) Log[b + a x])/(2 a^3)
                //return (a* x *(-two* b* c + two* a* d + a* c* x) +
                //                 two* (sqr(b)* c - a* b* d + sqr(a)*e) *ln(abs(*den_beg)))/(two*pow(a, 3));

                return (a*a)/(a*a*f + b*b*c - e*a*b) * (one/(a*x + b))->antiderivative(var) -
                        one / (a*a*f + b*b*c - e*a*b) * ((a*c*x - b*c +e*a)/(c*x*x +e*x +f))->antiderivative(var);

            }
            if (den_next->get()->getId() == DEGREE && *Degree::getDegreeOfExpression(den_next->get()) == *half)
            {
                auto arg = Degree::getArgumentOfDegree(den_next->get());
                auto dln_f = ::checkIfItsLinearFunction(arg, var);
                if (dln_f.first != nullptr)
                {// 1/((dx+c)sqrt(ax+b)) == d/((x + c/d)sqrt(ax+b)); c:=c/d
                    abs_ex &d = ln_f1.first;
                    abs_ex c = ln_f1.second / d;
                    abs_ex &a = dln_f.first;
                    abs_ex &b = dln_f.second;

                    abs_ex expr = b - a*c;
                    int pos = expr->getPositionRelativelyZero();
                    const abs_ex & root = *den_next;
                    if (pos > 0)
                    {
                        abs_ex temp = sqrt(expr);
                        return d/temp * ln(abs((root - temp) / (root + temp)));
                    }
                    if (pos < 0)
                    {
                        abs_ex temp = sqrt(-expr);
                        return d / temp * atan(root / temp);
                    }
                    if (pos == 0)
                        //Здесь бы по-хорошему разделение на разные ветки
                        assert(false);

                }
            }
        }
        auto qc_f1 = ::checkIfItsQuadraticFunction(*den_beg, var);
        auto qc_f2 = ::checkIfItsQuadraticFunction(*den_next, var);
        if (qc_f1[0] != nullptr && qc_f2[0] != nullptr)
        {//   1/(a*x^2 + b*x + f)*(c*x^2 + d*x + e)
            abs_ex &c = qc_f2[0];
            abs_ex &d = qc_f2[1];
            abs_ex &e = qc_f2[2];
            abs_ex &a = qc_f1[0];
            abs_ex &b = qc_f1[1];
            abs_ex &f = qc_f1[2];

            abs_ex D1 = sqr(b) - four * a * f;
            abs_ex D2 = sqr(d) - four * c * e;

        }
        //сортировка прошла также в порядке котангенс, тангенс, косинус, синус, степень
        const abs_ex & first = *den_beg;
        const abs_ex & second = *den_next;
        if (first->getId() == COSINUS && second->getId() == SINUS)
        {
            auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(first, var);
            auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(second, var);
            if (ln_f_cos.first != nullptr && ln_f_sin.first != nullptr && *(ln_f_cos.first - ln_f_sin.first) == *zero)
            {
                abs_ex & a = ln_f_cos.first;
                abs_ex & b = ln_f_cos.second;
                abs_ex & d = ln_f_sin.second;
                if (isIntegerNumber((b - d)/(::getPi()/two)))
                    return ln(abs(tan(a*x + b)))/a;
                return (ln(second) - ln(first))/(a*cos(b - d));
            }
        }
        if (first->getId() == COSINUS && second->getId() == DEGREE &&
                Degree::getArgumentOfDegree(second.get())->getId() == SINUS &&
                *Degree::getDegreeOfExpression(second.get()) == *two)
        {//          1 / ( sin(ax+b)^2*cos(ax + b))
            auto arg = Degree::getArgumentOfDegree(second.get());
            auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(first, var);
            auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(arg, var);
            if (ln_f_cos.first != nullptr && ln_f_sin.first != nullptr && *(ln_f_cos.first - ln_f_sin.first) == *zero &&
                    *(ln_f_cos.second - ln_f_sin.second) == *zero)
            {
                abs_ex & a = ln_f_cos.first;
                abs_ex & b = ln_f_cos.second;
                return one / a * (-one/sin(a*x+b) + ln(abs(cot((a*x + b)/two))));
            }
        }
        if (first->getId() == SINUS && second->getId() == DEGREE &&
                Degree::getArgumentOfDegree(second.get())->getId() == COSINUS &&
                *Degree::getDegreeOfExpression(second.get()) == *two)
        {//     1 / (sin(ax+b)*cos(ax+b)^2)
            auto arg = Degree::getArgumentOfDegree(second.get());
            auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(first, var);
            auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(arg, var);
            if (ln_f_cos.first != nullptr && ln_f_sin.first != nullptr && *(ln_f_cos.first - ln_f_sin.first) == *zero &&
                    *(ln_f_cos.second - ln_f_sin.second) == *zero)
            {
                abs_ex & a = ln_f_cos.first;
                abs_ex & b = ln_f_cos.second;
                return one / a * (one/cos(a*x+b) + ln(abs(tan((a*x + b)/two))));
            }
        }
        if (first->getId() == DEGREE && second->getId() == DEGREE &&
                Degree::getArgumentOfDegree(first.get())->getId() == COSINUS &&
                Degree::getArgumentOfDegree(second.get())->getId() == SINUS &&
                *Degree::getDegreeOfExpression(first.get()) == *two &&
                *Degree::getDegreeOfExpression(second.get()) == *two)
        {//    1 / ( sin(ax+b)^2 * cos(ax + b)^2 )
            auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(Degree::getArgumentOfDegree(first.get()), var);
            auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(Degree::getDegreeOfExpression(second.get()), var);
            if (ln_f_cos.first != nullptr && ln_f_sin.first != nullptr && *(ln_f_cos.first - ln_f_sin.first) == *zero &&
                    *(ln_f_cos.second - ln_f_sin.second) == *zero)
            {
                auto arg = getArgumentOfFunction(Degree::getArgumentOfDegree(first.get()));
                return one/ln_f_sin.first * (tan(arg) - cot(arg));
            }
        }
    }
    if (numerator.size() == 1 && denominator.size() == 2)
    {
        auto ln_f = ::checkIfItsLinearFunction(*numerator.begin(), var);
        auto ln_f1 = ::checkIfItsLinearFunction(*denominator.begin(), var);
        auto ln_f2 = ::checkIfItsLinearFunction(*next(denominator.begin()), var);
        if (ln_f.first != nullptr && ln_f1.first != nullptr && ln_f2.first != nullptr)
        {// (ex+f)/((ax+b)(cx+d))
            return ((-ln_f1.second * ln_f2.first * ln_f.first + ln_f1.first * ln_f2.first * ln_f.second) * ln(abs(*denominator.begin())) +
                    ln_f1.first * (ln_f2.second * ln_f.first - ln_f2.first * ln_f.second) * ln(abs(*next(denominator.begin())))) /
                    (ln_f1.first * ln_f2.first * (ln_f1.first * ln_f2.second - ln_f1.second * ln_f2.first));
        }
    }
    if (numerator.size() == 2 && denominator.size() == 2)
    {
        auto ln_f1 = ::checkIfItsLinearFunction(*numerator.begin(), var);
        auto ln_f2 = ::checkIfItsLinearFunction(*next(numerator.begin()), var);
        auto ln_f3 = ::checkIfItsLinearFunction(*denominator.begin(), var);
        auto ln_f4 = ::checkIfItsLinearFunction(*next(denominator.begin()), var);
        if (ln_f1.first != nullptr && ln_f2.first != nullptr && ln_f3.first != nullptr && ln_f4.first != nullptr)
        {// (e*x + f)*(g*x + h)/((a*x + b)*(c*x + d)
            abs_ex &e = ln_f1.first;
            abs_ex &f = ln_f1.second;
            abs_ex &g = ln_f2.first;
            abs_ex &h = ln_f2.second;
            abs_ex &a = ln_f3.first;
            abs_ex &b = ln_f3.second;
            abs_ex &c = ln_f4.first;
            abs_ex &d = ln_f4.second;
            return (sqr(c) * (b*e - a*f)*(b*g - a*h) * ln(abs(*denominator.begin())) + a*(c*(a*d - b*c)*e*g*x - a*(d*e - c*f)*(d*g - c*h)*ln(abs(*next(denominator.begin()))))) /
                    (sqr(a)*sqr(c) * (a*d - b*c));
        }
    }
    //Учитываем, что сортировка произойдет в следующем порядке : котангенс, тангенс, косинус, синус, степень
    if (numerator.size() == 2 && denominator.size() == 0)
    {
        const abs_ex& first = *numerator.begin();
        const abs_ex& second = *next(numerator.begin());

        if (second->getId() == DEGREE && first->getId() == var)
        {
            if (!Degree::getArgumentOfDegree(second.get())->hasVariable(var))
            {
                auto arg = copy(Degree::getArgumentOfDegree(second.get()));
                auto qc_f = ::checkIfItsQuadraticFunction(Degree::getDegreeOfExpression(second.get()), var);
                if (qc_f[0] != nullptr)
                {
                    if (::isZero(qc_f[1]))
                        return second / two / qc_f[0] / ln(arg);
                }

            }
        }
        if (first->getId() == COSINUS && second->getId() == SINUS)
        {
            auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(first, var);
            auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(second, var);
            if (ln_f_cos.first != nullptr && ln_f_sin.first != nullptr)
            {//  cos(cx + d)sin(ax+b)
                abs_ex& c = ln_f_cos.first;
                abs_ex& d = ln_f_cos.second;
                abs_ex& a = ln_f_sin.first;
                abs_ex& b = ln_f_sin.second;
                if (*(a - c) != *zero)
                    return -(cos(x*(a - c) +b - d))/(two*(a - c)) - cos(x*(a + c) +b + d)/(two * (a + c));
                return -((cos(two*a*x + b + d) - two*a*x*sin(b - d))/(four * a));
            }
        }
        if (first->getId() == COSINUS && second->getId() == COSINUS)
        {
            auto ln_f1 = ::checkIfItsFunctionOfLinearArgument(first, var);
            auto ln_f2 = ::checkIfItsFunctionOfLinearArgument(second, var);
            if (ln_f1.first != nullptr && ln_f2.first != nullptr)
            {// cos(ax + b)cos(cx + d)
                abs_ex & a = ln_f1.first;
                abs_ex & b = ln_f1.second;
                abs_ex & c = ln_f2.first;
                abs_ex & d = ln_f2.second;
                if (*(a - c) == *zero)
                    return (sin(two*a*x + b + d) + two*a*x*cos(b - d))/ (four * a);
                return sin(x*(a - c) + b - d)/(two*(a - c)) + sin(x*(a + c) + b + d)/(two*(a + c));
            }
        }
        if (first->getId() == SINUS && second->getId() == SINUS)
        {
            auto ln_f1 = ::checkIfItsFunctionOfLinearArgument(first, var);
            auto ln_f2 = ::checkIfItsFunctionOfLinearArgument(second, var);
            if (ln_f1.first != nullptr && ln_f2.first != nullptr)
            {// sin(ax + b)sin(cx + d)
                abs_ex & a = ln_f1.first;
                abs_ex & b = ln_f1.second;
                abs_ex & c = ln_f2.first;
                abs_ex & d = ln_f2.second;
                if (*(a - c) == *zero)
                    return -(sin(two*a*x + b + d) - two*a*x*cos(b - d))/ (four * a);
                return sin(x*(a - c) + b - d)/(two*(a - c)) - sin(x*(a + c) + b + d)/(two*(a + c));
            }
        }
        if (first->getId() == SINUS && second->getId() == DEGREE)
        {
            auto arg = Degree::getArgumentOfDegree(second.get());
            if (arg->getId() == COSINUS)
            {
                auto deg = Degree::getDegreeOfExpression(second.get());
                auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(first, var);
                if (ln_f_sin.first == nullptr)
                    return nullptr;
                auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(arg, var);
                if (ln_f_cos.first != nullptr && *ln_f_cos.first == *ln_f_sin.first &&
                        *ln_f_sin.second == *ln_f_cos.second)
                {//  sin(ax+b)cos(ax+b)^n
                    abs_ex &a = ln_f_sin.first;
                    //abs_ex &b = ln_f_sin.second;
                    return -pow(copy(arg), deg + one) / (a * (deg + one));
                }
            }
        }
        if (first->getId() == COSINUS && second->getId() == DEGREE)
        {
            auto arg = Degree::getArgumentOfDegree(second.get());
            if (arg->getId() == SINUS)
            {
                auto deg = Degree::getDegreeOfExpression(second.get());
                auto ln_f_cos = ::checkIfItsFunctionOfLinearArgument(first, var);
                if (ln_f_cos.first == nullptr)
                    return nullptr;
                auto ln_f_sin = ::checkIfItsFunctionOfLinearArgument(arg, var);
                if (ln_f_sin.first != nullptr && *ln_f_cos.first == *ln_f_sin.first &&
                        *ln_f_sin.second == *ln_f_cos.second)
                {//  cos(ax+b)sin(ax+b)^n
                    abs_ex &a = ln_f_sin.first;
                   // abs_ex &b = ln_f_sin.second;
                    return pow(copy(arg), deg + one) / (a * (deg + one));
                }
            }
        }
        if (first->getId() == DEGREE && second->getId() == DEGREE)
        { //sin(x)^n * cos(x)^m
            auto arg1 = Degree::getArgumentOfDegree(first.get());
            auto arg2 = Degree::getArgumentOfDegree(second.get());
            auto deg1 = Degree::getDegreeOfExpression(first.get());
            auto deg2 = Degree::getDegreeOfExpression(second.get());
            if (arg1->getId() == COSINUS && arg2->getId() == SINUS)
            {
                std::swap(arg1, arg2);
                std::swap(deg1, deg2);
            }
            if (arg1->getId() == SINUS && arg2->getId() == COSINUS && deg1->getId() == NUMBER &&
                    deg2->getId() == NUMBER && static_cast<Number*>(deg1.get())->isInteger() &&
                    static_cast<Number*>(deg2.get()))
            {
                auto ln_f1 = ::checkIfItsFunctionOfLinearArgument(arg1, var);
                if (ln_f1.first == nullptr)
                    return nullptr;
                auto ln_f2 = ::checkIfItsFunctionOfLinearArgument(arg2, var);
                if (ln_f2.first == nullptr || ln_f1.first != ln_f2.first || ln_f2.second != ln_f1.second)
                    return nullptr;
                long long int ideg1 = static_cast<Number*>(deg1.get())->getNumerator();
                long long int ideg2 = static_cast<Number*>(deg2.get())->getNumerator();
                if (ideg1 % 2 != 0)
                {//  sin^(2k+1)cos^mdx = (1 - cos^2)^k*cos^m*sin dx = |t=-cosx |=(1-t^2)^2*(-t)^m dt
                    abs_ex t = abs_ex(new Variable(systemVar()));
                    abs_ex pre_integral = (pow(one - sqr(t), ideg1/2) * pow(-t, ideg2))->antiderivative(t->getId());
                    std::map<int, abs_ex> to_change;
                    to_change.insert({t->getId(), copy(arg2) * minus_one});
                    replaceSystemVariablesToExpressions(pre_integral, to_change);
                    return one/ln_f2.first * pre_integral;
                }
                else if (ideg2 % 2 != 0)
                {//  sin^ncos^(2k+1)dx = sin^n (1 - sin^2)^k cos dx = |t=sin x |= t^n (1 - t^2) dt
                    abs_ex t = abs_ex(new Variable(systemVar()));
                    abs_ex pre_integral = (pow(t, ideg1) * pow(one - sqr(t), ideg2/2))->antiderivative(t->getId());
                    std::map<int, abs_ex> to_change;
                    to_change.insert({t->getId(), copy(arg1)});
                    replaceSystemVariablesToExpressions(pre_integral, to_change);
                    return one/ln_f2.first * pre_integral;
                }
                else
                {
                    if (ideg1 > ideg2)
                    {
                        return (pow(copy(arg1), ideg1) * pow(one - sqr(copy(arg1)), ideg2/2))->antiderivative(var);
                    }
                    else if (ideg1 == ideg2)
                        return pow(half * sin(two* getArgumentOfFunction(arg1)), ideg1)->antiderivative(var);
                    else
                        return (pow(one - sqr(copy(arg2)), ideg1/2) * pow(copy(arg2), ideg2))->antiderivative(var);
                }
            }
        }
    }
    return nullptr;
}

void Fractal::separatePolynomialsDegree()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == DEGREE && Degree::getDegreeOfExpression(it->get())->getId() == POLYNOMIAL)
        {
            auto arg = copy(Degree::getArgumentOfDegree(it->get()));
            auto deg = Degree::getDegreeOfExpression(it->get());
            auto degs = static_cast<Polynomial*>(deg.get())->getMonomialsPointers();
            for (auto &it : degs)
                this->numerator.push_front(pow(arg, copy(it)));
            it = this->numerator.erase(it);
        }
        else
            ++it;
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == DEGREE && Degree::getDegreeOfExpression(it->get())->getId() == POLYNOMIAL)
        {
            auto arg = copy(Degree::getArgumentOfDegree(it->get()));
            auto degs = static_cast<Polynomial*>(Degree::getDegreeOfExpression(it->get()).get())->getMonomialsPointers();
            for (auto &it : degs)
                this->denominator.push_front(pow(arg, copy(it)));
            it = this->denominator.erase(it);
        }
        else
            ++it;
    }
    //в конце никаких simplify(), иначе все эти разделенные степени обратно засунутся!
}

abs_ex Fractal::getPartWithVariable(int var) const
{
    return copy(this)/this->getFractalWithoutVariable(var);
}

bool Fractal::hasTrigonometricalMultipliers() const
{
    for (auto &it : this->numerator)
        if (isDegreeOfTrigonometricalFunction(it))
            return true;
    return false;
}

FunctionRange Fractal::getRange() const
{
    FunctionRange range = this->coefficient.getRange();
    std::set<int> vars_set;
    for (auto &it : this->numerator)
    {
        auto v_set = it->getSetOfVariables();
        if (hasIntersections(vars_set, v_set))
            return FunctionRange::getErrorRange();
        for (auto &it : v_set)
            vars_set.insert(it);
        auto mult_range = it->getRange();
        if (mult_range.isError())
            return mult_range;
        range = rangeOfMultiplication(range, mult_range);
    }
    for (auto &it : this->denominator)
    {
        auto v_set = it->getSetOfVariables();
        if (hasIntersections(vars_set, v_set))
            return FunctionRange::getErrorRange();
        for (auto &it : v_set)
            vars_set.insert(it);
        auto mult_range = it->getRange();
        if (mult_range.isError())
            return mult_range;
        range = rangeOfDivision(range, mult_range);
    }
    return range;

}

bool Fractal::hasDifferential() const
{
    for (auto &it : this->numerator)
        if (it->hasDifferential())
            return true;
    for (auto &it : this->denominator)
        if (it->hasDifferential())
            return true;
    return false;
}

bool Fractal::hasIntegratingConstantThatCanBeChanged() const
{
    return this->numerator.size() > 0 && isIntegratingConstantAndCanChangeIt(this->numerator.back()->getId());
}

void Fractal::takeAwayAbsoluteValues()
{
    bool taken_something = false;
    for (auto &it : this->numerator)
        if (it->getId() == ABSOLUTE_VALUE)
        {
            it = copy(static_cast<AbsoluteValue*>(it.get())->getExpression());
            taken_something = true;
        }
    for (auto &it : this->denominator)
        if (it->getId() == ABSOLUTE_VALUE)
        {
            it = copy(static_cast<AbsoluteValue*>(it.get())->getExpression());
            taken_something = true;
        }
    if (taken_something)
        this->simplify();
}

bool Fractal::hasIntegratingConstantMultiplierThatCanBeChanged() const
{
    for (auto &it : numerator)
        if (isIntegratingConstantAndCanChangeIt(it->getId()))
            return true;
    for (auto &it : denominator)
        if (isIntegratingConstantAndCanChangeIt(it->getId()))
            return true;
}

abs_ex Fractal::takeAwayConstantMultiplierThatCanBeChanged()
{
    assert(this->hasIntegratingConstantThatCanBeChanged());
    abs_ex constant = std::move(this->numerator.back());
    this->numerator.erase(--this->numerator.end());
    return constant;
}

bool Fractal::tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second)
{
    if (second->getId() == FRACTAL)
    {
        bool merged = false;
        //abs_ex its_second =  copy(second);
        auto fr = static_cast<Fractal*>(second.get())->getFractal();
        std::list<std::pair<fractal_argument::iterator, abs_ex>> originals;

        std::map<int, abs_ex> replaced_on_system_vars_integrating_constants;

        for (auto it1 = this->numerator.begin(), it2 = fr.first->begin();
             it1 != this->numerator.end() && it2 != fr.first->end();)
        {
            if (canBeConsideredAsConstantThatCanBeChanged(*it1) &&
                    canBeConsideredAsConstantThatCanBeChanged(*it2))
            {
                ++it1;
                ++it2;
            }
            else if (canBeConsideredAsConstantThatCanBeChanged(*it1))
                ++it1;
            else if (canBeConsideredAsConstantThatCanBeChanged(*it2))
                ++it2;
            if (it1 == this->numerator.end() && it2 != fr.first->end())
                return false;
            if (it2 == fr.first->end() && it1 != this->numerator.end())
                return false;
            if (it1 == this->numerator.end())
            {
                break;
            }
            if (subCompare(*it1, *it2))
            {
                ++it1;
                ++it2;
                continue;
            }
            if (isIntegratingConstant(it1->get()->getId()) &&
                    isIntegratingConstant(it2->get()->getId()))
            {
                ++it1;
                ++it2;
                continue;
            }

            originals.push_back({it1, copy(*it1)});
            replaceIntegratingConstantsOnSystemVariables(originals.back().second, replaced_on_system_vars_integrating_constants);
          //  qDebug() << originals.back().second->toString();
           // qDebug() << VariablesDistributor::amountOfVariable(1500000012);
          //  qDebug() << it1->get()->toString();
            if (!it1->get()->tryToMergeIdenticalBehindConstantExpressions(*it2))
            {

                for (auto &it : originals)
                {
                    replaceSystemVariablesBackOnIntegratingConstants(it.second, replaced_on_system_vars_integrating_constants);
                    *it.first = std::move(it.second);
                }
                return false;
            }
            ++it1;
            ++it2;
            merged = true;
        }
        for (auto it1 = this->denominator.begin(), it2 = fr.second->begin();
             it1 != this->denominator.end() && it2 != fr.second->end();)
        {
            if (canBeConsideredAsConstantThatCanBeChanged(*it1) &&
                    canBeConsideredAsConstantThatCanBeChanged(*it2))
            {
                ++it1;
                ++it2;
            }
            else if (canBeConsideredAsConstantThatCanBeChanged(*it1))
                ++it1;
            else if (canBeConsideredAsConstantThatCanBeChanged(*it2))
                ++it2;
            if (it1 == this->denominator.end() && it2 != fr.second->end())
                return false;
            if (it2 == fr.second->end() && it1 != this->denominator.end())
                return false;
            if (it1 == this->denominator.end())
            {
                break;
            }
            if (subCompare(*it1, *it2))
            {
                ++it1;
                ++it2;
                continue;
            }
            originals.push_back({it1, copy(*it1)});
            replaceIntegratingConstantsOnSystemVariables(originals.back().second, replaced_on_system_vars_integrating_constants);

            if (!it1->get()->tryToMergeIdenticalBehindConstantExpressions(*it2))
            {
                for (auto &it : originals)
                {
                    replaceSystemVariablesBackOnIntegratingConstants(it.second, replaced_on_system_vars_integrating_constants);
                    *it.first = std::move(it.second);
                }
                return false;
            }
            ++it1;
            ++it2;
            merged = true;
        }

        FunctionRange second_range = getRangeOfConstantMultipliersThatCanBeChanged(second);
        if (!(second_range.isPoint() && *second_range.getPoint() == *one))
        {
            merged = true;
            FunctionRange this_range = getRangeOfConstantMultipliersThatCanBeChangedAndTakeThemAway(dynamic_cast<AbstractExpression*>(this));
            this->pushBackToNumerator(integratingConstantExpr(unification(this_range, second_range)));

        }

        return merged;



    }
    return false;
}

abs_ex Fractal::tryToFindExponentialFunction(int var) const
{
    for (auto &it : this->numerator)
    {
        abs_ex res = it->tryToFindExponentialFunction(var);
        if (res != nullptr)
            return res;
    }
    for (auto &it : this->denominator)
    {
        abs_ex res = it->tryToFindExponentialFunction(var);
        if (res != nullptr)
            return res;
    }
    return nullptr;
}

void Fractal::pullSomeMultipliersIntoIntegratingConstant()
{
    FunctionRange range;
    bool initialized = false;
    bool is_only_constant = true;
    if (this->coefficient != 1)
    {
        range = this->coefficient.getRange();
        initialized = true;
        is_only_constant = false;
    }
    this->coefficient = 1;
    std::set<int> used_vars;

    abs_ex its_constant = nullptr;
    for (auto it = this->numerator.begin(); it != this->numerator.end();)
    {
        auto set = it->get()->getSetOfVariables();
        bool skip = false;
        if (!(set.empty() || isIntegratingConstantAndCanChangeIt(it->get()->getId())))
            skip = true;
        if (skip)
        {
            ++it;
            continue;
        }
        if (is_only_constant && its_constant == nullptr &&
                isIntegratingConstantAndCanChangeIt(it->get()->getId()))
        {
            its_constant = copy(*it);
        }
        else
            is_only_constant = false;
        for (auto &it1 : set)
            used_vars.insert(it1);
        auto it_range = it->get()->getRange();
        if (initialized)
            range = rangeOfMultiplication(range, it_range);
        else
        {
            range = std::move(it_range);
            initialized = true;
        }
        it = this->numerator.erase(it);
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end();)
    {
        auto set = it->get()->getSetOfVariables();
        bool skip = false;
        for (auto &it1 : set)
            if (!isIntegratingConstantAndCanChangeIt(it1) || has(used_vars, it1))
            {
                skip = true;
                break;
            }
        if (skip)
        {
            ++it;
            continue;
        }
        if (is_only_constant && its_constant == nullptr &&
                isIntegratingConstantAndCanChangeIt(it->get()->getId()))
        {
            its_constant = copy(*it);
        }
        else
            is_only_constant = false;
        for (auto &it1 : set)
            used_vars.insert(it1);
        auto it_range = it->get()->getRange();
        if (initialized)
            range = rangeOfDivision(range, it_range);
        else
        {
            range = std::move(it_range);
            initialized = true;
        }
        it = this->denominator.erase(it);
    }
    if (is_only_constant && its_constant != nullptr)
    {
        this->numerator.push_back(std::move(its_constant));
        return;
    }
    this->numerator.push_back(integratingConstantExpr(range));
}

abs_ex Fractal::tryToFindLogarithmInNumerator() const
{
    for (auto &it : this->numerator)
        if (Degree::getArgumentOfDegree(it.get())->getId() == LOGARITHM)
            return copy(Degree::getArgumentOfDegree(it.get()));
    return nullptr;
}

std::pair<abs_ex, abs_ex > Fractal::checkIfCanDoUniversalTrigonometricSubstitution(int var) const
{
    auto trig_functions = this->getTrigonometricalFunctions();
    if (trig_functions.empty())
        return {nullptr, nullptr};
    auto ln_f = checkIfItsFunctionOfLinearArgument(*trig_functions.begin(), var);
    if (ln_f.first == nullptr)
        return {nullptr, nullptr};
    if (trig_functions.begin()->get()->getId() == TANGENT ||
            trig_functions.begin()->get()->getId() == COTANGENT)
    {
        ln_f.first = ln_f.first * two;
        ln_f.second = ln_f.second * two;
    }
    for (auto it = next(trig_functions.begin()); it != trig_functions.end(); ++it)
    {
        auto it_ln_f = checkIfItsFunctionOfLinearArgument(*it, var);
        if (it_ln_f.first == nullptr)
            return {nullptr, nullptr};
        if (it->get()->getId() == SINUS || it->get()->getId() == COSINUS)
        {
            if (!(*ln_f.first == *it_ln_f.first && *ln_f.second == *it_ln_f.second))
                return {nullptr, nullptr};
        }
        else
        {
            if (!(*(ln_f.first/two) == *it_ln_f.first && *(ln_f.second/two) == *it_ln_f.second))
                return {nullptr, nullptr};
        }
    }
    return ln_f;
}

bool Fractal::isNumeratorsDegreeBiggerThanDenominatorsDegree(int var) const
{
    //assert(this->isRationalFunction());
    Number num_deg = 0;
    for (auto &it : numerator)
        num_deg += it->getMaxDegreeOfVariable(var);
    Number denum_deg = 0;
    for (auto &it : denominator)
        denum_deg += it->getMaxDegreeOfVariable(var);
    return num_deg.compareWith(denum_deg) > 0;
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

void Fractal::castTrigonometryArguments()
{
    //все степени здесь целые
    //первое выражение - аргумент функции, второй - степень функции и то, есть у этой функции нечетная степень, затем в словаре аргументы, которые представляют собой тот
    //аргумент, умноженный на какое-либо число.
    //ключ словаря - то самое число,  число в значении словаря - степени триг. функции с этим самым аргументом и то, есть ли у этой функции нечетная степень,
    //а строка - строковое представление функции с этим аргументом

    std::vector<std::pair<std::pair<abs_ex, std::pair<Number, bool>>, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>>>>
                                                                                                                                                            arguments_multipliers;

   auto arguments = this->getTrigonometryMultipliersArgumentsCopyAndItsDegrees();
   distributeTrigonometryArgumentsMultipliersRatios(arguments_multipliers, arguments);

   auto res = chooseInstructionsToCastTrigonometryArguments(arguments_multipliers);
   bool need_to_convert = res.second;
   auto instructions = std::move(res.first);
    if (need_to_convert)
        this->convertTrigonometricalMultipliersToDifferentArgument(instructions);
    if (need_to_convert)
    {
        this->simplified = false;
        this->simplify();
    }
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

abs_ex toAbsEx(const std::unique_ptr<Fractal> &expr)
{
    return abs_ex(copy(expr.get()).release())->downcast();
}

abs_ex toAbsEx(std::unique_ptr<Fractal> &&expr)
{
    return abs_ex(expr.release())->downcast();
}

std::unique_ptr<Fractal> toFrac(const abs_ex &expr)
{
    if (expr->getId() == FRACTAL)
        return std::unique_ptr<Fractal>(
                    static_cast<Fractal*>(copy(expr).release()));
    return std::unique_ptr<Fractal>(new Fractal(expr.get()));
}

std::unique_ptr<Fractal> toFrac(abs_ex &&expr)
{
    if (expr->getId() == FRACTAL)
        return std::unique_ptr<Fractal>(
                    static_cast<Fractal*>(expr.release()));
    return std::unique_ptr<Fractal>(new Fractal(std::move(expr)));
}

abs_ex integrate(const abs_ex &frac)
{
  //  qDebug() << "INTEGR: " << frac->makeStringOfExpression();
    if (frac->getId() == DIFFERENTIAL)
    {
        Differential* diff = static_cast<Differential*>(frac.get());
        assert(diff->getSetOfVariables().size() == 1);
        return diff->antiderivative(*diff->getSetOfVariables().begin());
    }
    assert(frac->getId() == FRACTAL);
    auto fr= static_cast<const Fractal*>(frac.get())->getFractal();
    fr.first->sort(&AbstractExpression::lessToSort);
    assert(fr.first->back()->getId() == DIFFERENTIAL);
    Differential* diff = static_cast<Differential*>(fr.first->back().get());
    assert(diff->getSetOfVariables().size() == 1);
    auto fr_without_diff = frac / fr.first->back();
    if (fr_without_diff->getId() == FRACTAL)
    {
        Fractal * fr_w_d = static_cast<Fractal*>(fr_without_diff.get());
        std::unique_ptr<Polynomial> res_pol;
        if (fr_w_d->canTurnIntoPolynomWithOpeningParentheses())
        {
            res_pol = fr_w_d->turnIntoPolynomWithOpeningParentheses(false);
            fr_without_diff.reset(res_pol.release());
        }
    }
    if (fr_without_diff->getId() == DEGREE)
    {
        if (static_cast<Degree*>(fr_without_diff.get())->canTurnIntoPolynomial())
        {
            Fractal deg_fr(fr_without_diff.get());
            fr_without_diff.reset(deg_fr.turnIntoPolynomWithOpeningParentheses(false).release());
        }

    }
    return fr_without_diff->antiderivative(*diff->getSetOfVariables().begin());
}
std::pair<abs_ex, int> getAntiderivAndVar(const abs_ex & frac)
{
    if (frac->getId() == DIFFERENTIAL)
    {
        Differential* diff = static_cast<Differential*>(frac.get());
        assert(diff->getSetOfVariables().size() == 1);
        return { diff->antiderivative(*diff->getSetOfVariables().begin()), *diff->getSetOfVariables().begin()};

    }
    assert(frac->getId() == FRACTAL);
    auto fr= static_cast<const Fractal*>(frac.get())->getFractal();
    fr.first->sort(&AbstractExpression::lessToSort);
    assert(fr.first->back()->getId() == DIFFERENTIAL);
    Differential* diff = static_cast<Differential*>(fr.first->back().get());
    assert(diff->getSetOfVariables().size() == 1);
    return {integrate(copy(frac)), *diff->getSetOfVariables().begin()};
}
abs_ex definiteIntegral(const abs_ex &frac, const abs_ex &from, const abs_ex &to)
{

    auto integ_and_var = getAntiderivAndVar(frac);
    if (integ_and_var.first == nullptr)
        return nullptr;
    auto left = std::move(integ_and_var.first);
    auto right = copy(left);
    int var = integ_and_var.second;
    std::map<int, abs_ex> left_replace;
    left_replace.insert({var, copy(to)});
    replaceSystemVariablesToExpressions(left, left_replace);
    left->setSimplified(false);
    std::map<int, abs_ex> right_replace;
    right_replace.insert({var, copy(from)});
    replaceSystemVariablesToExpressions(right, right_replace);
    right->setSimplified(false);
    return left - right;

}
std::unique_ptr<Fractal> operator*(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result(new Fractal(*left));
    result->setCoefficinet(result->getCoefficient() * right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.first)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.second)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;

}
std::unique_ptr<Fractal> operator*(std::unique_ptr<Fractal> &&left, const std::unique_ptr<Fractal> &right)
{
    left->setCoefficinet(left->getCoefficient() * right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.first)
        left->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.second)
        left->pushBackToDenominator(copy(it));
    left->simplify();
    return std::move(left);
}
std::unique_ptr<Fractal> operator*(std::unique_ptr<Fractal> &&left, std::unique_ptr<Fractal> &&right)
{
    left->setCoefficinet(left->getCoefficient() * right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.first)
        left->pushBackToNumerator(std::move(it));
    for (auto &it : *right_fr.second)
        left->pushBackToDenominator(std::move(it));
    left->simplify();
    return std::move(left);
}
std::unique_ptr<Fractal> operator*(const std::unique_ptr<Fractal> &left, std::unique_ptr<Fractal> &&right)
{
    return std::move(right) * left;
}
std::unique_ptr<Fractal> operator/(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result(new Fractal(*left));
    result->setCoefficinet(result->getCoefficient() / right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}



std::unique_ptr<Fractal> operator/(std::unique_ptr<Fractal> &&left, const std::unique_ptr<Fractal> &right)
{
    std::unique_ptr<Fractal> result = std::move(left);
    result->setCoefficinet(result->getCoefficient() / right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;

}

std::unique_ptr<Fractal> operator/(const std::unique_ptr<Fractal> &left, std::unique_ptr<Fractal> &&right)
{
    std::unique_ptr<Fractal> result(new Fractal(*left));
    result->setCoefficinet(result->getCoefficient() / right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(std::move(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(std::move(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator/(std::unique_ptr<Fractal> &&left, std::unique_ptr<Fractal> &&right)
{
    std::unique_ptr<Fractal> result = std::move(left);
    result->setCoefficinet(result->getCoefficient() / right->getCoefficient());
    auto right_fr = right->getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(std::move(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(std::move(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator*(const Fractal &left, const Fractal &right)
{
    std::unique_ptr<Fractal> result(new Fractal(left));
    result->setCoefficinet(result->getCoefficient() * right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.first)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.second)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator*(Fractal &&left, const Fractal &right)
{
    std::unique_ptr<Fractal> result(new Fractal(std::move(left)));
    result->setCoefficinet(result->getCoefficient() * right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.first)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.second)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator*(const Fractal &left, Fractal &&right)
{
    return std::move(right) * left;
}

std::unique_ptr<Fractal> operator*(Fractal &&left, Fractal &&right)
{
    std::unique_ptr<Fractal> result(new Fractal(std::move(left)));
    result->setCoefficinet(result->getCoefficient() * right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.first)
        result->pushBackToNumerator(std::move(it));
    for (auto &it : *right_fr.second)
        result->pushBackToDenominator(std::move(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator/(const Fractal &left, const Fractal &right)
{
    std::unique_ptr<Fractal> result(new Fractal(left));
    result->setCoefficinet(result->getCoefficient() / right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator/(Fractal &&left, const Fractal &right)
{
    std::unique_ptr<Fractal> result(new Fractal(std::move(left)));
    result->setCoefficinet(result->getCoefficient() / right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(copy(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(copy(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator/(const Fractal &left, Fractal &&right)
{
    std::unique_ptr<Fractal> result(new Fractal(left));
    result->setCoefficinet(result->getCoefficient() / right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(std::move(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(std::move(it));
    result->simplify();
    return result;
}

std::unique_ptr<Fractal> operator/(Fractal &&left, Fractal &&right)
{
    std::unique_ptr<Fractal> result(new Fractal(std::move(left)));
    result->setCoefficinet(result->getCoefficient() / right.getCoefficient());
    auto right_fr = right.getFractal();
    for (auto &it : *right_fr.second)
        result->pushBackToNumerator(std::move(it));
    for (auto &it : *right_fr.first)
        result->pushBackToDenominator(std::move(it));
    result->simplify();
    return result;
}
