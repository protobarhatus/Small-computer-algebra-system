#include "polynomial.h"
#include "assert.h"
#include "fractal.h"
#include "algorithm"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
#include "variable.h"
#include <cmath>
#include <assert.h>
#include "variablesdistributor.h"
#include "absolutevalue.h"
#include "trigonometrical_conversions.h"
Polynomial::Polynomial()
{

}
void Polynomial::simplify()
{
    SIM_IF_NEED
    for (auto &it : this->monomials)
        it->simplify();

    for (auto iter = this->monomials.begin(); iter != this->monomials.end();)
    {
        if (iter->get()->isPolynomial())
        {
            this->mergeWithoutSimplifiyng(static_cast<Polynomial*>(iter->get()->getFractal().first->begin()->get()), iter->get()->getCoefficient());
            iter = this->monomials.erase(iter);
        }
        else
            ++iter;
    }
    this->openParentheses();
    this->leadLikeMembers();


    this->monomials.sort(&Fractal::lessFrac);


    //after leading like members, polynomial turns into a 1 fractal if denominators are not 1.
    //if denominators of all members are 1 it remains polynomial to prevent infinit downcasting because
    //turning into one fractal with denominator 1 means that it's numerator is the same polynomial


    this->removeZeroes();
    //да мы это повторяем

    if (this->is_fractional_coefficients_allowed)
        return;

    if (this->canBecameFractal())
    {

        while(this->monomials.size() != 1)
        {
            *this->monomials.begin() = *this->monomials.begin()->get() + std::move((*(++this->monomials.begin())));
            this->monomials.erase(++this->monomials.begin());
        }

    }

    this->castTrigonometry();
    this->simplified = true;
}
void Polynomial::castTrigonometry()
{
    this->casted_trigonometry = false;
    this->castTrigonometricalFunctions();
    if (this->casted_trigonometry)
    {
        this->simplified = false;
        this->simplify();
    }
}
void Polynomial::removeZeroes()
{
    //in simplified()
    for (auto it = this->monomials.begin(); it != this->monomials.end();)
    {
        if (it->get()->isZero())
            it = this->monomials.erase(it);
        else
            ++it;
    }
}
void Polynomial::openParentheses()
{
    //in simplified()
    bool has_opened_parentheses = false;
    for (auto it = this->monomials.begin(); it != this->monomials.end();)
    {
        if (it->get()->canTurnIntoPolynomWithOpeningParentheses())
        {
            this->mergeWithoutSimplifiyng(it->get()->turnIntoPolynomWithOpeningParentheses());
            it = this->monomials.erase(it);
            has_opened_parentheses = true;
        }
        else
            ++it;
    }
    if (has_opened_parentheses)
        this->simplify();
}
void Polynomial::merge(Polynomial *polynomial)
{
    NONCONST
    for (auto &it : polynomial->monomials)
        this->pushBack(std::make_unique<Fractal>((it.get())));
    this->simplify();
}
void Polynomial::mergeWithoutSimplifiyng(Polynomial *polynomial, Number coe)
{
    NONCONST
    for (auto &it : polynomial->monomials)
        this->pushBack(*it.get() * Fractal(coe));
}
bool Polynomial::canBecameFractal()
{
    for (auto &it : this->monomials)
    {
        if (!it->isDenominatorEqualToOne())
            return true;
    }
    return false;
}
bool Polynomial::isZero()
{
    return this->monomials.empty();
}
bool Polynomial::canDowncastTo(AlgebraExpression expr)
{
    if ((expr == NUMBER || expr == FRACTAL) && this->isZero())
        return true;
    return this->monomials.size() == 1 && ((expr == FRACTAL) || this->monomials.begin()->get()->canDowncastTo(expr));
}
std::unique_ptr<AbstractExpression> Polynomial::downcastTo(AlgebraExpression expr)
{
    assert(canDowncastTo(expr));
    if (expr == NUMBER && this->isZero())
        return std::unique_ptr<AbstractExpression>(new Number(0));
    if (expr == FRACTAL && this->isZero())
        return Fractal::makeZeroFractal();
    if (expr == FRACTAL)
        return makeAbstractExpression(FRACTAL, this->monomials.begin()->get());
    else
        return this->monomials.begin()->get()->downcastTo(expr);
}
AlgebraExpression Polynomial::getId() const
{
    return POLYNOMIAL;
}


bool Polynomial::foldMonomials(std::list<std::unique_ptr<Fractal> >::iterator first, std::list<std::unique_ptr<Fractal> >::iterator second)
{
    //in simplified()
    Fractal * left = first->get();
    Fractal * right = second->get();
    if (!left->isLetterPartEquallTo(right))
        return false;

    first->get()->setCoefficinet(first->get()->getCoefficient() + second->get()->getCoefficient());
    return true;

}
void Polynomial::leadLikeMembers(std::list<std::unique_ptr<Fractal> >::iterator to_whom)
{
    //in simplified()
    auto it = to_whom;
    ++it;
    while(it != this->monomials.end())
    {
        if (this->foldMonomials(to_whom, it))
            it = this->monomials.erase(it);
        else
            ++it;
    }
}
void Polynomial::leadLikeMembers()
{
    //in simplified()
    for (auto it = this->monomials.begin(); it != this->monomials.end(); ++it)
        this->leadLikeMembers(it);
}
bool Polynomial::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == POLYNOMIAL);
    Polynomial * pol = static_cast<Polynomial*>(const_cast<AbstractExpression*>(&right));
    if (this->monomials.size() != pol->monomials.size())
        return this->monomials.size() < pol->monomials.size();
    std::list<std::unique_ptr<Fractal>>::iterator it1 = pol->monomials.begin();
    for (auto &it : this->monomials)
    {
        if (less(it.get(), it1->get()))
            return true;
        if (less(it1->get(), it.get()))
            return false;

    }
    return false;
}

bool Polynomial::operator==(AbstractExpression &right)
{
    if (right.getId() != POLYNOMIAL)
        return false;
    Polynomial * pol = static_cast<Polynomial*>(&right);
    if (this->monomials.size() != pol->monomials.size())
        return false;
    std::list<std::unique_ptr<Fractal>>::iterator it1 = pol->monomials.begin();
    for (auto &it : this->monomials)
    {
        if (*((AbstractExpression*)(it.get())) != *((AbstractExpression*)(*it1).get()))
            return false;
        ++it1;
    }
    return true;
}
bool Polynomial::isMonomial()
{
    return this->monomials.size() == 1;
}
std::unique_ptr<AbstractExpression> Polynomial::turnIntoMonomial()
{
    assert(this->isMonomial());
    Fractal * monomial = new Fractal(*(this->monomials.begin()->get()));
    return std::unique_ptr<AbstractExpression>(monomial);

}
Polynomial::Polynomial(const Polynomial & pol)
{
    for (auto &it : pol.monomials)
    {
        this->pushBack(std::unique_ptr<Fractal>(new Fractal(*it.get())));
    }
    this->is_fractional_coefficients_allowed = pol.is_fractional_coefficients_allowed;
    this->simplified = pol.simplified;
}
Polynomial::Polynomial(Polynomial && pol)
{
    for (auto &it : pol.monomials)
    {
        this->pushBack(std::move(it));
    }
    this->is_fractional_coefficients_allowed = pol.is_fractional_coefficients_allowed;
    this->simplified = pol.simplified;
}
Polynomial& Polynomial::operator=(Polynomial && pol)
{
    this->monomials.clear();
    for (auto &it : pol.monomials)
    {
        this->pushBack(std::move(it));
    }
    this->is_fractional_coefficients_allowed = pol.is_fractional_coefficients_allowed;
    this->simplified = pol.simplified;
    return *this;
}
Polynomial::Polynomial(AbstractExpression * fsum, AbstractExpression * secsum)
{
    this->pushBack(std::make_unique<Fractal>((fsum)));
    this->pushBack(std::make_unique<Fractal>((secsum)));
    this->simplify();
}
void Polynomial::addMonomial(Fractal* fractal)
{
    NONCONST
    this->pushBack(std::make_unique<Fractal>((*fractal)));
    this->simplify();
}
void Polynomial::addMonomial(std::unique_ptr<Fractal> &&monom)
{
    NONCONST
    this->pushBack(std::move(monom));
    this->simplify();
}
void Polynomial::merge(std::unique_ptr<Polynomial> && polynom)
{
    NONCONST
    for (auto &it : polynom->monomials)
    {
        this->pushBack(std::move(it));
    }
    this->simplify();
}
void Polynomial::mergeWithoutSimplifiyng(std::unique_ptr<Polynomial> && polynom, Number coe)
{
    NONCONST
    for (auto &it : polynom->monomials)
    {
        this->pushBack(*it.get() * Fractal(coe));
    }

}
Number Polynomial::reduce()
{
    NONCONST
    //as polynomial is simplified, all denominators are equally to one
    if (this->monomials.size() <= 1)
        return 1;
    long long int gcf = gcd(this->monomials.begin()->get()->getCoefficient().getNumerator(), (++this->monomials.begin())->get()->getCoefficient().getNumerator());
    for (auto it = ++(++this->monomials.begin()); it != this->monomials.end(); ++it)
    {
        gcf = gcd(gcf, it->get()->getCoefficient().getNumerator());
    }
    for (auto &it : monomials)
    {
        it.get()->setCoefficinet(it.get()->getCoefficient() / gcf);
    }
    return gcf;
}
std::set<int> Polynomial::getSetOfPolyVariables() const
{
    std::set<int> set;
    for (auto &it : this->monomials)
    {
        std::set<int> vars = it->getSetOfPolyVariables();
        set.insert(vars.begin(), vars.end());
    }
    return set;
}
std::set<int> Polynomial::getSetOfVariables() const
{
    std::set<int> set;
    for (auto &it : this->monomials)
    {
        std::set<int> vars = it->getSetOfVariables();
        set.insert(vars.begin(), vars.end());
    }
    return set;
}
Number Polynomial::getMaxDegreeOfVariable(int id)
{
    Number num = std::numeric_limits<long long int>::min();
    bool has_var = false;
    for (auto &it : monomials)
    {
        Number deg = it->getMaxDegreeOfVariable(id);
        if (deg.isCorrect())
        {
            has_var = true;
            if (deg.compareWith(num) > 0)
                num = deg;
        }
        else
            return Number::makeErrorNumber();

    }
    if (has_var)
        return num;
    return 0;
}
std::pair<std::unique_ptr<Polynomial>, std::unique_ptr<Polynomial>> Polynomial::divide(Polynomial * dividend)
{
    //copys divider and dividend and sort them in descending order of degree of first met variable that have only numeric degree. if it hasn't variables but has trigonometrical functions,
    //it works with tangent
    //otherwise just divide coefficients. If polynomials has different sets of variables, it returns nullptr. If max degree of all variables in divider is less then in dividend,
    //it returns nullptr. For correct work, both of them must be simplified. Also, it can work only with variable with numeric degree, if there are no common numeric, it returns
    //nullptr.

    int argument_variables_id = -1;
    std::set<int> dividers_set = this->getSetOfPolyVariables(), dividends_set = dividend->getSetOfPolyVariables();
    auto it1 = dividers_set.begin();
    auto it2 = dividends_set.begin();
    while (it1 != dividers_set.end() && it2 != dividends_set.end())
    {

        if (*it1 == *it2)
        {
            Number deg1 = this->getMaxDegreeOfVariable(*it1);
            Number deg2 = dividend->getMaxDegreeOfVariable(*it2);
            if (deg1.isCorrect() && deg2.isCorrect())
            {
                if (deg1.compareWith(deg2) >= 0)
                {
                    argument_variables_id = *it1;
                    break;
                }
            }
        }
        if (*it1 > *it2)
            ++it2;
        else
            ++it1;
    }
    if (argument_variables_id == -1)
        return std::pair<std::unique_ptr<Polynomial>, std::unique_ptr<Polynomial>>(nullptr, nullptr);
    std::unique_ptr<Polynomial> divinders_monomials(new Polynomial), dividends_monomials(new Polynomial);
    for (auto &it : this->monomials)
        divinders_monomials->pushBack(std::unique_ptr<Fractal>(new Fractal(it.get())));
    for (auto &it : dividend->monomials)
        dividends_monomials->pushBack(std::unique_ptr<Fractal>(new Fractal(it.get())));
    /*
    auto comp = [argument_variables_id](const std::unique_ptr<Fractal> & a, const std::unique_ptr<Fractal> & b)->bool {
        Number a_n = a->getMaxDegreeOfVariable(argument_variables_id);
        Number b_n = b->getMaxDegreeOfVariable(argument_variables_id) ;
        if (a_n.isCorrect() && b_n.isCorrect())
            return a_n.compareWith(b_n) > 0;
        return a_n.isCorrect() && !b_n.isCorrect();
    };

    //std::sort(divinders_monomials->monomials.begin(), divinders_monomials->monomials.end(), comp);
    divinders_monomials->monomials.sort(comp);
    //std::sort(dividends_monomials->monomials.begin(), dividends_monomials->monomials.end(), comp);
    dividends_monomials->monomials.sort(comp);*/
    divinders_monomials->is_fractional_coefficients_allowed = true;
    dividends_monomials->is_fractional_coefficients_allowed = true;
    std::list<std::unique_ptr<Fractal>> result_list;
    Number deg_of_max = divinders_monomials->getMaxDegreeOfVariable(argument_variables_id);
    Number deg_of_div = dividends_monomials->getMaxDegreeOfVariable(argument_variables_id);
    std::unique_ptr<AbstractExpression> arg_variable(new Variable(argument_variables_id));

    Polynomial coefficient_of_max_degree_in_dividend = dividend->getCoefficientOfMaxDegreeOfVariable(argument_variables_id);
    while (deg_of_max.isCorrect() && deg_of_max.compareWith(deg_of_div) >= 0)
    {
        Polynomial coe_of_max_degree = divinders_monomials->getCoefficientOfMaxDegreeOfVariable(argument_variables_id);
        std::unique_ptr<AbstractExpression> degree_diff(new Number(deg_of_max - deg_of_div));
        Degree var_multiplier = Degree(arg_variable, degree_diff);
        Fractal multiplier = *((coe_of_max_degree / &coefficient_of_max_degree_in_dividend) * Fractal(&var_multiplier));
        Polynomial subtrahend = *dividends_monomials * &multiplier;
        *divinders_monomials = *divinders_monomials - &subtrahend;
        deg_of_max = divinders_monomials->getMaxDegreeOfVariable(argument_variables_id);
        result_list.push_back(std::unique_ptr<Fractal>(new Fractal(std::move(multiplier))));
    }
    std::unique_ptr<Polynomial> result = std::make_unique<Polynomial>(std::move(result_list));
    result->simplify();
    return std::pair<std::unique_ptr<Polynomial>, std::unique_ptr<Polynomial>>(std::move(result), std::move(divinders_monomials));
}
Polynomial::Polynomial(std::list<std::unique_ptr<Fractal>> & list)
{
    for (auto &it : list)
        this->pushBack(std::make_unique<Fractal>((it.get())));

}
Polynomial::Polynomial(std::list<std::unique_ptr<Fractal>> && list)
{
    this->monomials = std::move(list);
}
std::list<Fractal*> Polynomial::getMonomialsPointers()
{
    NONCONST
    std::list<Fractal*> monoms;
    for (auto &it : this->monomials)
    {
        monoms.push_back(it.get());
    }
    return monoms;
}
void Polynomial::_qDebugOut()
{
    qDebug() << "POLYNOMIAL: <list of members>";
    for (auto &it : this->monomials)
    {
        it->_qDebugOut();
        qDebug() << "#####";
    }
    qDebug() << "END OF POLYNOMIAL;";
}
Polynomial Polynomial::getCoefficientOfMaxDegreeOfVariable(int id)
{
    //must be simplified
    Polynomial result;
    Number degree = this->getMaxDegreeOfVariable(id);
    if (!degree.isCorrect())
        return std::move(result);
    for (auto &it : this->monomials)
    {
        Number it_deg = it->getMaxDegreeOfVariable(id);
        if (it_deg.isCorrect())
        {
            if (it_deg == degree)
            {
                result.pushBack(it->getFractalWithoutVariable(id));
            }
        }
    }
    return result;
}

Polynomial Polynomial::operator+(AbstractExpression *expr)
{
    Polynomial result = *this;
    std::unique_ptr<Fractal> frac(new Fractal(expr));
    result.pushBack(std::move(frac));
    result.simplify();
    return result;
}
Polynomial Polynomial::operator-(AbstractExpression * expr)
{
    Polynomial result = *this;
    std::unique_ptr<Fractal> frac(new Fractal(expr));
    frac->setCoefficinet(frac->getCoefficient() * -1);
    result.pushBack(std::move(frac));
    result.simplify();
    return result;
}
Polynomial Polynomial::operator*(AbstractExpression * expr)
{
    Polynomial result = *this;
    std::unique_ptr<Fractal> frac(new Fractal(expr));
    for (auto &it : result.monomials)
    {
        it = *it * frac;
    }
    result.simplify();
    return result;
}
Fractal Polynomial::operator/(AbstractExpression * expr)
{
    Fractal result(this, expr);
    return result;
}
QString Polynomial::makeStringOfExpression()
{
    QString result = "(";
    for (auto &it : this->monomials)
    {
       //if (it->getCoefficient().compareWith(0) > 0)
            result += "+";

        result += it->makeStringOfExpression();
    }
    result += ")";
    if (result[1] == '+')
        result = result.remove(1, 1);
    return result;
}
std::unique_ptr<AbstractExpression> Polynomial::reduceCommonPart()
{
    NONCONST
    assert(this->monomials.size() >= 2);
    std::unique_ptr<AbstractExpression> common_part = this->monomials.begin()->get()->findCommonPart((++this->monomials.begin())->get());
    auto it = ++++this->monomials.begin();
    while (it != this->monomials.end())
    {
        common_part = static_cast<Fractal*>(common_part.get())->findCommonPart(it->get());
        ++it;
    }
    for (auto &it : this->monomials)
    {
        it = *it / *static_cast<Fractal*>(common_part.get());
    }
    return common_part;
}
bool Polynomial::isIrrationalSum()
{
    //polynoms with one summary can not exist becouse they downcast itself before. exceptions are variables with non-numeric degrees
    return this->getSetOfVariables().empty();
}
bool Polynomial::canGetRidOfIrrationalytyByMultiplying()
{
    if (!this->isIrrationalSum())
        return false;
    bool has_radical_with_degree_3_or_above = false;
    for (auto &it : this->monomials)
    {
        if (!it->getFractal().first->empty())
        {
            if (static_cast<Degree*>(it->getFractal().first->front().get())->getRootValue() >= 3)
            {
                if (!has_radical_with_degree_3_or_above)
                    has_radical_with_degree_3_or_above = true;
                else
                    return false;
            }
        }
    }
    return true;
}

std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression>> Polynomial::multiplyIrrationalSumOnAppropriateFormula()
{
    assert(this->canGetRidOfIrrationalytyByMultiplying());
    Polynomial* left = new Polynomial;
    Polynomial* right = new Polynomial;
    int ind_of_right = -1;
    int counter = -1;
    for (auto &it : this->monomials)
    {
        ++counter;
        if (!it->getFractal().first->empty())
            if (static_cast<Degree*>(it->getFractal().first->front().get())->getRootValue() >= 3 || ind_of_right == -1)
                ind_of_right = counter;
    }
    counter = -1;
    for (auto &it : this->monomials)
    {
        ++counter;
        if (counter == ind_of_right)
            right->pushBack(std::unique_ptr<Fractal>(new Fractal(it.get())));
        else
            left->pushBack(std::unique_ptr<Fractal>(new Fractal(it.get())));
    }

    std::unique_ptr<AbstractExpression> l_ptr(left), r_ptr(right);
  //  qDebug() << left->makeStringOfExpression();
  //  return std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression>>(*l_ptr * *l_ptr - *r_ptr * *r_ptr, *l_ptr - *r_ptr);
    std::unique_ptr<AbstractExpression> zero(new Number(0));
    long long int deg = static_cast<Degree*>(right->monomials.front()->getFractal().first->front().get())->getRootValue();
    if (deg % 2 == 0)
        return std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression>>((*l_ptr - *r_ptr)*(*l_ptr + *r_ptr)+zero, *l_ptr - *r_ptr);
    std::unique_ptr<AbstractExpression> second_multiplier(new Polynomial);
    bool plus = true;
    for (long long int i = deg - 1; i >= 0; --i)
    {
        std::unique_ptr<AbstractExpression> num_l(new Number(i));
        std::unique_ptr<AbstractExpression> a(new Degree(l_ptr, num_l));
        std::unique_ptr<AbstractExpression> num_r(new Number(deg - 1 - i));
        std::unique_ptr<AbstractExpression> b(new Degree(r_ptr, num_r));
        if (plus)
            second_multiplier = second_multiplier + a * b;
        else
            second_multiplier = second_multiplier - a * b;
        plus = !plus;
    }
    AbstractExpression * th = static_cast<AbstractExpression*>(this);
//    auto d = *th * *second_multiplier;

    return std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression>>(*th * *second_multiplier + zero, std::move(second_multiplier));
}

std::unique_ptr<AbstractExpression> Polynomial::toDegree(long long degree)
{
    std::unique_ptr<AbstractExpression> result(new Polynomial);
    Polynomial * result_ptr = static_cast<Polynomial*>(result.get());
    VariablesCombinations combinations(this->monomials.size());
    combinations[0] = degree;
    std::vector<std::list<std::unique_ptr<Fractal>>::iterator> monoms_vec;
    for (auto it = this->monomials.begin(); it != this->monomials.end(); ++it)
        monoms_vec.push_back(it);
    auto absToFrac = [](std::unique_ptr<AbstractExpression> && ptr)->std::unique_ptr<Fractal> {
        if (ptr->getId() == FRACTAL)
            return std::unique_ptr<Fractal> (static_cast<Fractal*>(ptr.release()));
        return std::unique_ptr<Fractal>(new Fractal(std::move(ptr)));
    };
    do
    {
        std::unique_ptr<AbstractExpression> monom = getDegrCoefficientPtr(degree, combinations);
        for (int i = 0; i < combinations.size(); ++i)
            monom = monom * takeDegreeOf(makeAbstractExpression(FRACTAL, monoms_vec[i]->get()), combinations[i]);
        result_ptr->pushBack(absToFrac(std::move(monom)));
    }while(inc(combinations));
    result_ptr->simplify();
    return result;
}




//from here all functions are for taking full root from polynom
std::unique_ptr<AbstractExpression> Polynomial::tryToTakeRoot(long long int root)
{
    auto set = this->getSetOfPolyVariables();
    if (this->getSetOfVariables() != set || root > 20 || root < 2 || (set.empty() && root > 2))
        return std::unique_ptr<AbstractExpression>(nullptr);
    for (auto &it : this->monomials)
        for (auto &it1 : *it->getFractal().first)
            if (Degree::getArgumentOfDegree(it1.get())->getId() == POLYNOMIAL || Degree::getDegreeOfExpression(it1.get())->getId() != NUMBER)
                return std::unique_ptr<AbstractExpression>(nullptr);
    std::unique_ptr<AbstractExpression> root_ptr;
    if (set.empty())
        root_ptr = this->tryToTakeRootOfNonVariablesPolynomial();
    else
        root_ptr = this->tryToTakeRootOfVariablesPolynomial(root);
    if (root_ptr != nullptr && root % 2 == 0)
        return std::unique_ptr<AbstractExpression>(new AbsoluteValue(std::move(root_ptr)));
    return root_ptr;
}
typedef std::vector<Number> DegreesPattern;
DegreesPattern getPatternOfMonomialsDegree(const Fractal * monom)
{
    assert(!monom->getSetOfPolyVariables().empty());
    const_cast<Fractal*>(monom)->sortVariablesInIncreasingOfTheirId();
    DegreesPattern degrees;
    for (auto &it : *monom->getFractal().first)
    {
        if (Degree::getArgumentOfDegree(it.get())->getId() > 0)
            degrees.push_back(*static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get()));
    }
    const_cast<Fractal*>(monom)->simplify();
    return degrees;
}
bool isEqually(const DegreesPattern & a, const DegreesPattern & b)
{
    if (a.size() != b.size())
        return false;
    for (DegreesPattern::const_iterator it1 = a.begin(), it2 = b.begin(); it1 != --a.end(); ++it1, ++it2)

       // if ((*it1._Ptr / *(++DegreesPattern::const_iterator(it1))._Ptr).compareWith(*it2._Ptr / *(++DegreesPattern::const_iterator(it2))._Ptr) != 0)
        if ((*it1) != (*it2))
            return false;
    return true;
}

std::list<Fractal*> getMonomialsSorted(std::list<Fractal*> && unsorted)
{
    unsorted.sort( [](const Fractal * a, const Fractal * b)->bool {
       auto set_a = a->getSetOfPolyVariables();
       auto set_b = b->getSetOfPolyVariables();
       if (set_a.size() < set_b.size())
           return true;
       if (set_a.size() > set_b.size())
           return false;
       if (set_a.empty() && set_b.empty())
           return Fractal::less(a, b);
       for (auto it1 = set_a.begin(), it2 = set_b.begin(); it1 != set_a.end(); ++it1, ++it2)
           if (*it1 < *it2)
               return true;
           else if (*it1 > *it2)
               return false;

       DegreesPattern degrs_a = getPatternOfMonomialsDegree(a), degrs_b = getPatternOfMonomialsDegree(b);
     //  if (isEqually(degrs_a, degrs_b))
     //  {
           Number sum_a = 0, sum_b = 0;
           for (auto &it : degrs_a)
               sum_a += it;
           for (auto &it : degrs_b)
               sum_b += it;

           return sum_a.compareWith(sum_b) == -1;
    });
    return std::move(unsorted);
}
bool operator==(const DegreesPattern & a, const DegreesPattern & b)
{
    if (a.size() != b.size())
        return false;
    for (auto it1 = a.begin(), it2 = b.begin(); it1 != a.end(); ++it1, ++it2)
        if (*it1 != *it2)
            return false;
    return true;
}
std::list<std::unique_ptr<Fractal>> groupPolynom(const std::list<Fractal*> & ungrouped)
{
    std::list<Fractal*>::const_iterator start_of_group = ungrouped.begin(), end_of_group = ungrouped.begin();
    std::list<std::unique_ptr<Fractal>> result;
    auto group = [&result](std::list<Fractal*>::const_iterator & start, std::list<Fractal*>::const_iterator & end) mutable ->void {
        Fractal var_group(1);
        for (auto &it : *(*start)->getFractal().first)
            if (Degree::getArgumentOfDegree(it.get())->getId() > 0)
                var_group.getFractal().first->push_back(makeAbstractExpression(it->getId(), it.get()));
        Polynomial remain_members;
        for (auto it = start; it != end; ++it)
            remain_members.addMonomial(*(*it) / var_group);
        result.push_back(var_group * Fractal(&remain_members));
    };
    //members without radicals are in begining
    while ((*start_of_group)->getSetOfPolyVariables().empty())
    {
        result.push_back(std::unique_ptr<Fractal>(new Fractal((*start_of_group))));
        ++start_of_group;
    }
    end_of_group = start_of_group;
    while (start_of_group != ungrouped.end())
    {
        while (end_of_group != ungrouped.end() &&
               (*end_of_group)->getSetOfPolyVariables() ==(*start_of_group)->getSetOfPolyVariables() &&
               getPatternOfMonomialsDegree((*end_of_group)) == getPatternOfMonomialsDegree((*start_of_group)))
            ++end_of_group;
        group(start_of_group, end_of_group);
        start_of_group = end_of_group;

    }
    return result;
}
//also functions for Polynomial::tryToTakeRootOfVariablesPolynomial(long long )
std::vector<Number> getArrayOfDegreesOfAnyWarInGroup(const std::list<std::unique_ptr<Fractal>> & group)
{
    int var = *group.begin()->get()->getSetOfPolyVariables().begin();
    std::vector<Number> degrees;
    for (auto &it : group)
        degrees.push_back(it->getMaxDegreeOfVariable(var));
    return degrees;
}
abs_ex openAbs(abs_ex rt)
{
    if (rt->getId() == ABSOLUTE_VALUE)
        return static_cast<AbsoluteValue*>(rt.get())->open();
    if (rt->getId() == FRACTAL)
    {
        Fractal * frac = static_cast<Fractal*>(rt.get());
        auto fr = frac->getFractal();
        for (auto &it : *fr.first)
            if (it->getId() == ABSOLUTE_VALUE)
            {
                abs_ex under_abs = static_cast<AbsoluteValue*>(it.get())->open();
                it = makeAbstractExpression(under_abs->getId(), under_abs.get());
            }
        assert(fr.second->size() == 0);
        return abs_ex(new Fractal(fr.first, frac->getCoefficient()));
    }
    return rt;
}
std::unique_ptr<Fractal> chooseAddictive(std::list<std::list<std::unique_ptr<Fractal>>> & groups, long long int root)
{
    auto areSetsIntersect = [](const std::set<int> & a, const std::set<int> & b) ->bool {
        auto it_a = a.begin();
        auto it_b = b.begin();
        while (it_a != a.end() && it_b != b.end())
        {
            if (*it_a == *it_b)
                return false;
            if (*it_a < *it_b)
                ++it_a;
            else if (*it_a > *it_b)
                ++it_b;
        }
    };
    bool can_choose_group_that_dont_intersect_with_others = false;
    int max_amount_of_vars = 0;
    int min_amount_of_vars_of_non_intersect_groups = std::numeric_limits<int>::max();

    std::list<std::list<std::unique_ptr<Fractal>>>::iterator group_with_max_amount_of_vars;
    std::list<std::list<std::unique_ptr<Fractal>>>::iterator result;
    for (auto it1 = groups.begin(); it1 != groups.end(); ++it1)
    {
        int amount = it1->begin()->get()->getSetOfPolyVariables().size();
        if (amount > max_amount_of_vars)
        {
            max_amount_of_vars = amount;
            group_with_max_amount_of_vars = it1;
        }
        bool is_intersect_with_anybody = false;
        for (auto it2 = groups.begin(); it2 != groups.end(); ++it2)
        {
            if (it1 == it2)
                continue;
            if (areSetsIntersect(it1->begin()->get()->getSetOfPolyVariables(), it2->begin()->get()->getSetOfPolyVariables()))
            {
                is_intersect_with_anybody = true;
                break;
            }
        }
        if (!is_intersect_with_anybody)
        {
            can_choose_group_that_dont_intersect_with_others = true;
            if (it1->begin()->get()->getSetOfPolyVariables().size() < min_amount_of_vars_of_non_intersect_groups)
            {
                result = it1;
                min_amount_of_vars_of_non_intersect_groups = it1->begin()->get()->getSetOfPolyVariables().size();
            }
            break;
        }
    }
    auto absToFrac = [](std::unique_ptr<AbstractExpression> && ptr)->std::unique_ptr<Fractal> {
        if (ptr->getId() == FRACTAL)
            return std::unique_ptr<Fractal> (static_cast<Fractal*>(ptr.release()));
        return std::unique_ptr<Fractal>(new Fractal(std::move(ptr)));
    };
    if (can_choose_group_that_dont_intersect_with_others)
        return absToFrac(openAbs(takeDegreeOf(makeAbstractExpression(result->begin()->get()->getId(), result->begin()->get()), Number(1) / root)));
    return absToFrac(openAbs(takeDegreeOf(makeAbstractExpression(group_with_max_amount_of_vars->begin()->get()->getId(), group_with_max_amount_of_vars->begin()->get()), Number(1) / root)));
}
void _debugGroup(std::list<std::list<std::unique_ptr<Fractal>>::iterator> & group)
{
    qDebug() << "#########";
    for (auto &it : group)
    {
        qDebug() << it->get()->makeStringOfExpression();
        qDebug() << "#";
    }
    qDebug() << "";
}
std::list<std::unique_ptr<Fractal>> getAddictivesFromSameGroup(std::list<std::list<std::unique_ptr<Fractal>>::iterator> & group, std::unique_ptr<Fractal> & degree_of_defined_addictive,
                                                               long long int root, std::unique_ptr<Fractal> & defined_addictive)
{
 //   _debugGroup(group);
    auto getSumOfDegrees = [](Fractal * a)->Number {
        DegreesPattern p = getPatternOfMonomialsDegree(a);
        Number sum = 0;
        for (auto &it : p)
            sum += it;
        return sum;
    };
    group.sort([&getSumOfDegrees](const std::list<std::unique_ptr<Fractal>>::iterator & a, const std::list<std::unique_ptr<Fractal>>::iterator & b)->bool {
       Number sum_a = getSumOfDegrees(a->get()), sum_b = getSumOfDegrees(b->get());
       //вернет true, если a больше, чтобы отсортировать по убыванию
       if (sum_a.compareWith(sum_b) > 0)
           return true;
       return false;
    });
    std::list<std::unique_ptr<Fractal>> result;
    result.push_back(std::move(defined_addictive));
    if (group.size() == 1)
        return result;
    Number def_sum = getSumOfDegrees(degree_of_defined_addictive.get());
    std::unique_ptr<Fractal> root_as_frac(new Fractal(root));

    auto absToFrac = [](std::unique_ptr<AbstractExpression> && ptr)->std::unique_ptr<Fractal> {
        if (ptr->getId() == FRACTAL)
            return std::unique_ptr<Fractal> (static_cast<Fractal*>(ptr.release()));
        return std::unique_ptr<Fractal>(new Fractal(std::move(ptr)));
    };
    DegreesPattern def_pattern = getPatternOfMonomialsDegree(degree_of_defined_addictive.get());
    auto sum = [](const DegreesPattern & a, const DegreesPattern & b)->DegreesPattern {
        DegreesPattern result = a;
        for (int i = 0; i < a.size(); ++i)
            result[i] += b[i];
        return result;
    };
    auto mult = [](const DegreesPattern & a, int b)->DegreesPattern {
        DegreesPattern result = a;
        for (auto &it : result)
            it *= b;
        return result;
    };
    auto in = [&def_pattern](const DegreesPattern & a)->bool {
        for (int i = 0; i < a.size(); ++i)
            if (a[i].compareWith(def_pattern[i]) < 0)
                return false;
        return true;
    };
    std::vector<std::list<std::unique_ptr<Fractal>>::iterator> result_vector;
    result_vector.push_back(result.begin());
    for (auto it = ++group.begin(); it != group.end() && in(getPatternOfMonomialsDegree((*it)->get())); ++it)
    {
        std::unique_ptr<AbstractExpression> monom = makeAbstractExpression(FRACTAL, (*it)->get());
        if (result.size() > 1)
        {
            VariablesCombinations combinations = VariablesCombinations(result.size());
            init(combinations, root);
            do
            {
                DegreesPattern summ = DegreesPattern(def_pattern.size());
                for (int i = 0; i < combinations.size(); ++i)
                    summ = sum(summ, mult(getPatternOfMonomialsDegree((*result_vector[i]).get()), combinations[i]));
                if (summ == getPatternOfMonomialsDegree((*it)->get()))
                {
                    std::unique_ptr<AbstractExpression> multiplicator = openAbs(takeDegreeOf(makeAbstractExpression(FRACTAL, (*result_vector.begin())->get()), combinations[0]));
                    for (int i = 1; i < combinations.size(); ++i)
                        if (combinations[i] != 0)
                            multiplicator = multiplicator * openAbs(takeDegreeOf(makeAbstractExpression(FRACTAL, (*result_vector[i]).get()), combinations[i]));
                    monom = monom - multiplicator * getDegrCoefficientPtr(root, combinations);
                }
            } while(inc(combinations));
        } /*
        for (auto it1 = result.begin(); it1 != result.end(); ++it1)
        {
            for (auto it2 = next(it1); it2 != result.end(); ++it2)
            {
                for (int i = 1; i <= root; ++i)
                {
                //    if (sum(mult(getPatternOfMonomialsDegree((*it1).get()), i), mult(getPatternOfMonomialsDegree((*it2).get()), root - i)) == getPatternOfMonomialsDegree((*it)->get()))
                  //      monom = monom - openAbs(takeDegreeOf(makeAbstractExpression(FRACTAL, (*it1).get()), i) * openAbs(takeDegreeOf(makeAbstractExpression(FRACTAL, (*it2).get()), root - i) *
                    //            getBynomialCoefficient(root, i);
                }
            }
        }*/
        result.push_back(absToFrac(std::move(monom)) / degree_of_defined_addictive / root_as_frac);
        result_vector.push_back(--result.end());
    }
    return result;
}
std::list<std::unique_ptr<Fractal>> getAddictivesInPolynom(std::list<std::list<std::unique_ptr<Fractal>>> & groups, std::list<std::unique_ptr<Fractal>> & monoms,
                                                                              long long int root)
{
    //выбирается член с максимальной степенью из любой группы (необходимое слагаемое - корень
    //группы д. б. отсортированы
    std::list<std::unique_ptr<Fractal>> result;
    auto absToFrac = [](std::unique_ptr<AbstractExpression> && ptr)->std::unique_ptr<Fractal> {
        if (ptr->getId() == FRACTAL)
            return std::unique_ptr<Fractal> (static_cast<Fractal*>(ptr.release()));
        return std::unique_ptr<Fractal>(new Fractal(std::move(ptr)));
    };
    std::unique_ptr<Fractal> defined_addictive = chooseAddictive(groups, root);
    std::set<int> vars = defined_addictive->getSetOfPolyVariables();
    DegreesPattern degrees = getPatternOfMonomialsDegree(defined_addictive.get());
    for (auto &it : degrees)
        it *= (root - 1);

    auto isFactorMatchingToAddictive = [&vars, &degrees](std::unique_ptr<Fractal> & factor) -> bool {
        int amount_of_matching = 0;
        std::set<int> factors_vars = factor->getSetOfPolyVariables();
        if (factors_vars.size() < vars.size())
            return false;
        if (factors_vars.size() == vars.size() && getPatternOfMonomialsDegree(factor.get()) == degrees)
            return false;
        //одни и те же переменные и там, и тут будут идти в одном и том же порядке
        auto vars_iterator_fact = factors_vars.begin();
        auto vars_iterator = vars.begin();
        auto degree_iterator = degrees.begin();
        while (vars_iterator_fact != factors_vars.end() && vars_iterator != vars.end())
        {
            if (*vars_iterator_fact == *vars_iterator)
            {
                if (factor->getMaxDegreeOfVariable(*vars_iterator_fact).compareWith(*degree_iterator) >= 0)
                {
                    ++amount_of_matching;
                    ++vars_iterator_fact;
                    ++vars_iterator;
                    ++degree_iterator;
                }
                else
                    return false;
            }
            else if (*vars_iterator < *vars_iterator_fact)
                ++vars_iterator;
            else
                ++vars_iterator_fact;
        }
        if (amount_of_matching == vars.size())
            return true;
        return false;
    };

    std::list<std::list<std::unique_ptr<Fractal>>::iterator> addictives_with_same_group;
    std::unique_ptr<Fractal> root_as_number(new Fractal(root));
    std::unique_ptr<Fractal> degree_of_defined_addictive = absToFrac(openAbs(takeDegreeOf(makeAbstractExpression(FRACTAL, defined_addictive.get()), root - 1)));
    for (auto it = monoms.begin(); it != monoms.end(); ++it)
    {
        if (!isFactorMatchingToAddictive(*it))
            continue;
        auto result_of_dividing = *(*it->get() / root_as_number.get()) / degree_of_defined_addictive;
        if (result_of_dividing->getSetOfPolyVariables() == vars)
            addictives_with_same_group.push_back(it);
        else
            result.push_back(std::move(result_of_dividing));
    }
    auto from_this_group = getAddictivesFromSameGroup(addictives_with_same_group, degree_of_defined_addictive, root, defined_addictive);
    for (auto &it : from_this_group)
        result.push_back(std::move(it));
    return result;
}
void sortGroup(std::list<std::unique_ptr<Fractal>> & group)
{
    group.sort([](const std::unique_ptr<Fractal> & a, const std::unique_ptr<Fractal> & b)->bool {
        DegreesPattern degr_a = getPatternOfMonomialsDegree(a.get()), degr_b = getPatternOfMonomialsDegree(b.get());
        Number sum_a = 0;
        for (auto &it : degr_a)
            sum_a += it;
        Number sum_b = 0;
        for (auto &it : degr_b)
            sum_b += it;
        //вернет true, если sum_a > sum_b, чтобы отсортировать по убыванию
        return sum_a.compareWith(sum_b) == 1;
     });
}
std::unique_ptr<AbstractExpression> Polynomial::tryToTakeRootOfVariablesPolynomial(long long root)
{

    //отсортировать слагаемые по группам множителей-переменных, среди них - по паттерну степеней (паттерн - их отношение друг к другу), среди них - по возрастанию суммы степеней
    auto monoms = groupPolynom(getMonomialsSorted(this->getMonomialsPointers()));
    //теперь вынести за скобки одинаковые группы переменных (чтобы в скобках их не было)
    //первое число - id переменной, набор чисел - набор переменных в минимальном множителе, где она встречается, набор итераторов - указатели на все слагаемые с этим набором,
    typedef std::list<std::unique_ptr<Fractal>>::iterator iterator;
    typedef std::map<int, std::pair<std::set<int>, std::vector<iterator>>> min_mults_type;
    min_mults_type min_mults_of_vars;


    //определение слагаемых в полиноме-результате.
    /* коммент для комента
    //число не учитывается, если это не коэффициент, только если это самостоятельное слагаемое
    //ищется минимальный набор для каждой переменной. если в исходном полиноме есть abc, bc и a (в какой-то степени), то a и bc - слагаемые в конечном полиноме. */
    //берется список переменных
    //для них для каждой из них ищется минимальное вхождение с другими переменными (числа и радикалы пока не рассматриваются, их степени тоже)
    //если в исходном полиноме есть свободное от переменных слагаемое (число или радикал), то такое есть в конечном. если в исходном его нет, то в конечном также его нет
    //если в полиноме, где есть радикал, дважды встречается одна и та же комбинация переменных (где одна из них с радикалом, а вторая - без), то значит, что в конечном полиноме есть
    //слагаемое - число.
    //Если таких несколько (и просто числа как слагаемого нет\
    или оно есть, и количество вхождений этого набора множителей превышает root раз), то ищется их произведение со слагаемым с
    //определенным набором переменных в степени (root-1) * определенную. Там необходимый набор будет в конечной степени, корень из них брать не надо.
            auto distributeSetOfVarsInAddend = [&min_mults_of_vars](std::unique_ptr<Fractal> & addend, iterator iter) mutable {
        auto vars = addend->getSetOfPolyVariables();
        for (auto &it : vars)
        {
            auto var = min_mults_of_vars.find(it);
            std::vector<iterator> vi;
            vi.push_back(iter);
            if (var == min_mults_of_vars.end())
                min_mults_of_vars.insert(std::pair<int, std::pair<std::set<int>, std::vector<iterator>>>(it, std::pair<std::set<int>, std::vector<iterator>>(vars, vi)));
            else
            {
                if (var->second.first.size() > vars.size())
                    var->second = std::pair<std::set<int>, std::vector<iterator>>(vars, vi);
                else if (var->second.first == vars)
                    var->second.second.push_back(iter);
            }
        }
    };
    std::list<std::unique_ptr<Fractal>> monoms_without_vars;
    for (auto it = monoms.begin(); it != monoms.end(); ++it)
    {
        distributeSetOfVarsInAddend(*it, it);
        if (it->get()->getSetOfPolyVariables().empty())
            monoms_without_vars.push_back(std::unique_ptr<Fractal>(new Fractal(it->get())));
    }
    //теперь нужно проверить, нет ли совпадений среди мин. вхождений переменных. если они есть - удалить лишние
    for (min_mults_type::iterator it1 = min_mults_of_vars.begin(); it1 != min_mults_of_vars.end(); ++it1)
    {
        for (auto it2 = std::next(it1); it2 != min_mults_of_vars.end();)
        {
            if (it2->second.first == it1->second.first)
                it2 = min_mults_of_vars.erase(it2);
            else
                ++it2;
        }
    }
    //теперь нужно определить слагаемые (которые содержат переменные) в конечном многочлене. Для этого берем член с максимальной степенью из любой группы (корень степени root из него),
    //находим множители, где он находится в степени root - 1 и делим первое на второе и на root (т. к. биномиальные коэффициенты). для одной и той же группы сложнее - нужно учитывать
    //возможность сложения одинаковых степеней, полученных разным путем
    std::list<std::list<std::unique_ptr<Fractal>>> groups;
    std::list<std::unique_ptr<Fractal>> precisely_defined_addictives;
    for (auto &it1 : min_mults_of_vars)
    {
        groups.push_back(std::list<std::unique_ptr<Fractal>>(std::list<std::unique_ptr<Fractal>>()));
        for (auto &it2 : it1.second.second)
            groups.back().push_back(std::unique_ptr<Fractal>(new Fractal(it2->get())));

    }
    for (auto &it : groups)
        sortGroup(it);
    //если корень четный, то максимальная степень в каждой группе будет полным квадратом, следовательно - иметь знак +
    if (root % 2 == 0)
    {
        for (auto &it : groups)
        {
            if (it.begin()->get()->getCoefficient().getNumerator() < 0)
                return std::unique_ptr<AbstractExpression>(nullptr);
        }
    }
    auto addictives = getAddictivesInPolynom(groups, monoms, root);
    std::unique_ptr<AbstractExpression> result_polynom = std::unique_ptr<AbstractExpression>(new Polynomial(std::move(addictives)))
            + takeDegreeOf(std::unique_ptr<AbstractExpression>(new Polynomial(std::move(monoms_without_vars))), Number(1) / root);
    std::unique_ptr<AbstractExpression> zero(new Number(0));
    if (*this == *(takeDegreeOf(result_polynom, root) + zero))
        return result_polynom;

    return std::unique_ptr<AbstractExpression>(nullptr);
}







//square root
std::unique_ptr<AbstractExpression> Polynomial::tryToTakeRootOfNonVariablesPolynomial()
{
    auto monoms = this->getMonomialsPointers();
    std::list<Fractal*> monoms_without_free_member;
    Fractal* free_member = nullptr;
    bool has_free_member = false;
    bool has_non_square_root = false;
    for (auto &it : monoms)
        if (it->getFractal().first->size() == 0)
        {
            has_free_member = true;
            free_member = it;
        }
        else
        {
            for (auto &it1 : *it->getFractal().first)
                if (static_cast<Number*>(Degree::getDegreeOfExpression(it1.get()).get())->getDenominator() > 2)
                {
                    has_non_square_root = true;
                    break;
                }
            monoms_without_free_member.push_back(it);
        }
    if (!has_free_member || has_non_square_root || free_member->getCoefficient().getNumerator() < 0)
        return std::unique_ptr<AbstractExpression>(nullptr);
    std::unique_ptr<AbstractExpression> result;
    std::unique_ptr<AbstractExpression> two_p(new Number(2));
    std::unique_ptr<AbstractExpression> half(new Number(1, 2));
    std::unique_ptr<AbstractExpression> minus(new Number(-1));
    std::unique_ptr<AbstractExpression> zero(new Number(0));
    Number hl(1, 2);
    auto frac_to_underroot = [](Fractal * frac)->Number {
        Number n = frac->getCoefficient() * frac->getCoefficient();
        for (auto &it : *frac->getFractal().first)
            n *= *static_cast<Number*>(Degree::getArgumentOfDegree(it.get()));
        return n;
    };
    auto takeUnderRootOfFractalWithoutCoefficient = [](Fractal * frac)->Number {
        Number n = 1;
        for (auto &it : *frac->getFractal().first)
            n *= *static_cast<Number*>(Degree::getArgumentOfDegree(it.get()));
        return n;
    };
    auto numToAbs = [](Number num)->std::unique_ptr<AbstractExpression> {
        return std::unique_ptr<AbstractExpression>(new Number(num));
    };
    auto intToAbs = [](long long int num)->std::unique_ptr<AbstractExpression> {
        return std::unique_ptr<AbstractExpression> (new Number(num));
    };
    double lli_limit = std::numeric_limits<long long int>::max();
    //просто определенные формулы для каждого из вариантов размера исходного. делить на кучу функций нет смысла, так
    //что будет в одной большой, да простит меня макконнел
    if (monoms.size() == 2)
    {
        Number a = free_member->getCoefficient();
        bool is_negative = (*monoms_without_free_member.begin())->getCoefficient().getNumerator() < 0;
        Number b = frac_to_underroot(*monoms_without_free_member.begin());
        bool s = false;
        long long int rtex = (a*a-b).getNumerator();
        long long int undr;
        if (rtex >= 0)
            undr = root((a*a-b).getNumerator(), 2, s);
        if (!s)
            return std::unique_ptr<AbstractExpression>(nullptr);
        std::unique_ptr<AbstractExpression> a_p (new Number(a));
        std::unique_ptr<AbstractExpression> undr_p (new Number(undr));

        std::unique_ptr<AbstractExpression> l (new Degree((a_p + undr_p) / two_p, half));
        std::unique_ptr<AbstractExpression> r (new Degree((a_p - undr_p) / two_p, half));
        if (is_negative)
            return l - r;
        return l + r;

    }
    if (monoms.size() == 3) //3 может быть, если в конечном выражении 4 слагаемых при определенных условиях. решать уравнения для этого слишком сложно, по этому присутствует элемент перебора
    {
        long long int a_coefficient = (*this->monomials.begin()).get()->getCoefficient().getNumerator();
        long long int b_coefficient = (*(++this->monomials.begin())).get()->getCoefficient().getNumerator();
        long long int a = takeUnderRootOfFractalWithoutCoefficient((*this->monomials.begin()).get()).getNumerator();
        long long int b = takeUnderRootOfFractalWithoutCoefficient((*(++this->monomials.begin())).get()).getNumerator();
        long long int free_member = (*(this->monomials.back())).getCoefficient().getNumerator();
        if (a >= lli_limit / b ||
                a >= free_member || b >= free_member || a * b >= free_member || a + b + a * b >= free_member ||
                free_member >= pow(2, 60))
                return std::unique_ptr<AbstractExpression>(nullptr);
        long long int c = a * b;
        if (b > a)
        {
            std::swap(a, b);
            std::swap(a_coefficient, b_coefficient);
        }
        //условия, которыми руководствуемся при переборе: free_member = cf^2 + ca^2*a + cb^2*b + cc^2 * c; cf * cc = ca * cb
        //края определяются производной. Для них нужно побочное вычисление параметра. Единицы добавлены для предотвращения ошибки на единицу из-за округления
        //подсчет края для коэффициента при sqrt(a) (в конечном)
        double z_v  = sqrt(sqrt(c*a*free_member) / double (a *  c) - 1 / double (a));
        long long int edge_ca = sqrt(double(double(free_member) * z_v * z_v - double(b) * z_v * z_v - double(c) * pow(z_v, 4))/double(1 + double(a) * z_v * z_v)) + 1;
        //подсчет края для свободного члена в конечном
        double x_v = sqrt(sqrt(a*b*free_member)/double(a) - b);
        long long int edge_cf = sqrt(double(-x_v*x_v*(a*x_v*x_v+c-free_member))/double(b+x_v*x_v)) + 1;
        auto cb_and_cc_formula = [&a, &b, &c, &free_member](long long int cf, long long int ca)->std::pair<bool, std::pair<long long int, long long int>> {
            long long int p_b = sqrt(double(free_member - cf * cf - ca * ca * a) / double(b + double(ca * ca) / double (cf * cf) * c));
            long long int m = p_b * ca;
            if ((m / cf) * cf == m && m != 0)
                return std::pair<bool, std::pair<long long int, long long int>>(true, std::pair<long long int, long long int>(p_b, m / cf));
            ++p_b;
            m = p_b * ca;
            if ((m / cf) * cf == m)
                return std::pair<bool, std::pair<long long int, long long int>>(true, std::pair<long long int, long long int>(p_b, m / cf));
            return std::pair<bool, std::pair<long long int, long long int>>(false, std::pair<long long int, long long int>(0, 0));
        };
        long long int cf = 1, ca = 1, cb = 1, cc = 1;
        auto count = [&a, &b, &c](long long int cf, long long int ca, long long int cb, long long int cc) {
            return cf * cf + ca * ca * a + cb * cb * b + cc * cc * c;
        };

        auto f_r = cb_and_cc_formula(cf, ca);
        bool have_b_coe_appropriated = f_r.first;
        if (have_b_coe_appropriated)
        {
            cb = f_r.second.first;
            cc = f_r.second.second;
        }
        long long int current_val;
        bool should_continue = true;
        while (true)
        {
            while (!have_b_coe_appropriated || (current_val = count(cf, ca, cb, cc)) != free_member || should_continue)
            {
                should_continue = false;
                if (cf == edge_cf)
                {
                    if (ca == edge_ca)
                        return std::unique_ptr<AbstractExpression>(nullptr);
                    ++ca;
                    cf = 1;
                }
                else
                        ++cf;

                auto form_res = cb_and_cc_formula(cf, ca);
                have_b_coe_appropriated = form_res.first;
                if (have_b_coe_appropriated)
                {
                    cb = form_res.second.first;
                    cc = form_res.second.second;
                }

            }
            if (- cf * ca * 2 + 2 * cb * cc * b == a_coefficient && 2 * cf * cb - 2 * ca * cc * a == b_coefficient)
                result = intToAbs(cf) - intToAbs(ca) * takeDegreeOf(a, hl) + intToAbs(cb) * takeDegreeOf(b, hl) + intToAbs(cc) * takeDegreeOf(c, hl);
            else if (-cf * cb * 2 + 2 * ca * cc * a == b_coefficient && 2 * cf * ca - 2 * cb * cc * b == a_coefficient)
                result = intToAbs(cf) + intToAbs(ca) * takeDegreeOf(a, hl) - intToAbs(cb) * takeDegreeOf(b, hl) + intToAbs(cc) * takeDegreeOf(c, hl);
            else if (-cf * ca * 2 + 2 * cb * cc * b == a_coefficient && -cf * cb * 2 + 2 * ca * cc * a == b_coefficient)
                result = intToAbs(cc) * takeDegreeOf(c, hl) - intToAbs(cf) + intToAbs(ca) * takeDegreeOf(a, hl) + intToAbs(cb) * takeDegreeOf(b, hl);
            else
                result = intToAbs(cf) + intToAbs(ca) * takeDegreeOf(a, hl) + intToAbs(cb) * takeDegreeOf(b, hl) - intToAbs(cc) * takeDegreeOf(c, hl);
            if (*this == *(takeDegreeOf(result, two_p) + zero))
                return result;
            should_continue = true;

        }

    }
    if (monoms.size() == 4) //4 может быть, если в выражении-корне (конечном) либо 3, либо 4 члена. сейчас проверяется на 3
    {
        //скобки для области видимости переменной
        {
            auto isNeg = [](Number a, Number b)->bool {
                return a.getNumerator() < 0 && b.getNumerator() < 0;
            };
            Number nx = frac_to_underroot(*monoms_without_free_member.begin()), ny =
                    frac_to_underroot(*(++monoms_without_free_member.begin())),
                    nz = frac_to_underroot(*(++++monoms_without_free_member.begin()));
            std::unique_ptr<AbstractExpression> x = takeDegreeOf(nx,hl),
                    y = takeDegreeOf(ny, hl),
                    z = takeDegreeOf(nz, hl);
            auto formula = [&two_p, &half, &minus](abs_ex & mult1, abs_ex & mult2, abs_ex & divider, bool is_negative)->std::unique_ptr<AbstractExpression> {
                std::unique_ptr<AbstractExpression> res (new Degree(mult1 * mult2 / divider / two_p,
                                                                    makeAbstractExpression(NUMBER, half.get())));
                if (is_negative)
                    res = res * minus;
                return res;
            };
            result = formula(x, y, z, isNeg(nx, ny)) + formula(x, z, y, isNeg(nx, nz)) + formula(z, y, x, isNeg(nz, ny));
            if (*this == *(takeDegreeOf(result, two_p) + zero))
                return result;
        }
        //если это не сработало, то проверяется другой случай. его описание:
        //sqrt (a*sqrt(k)+b*sqrt(s)+c*sqrt(k*s)+d) == x*sqrt(k)+y*sqrt(s)+z*sqrt(k*s)+t
        //соответственно, система:
        //x^2*k + y^2*s + z^2*k*s + t^2 == d;
        //x*t + y*z*s = a/2;
        //y*t + x*z*k = b/2;
        //z*t+x*y = c/2.
        //где x, y, z, t - неизвестны
        //решать ее невозможно из-за слишком большой степени, поэтому, выразив два члена, можно перебирать по оставшимся 2
        //выражаем y и t, перебираем по x и z

        long long int a = (*monoms_without_free_member.begin())->getCoefficient().getNumerator();
        long long int b = (*(++monoms_without_free_member.begin()))->getCoefficient().getNumerator();
        long long int c = (*(++++monoms_without_free_member.begin()))->getCoefficient().getNumerator();
        long long int k = takeUnderRootOfFractalWithoutCoefficient(*monoms_without_free_member.begin()).getNumerator();
        long long int s = takeUnderRootOfFractalWithoutCoefficient(*(++monoms_without_free_member.begin())).getNumerator();
        long long int ks = takeUnderRootOfFractalWithoutCoefficient(*(++++monoms_without_free_member.begin())).getNumerator();
        long long int d = free_member->getCoefficient().getNumerator();
        if (ks < k)
        {
            std::swap(k, ks);
            std::swap(a, c);
        }
        if (ks < s)
        {
            std::swap(s, ks);
            std::swap(b, c);
        }
        if (k < s)
        {
            std::swap(s, k);
            std::swap(a, b);
        }
        if (k >= lli_limit / s || ks != k * s || k + s >= lli_limit - ks || k + s + ks >= d || d >= pow(2, 60) ||
                a % 2 != 0 || b % 2 != 0 || c % 2 != 0 || d < 12 || c >= pow(2, 31))
            return abs_ex(nullptr);
        auto d_formula = [&a, &b, &c, &k, &s, &ks](long long int x, long long int y, long long z, long long int t) {
            return x*x*k + y*y*s + z*z*ks + t*t;
        };

        a /= 2;
        b /= 2;
        c /= 2;

        //9 - сумма минимально возможных t^2 + z^2*ks + y^2 * s (мин. t, z, y = 1, мин. s = 2, мин. k = 3)
        long long int x_edge = sqrt(double((d - 9)) / k) + 1;
        long long int z_edge = sqrt(double((d - 6)) / ks) + 1;
        long long int x = -x_edge, z = -z_edge, y, t;
        //возвращает подходят ли x и z значения и y и t, если да
        auto fits = [&b, &c, &k, &d, &d_formula](long long int x, long long int z)->std::pair<bool, std::pair<long long int, long long int>>
        {
            long long int undrt1 = 4*x*z*(k*x*z - b);
            long long int undrt2 = c*c;
            if (undrt1 > undrt2)
                return std::pair<bool, std::pair<long long int, long long int>>(false, std::pair<long long int, long long int>(0, 0));
            long long int underroot = undrt1 + undrt2;
            bool s = false;
            long long int rt = root(underroot, 2, s);
            if (!s)
                return std::pair<bool, std::pair<long long int, long long int>>(false, std::pair<long long int, long long int>(0, 0));
            long long int var1 = c + rt, var2 = c - rt;
            //checking var1
            if (var1 % 2 == 0)
                if ((var1 /= 2) % x == 0 && std::abs(var1) >= std::abs(x))
                        if ((c - var1) % z == 0 && std::abs(c - var1) >= std::abs(z))
                        {
                            long long int t = (c - var1) / z;
                            long long int y = var1 / x;
                            if (d_formula(x, y, z, t) == d)
                                return std::pair<bool, std::pair<long long int, long long int>>(true, std::pair<long long int, long long int>(y, t));
                        }
            //checking var2
            if (var2 % 2 == 0)
                if ((var2 /= 2) % x == 0 && std::abs(var2) >= std::abs(x))
                        if ((c - var2) % z == 0 && std::abs(c - var2) >= std::abs(z))
                        {
                            long long int t = (c - var2) / z;
                            long long int y = var2 / x;
                            if (d_formula(x, y, z, t) == d)
                                return std::pair<bool, std::pair<long long int, long long int>>(true, std::pair<long long int, long long int>(y, t));
                        }
            return std::pair<bool, std::pair<long long int, long long int>>(false, std::pair<long long int, long long int>(0, 0));
        };
        auto is_result = [&k, &s, &a, &b, &c](long long x, long long y, long long z, long long t) {
            return x*t + y*z*s == a && y*t + x*z*k == b && z*t + x*y == c;
        };

        while (true)
        {
            if (x != 0 && z != 0)
            {
                auto fits_res = fits(x, z);
                if (fits_res.first)
                {
                    y = fits_res.second.first;
                    t = fits_res.second.second;
                    if (is_result(x, y, z, t))
                        return intToAbs(x) * takeDegreeOf(k, hl) + intToAbs(y) * takeDegreeOf(s, hl) + intToAbs(z) * takeDegreeOf(ks, hl) + intToAbs(t);
                }
            }
            if (x == x_edge)
            {
                x = 0;
                if (z == z_edge)
                    return abs_ex(nullptr);
                ++z;
            }
            else
                ++x;
        }

    }
    if (monoms.size() == 7) //7 может быть, если в результате либо 4, либо 5 членов. я закончу на 4
    {
        //true если перед корнем "-"
        typedef std::pair<long long int, bool> underroots_t;
        std::vector<underroots_t> underroots;
        for (auto &it : monoms_without_free_member)
            underroots.push_back(underroots_t(frac_to_underroot(it).getNumerator() / 4, it->getCoefficient().getNumerator() < 0));

        std::sort(underroots.begin(), underroots.end(), [](underroots_t a, underroots_t b)->bool { return a.first > b.first;});
        //x > y > z > t
        //далее возможны два варианта уравнений, нужно проработать оба
        long long int x = 0, y = 0, z = 0, t = 0;
        auto checkAns = [&underroots](long long int x, long long int y, long long int z, long long int t)->bool {
            int amount_of_matching = 0;
            for (auto &it : underroots)
                if (it.first == x * y || it.first == x * z || it.first == x * t || it.first == y * z || it.first == y * t || it.first == z * t)
                    ++amount_of_matching;
            return amount_of_matching == 6;
        };
        //сначала первая система: xy = u[0], xz = u[1], xt = u[2], yz = u[3]
        bool s = false;
        x = root(underroots[0].first * underroots[1].first / underroots[3].first, 2, s);
        if (s)
        {
            y = underroots[0].first / x;
            z = underroots[1].first / x;
            t = underroots[2].first / x;
        }
        if (checkAns(x, y, z, t))
        {
            //достаточно двух отрицательных, чтобы считать x отрицательным
            bool is_x_negative = (underroots[0].second && underroots[1].second) || (underroots[0].second && underroots[2].second) || (underroots[1].second && underroots[2].second);
         /*   result = (is_x_negative ? takeDegreeOf(x, hl) * minus : takeDegreeOf(x, hl)) + ((is_x_negative && !underroots[0].second) || (!is_x_negative && underroots[0].second) ?
                        takeDegreeOf(y, hl) * minus : takeDegreeOf(y, hl)) + ((is_x_negative && !underroots[1].second) || (!is_x_negative && underroots[1].second) ?
                        takeDegreeOf(z, hl) * minus : takeDegreeOf(z, hl)) + ((is_x_negative && underroots[2] > 0) || (!is_x_negative && underroots[2] < 0) ?
                        takeDegreeOf(t, hl) * minus : takeDegreeOf(t, hl));*/
            result = (is_x_negative ? takeDegreeOf(x, hl) * minus : takeDegreeOf(x, hl)) + (is_x_negative ^ underroots[0].second ? takeDegreeOf(y, hl) * minus : takeDegreeOf(y, hl)) +
                    (is_x_negative ^ underroots[1].second ? takeDegreeOf(z, hl) * minus : takeDegreeOf(z, hl)) + (is_x_negative ^ underroots[2].second ? takeDegreeOf(t, hl) * minus :
                                                                                                                                                         takeDegreeOf(t, hl));
            assert(*this == *(takeDegreeOf(result, two_p) + zero));
            return result;

        }
        //теперь вторая: xy = u[0], xz = u[1], yz = u[2], xt = u[3]
        s = false;
        x = root(underroots[0].first * underroots[1].first / underroots[2].first, 2, s);
        if (!s)
            return abs_ex(nullptr);
        y = underroots[0].first / x;
        z = underroots[1].first / x;
        t = underroots[3].first / x;
        if (checkAns(x, y, z, t))
        {
            bool is_x_negative = (underroots[0].second && underroots[1].second) || (underroots[0].second && underroots[3].second) || (underroots[1].second || underroots[3].second);

            result = (is_x_negative ? takeDegreeOf(x, hl) * minus : takeDegreeOf(x, hl)) + (is_x_negative ^ underroots[0].second ? takeDegreeOf(y, hl) * minus : takeDegreeOf(y, hl)) +
                    (is_x_negative ^ underroots[1].second ? takeDegreeOf(z, hl) * minus : takeDegreeOf(z, hl)) + (is_x_negative ^ underroots[3].second ? takeDegreeOf(t, hl) * minus :
                                                                                                                                                         takeDegreeOf(t, hl));

            assert(*this == *(takeDegreeOf(result, two_p) + zero));
            return result;
        }


    }
    return std::unique_ptr<AbstractExpression>(nullptr);
}
void Polynomial::pushBack(std::unique_ptr<Fractal> &&fractal)
{
    NONCONST
    this->monomials.push_back(std::move(fractal));
}

double Polynomial::getApproximateValue()
{
    double value  = 0;
    for (auto &it : this->monomials)
        value += it->getApproximateValue();
    return value;
}

int Polynomial::getPositionRelativelyZeroIfHasVariables()
{
    //сначала проверка, является ли это суммой всегда положительных или всегда отрицательных значений
    int monoms_pos = this->monomials.begin()->get()->getPositionRelativelyZero();
    if (monoms_pos != 0)
    {
        for (auto it = ++this->monomials.begin(); it != this->monomials.end(); ++it)
        {
            if (it->get()->getPositionRelativelyZero() != monoms_pos)
            {
                monoms_pos = -2;
                break;
            }
        }
        if (monoms_pos != -2)
            return monoms_pos;

    }
    auto vars = this->getSetOfPolyVariables();

    //теперь проверка на то, является ли это параболой или прямой и пересекает ли она 0 в области определения переменной, если да
    //РЕШЕНИЕ КВАДРАТНОГО УРАВНЕНИЯ ЗДЕСЬ ПЕРЕНЕСТИ В ОТДЕЛЬНЫЙ МОДУЛЬ ДЛЯ УРАВНЕНИЯ, НЕ ДУБЛИРОВАТЬ КОД. ЭТОТ ОГРОМНЫЙ БЛОК ЗДЕСЬ ВРЕМЕННО
    if (vars.size() == 1 && this->getMaxDegreeOfVariable(*vars.begin()).compareWith(2) == 0)
    {
        int var_id = *vars.begin();
        bool succes = true;
        std::unique_ptr<AbstractExpression> var(new Variable(getVariable(var_id)));

        std::unique_ptr<AbstractExpression> a, b, c;
        a = std::unique_ptr<AbstractExpression>(new Number(0));
        b = std::unique_ptr<AbstractExpression>(new Number(0));
        c = std::unique_ptr<AbstractExpression>(new Number(0));
        for (auto &it : this->monomials)
        {
            //может возникнуть подозрение, что узнавать так о степени переменной некоректно, ведь это запрос на максимальную степень. Однако в дроби может быть только одно вхождение
            //переменной
            Number degr = it->getMaxDegreeOfVariable(var_id);
            if (degr.compareWith(2) == 0)
                a = a + it->getFractalWithoutVariable(var_id);
            else if (degr.isOne())
                b = b + it->getFractalWithoutVariable(var_id);
            else if (degr.isZero())
                c = c + makeAbstractExpression(FRACTAL, it.get());
            else
            {
                succes = false;
                break;
            }
        }
        bool is_a_zero = false;
        if (a == nullptr)
        {
            a = std::unique_ptr<AbstractExpression>(new Number(0));
            is_a_zero = true;
        }
        if (b == nullptr)
            b = std::unique_ptr<AbstractExpression>(new Number(0));
        if (c == nullptr)
            c = std::unique_ptr<AbstractExpression>(new Number(0));
        if (succes)
        {
            std::unique_ptr<AbstractExpression> four(new Number(4));
            std::unique_ptr<AbstractExpression> D = b * b - four * a * c;
            if (D->getPositionRelativelyZero() < 0)
                return a->getPositionRelativelyZero();
            std::unique_ptr<AbstractExpression> minus(new Number(-1));
            std::unique_ptr<AbstractExpression> two(new Number(2));
            std::unique_ptr<AbstractExpression> hl(new Number(1, 2));
            std::unique_ptr<AbstractExpression> root1_ptr = is_a_zero ? (minus * c / b) : (minus * b + takeDegreeOf(D, hl)) / two / a;
            std::unique_ptr<AbstractExpression> root2_ptr = is_a_zero ? (minus * c / b) : (minus * b - takeDegreeOf(D, hl)) / two / a;
            double root1 = root1_ptr->getApproximateValue(), root2 = root2_ptr->getApproximateValue();
            auto definition = VariablesDistributor::getVariablesDefinition(var_id);
            auto max = [](double a, double b) { return a > b ? a : b;};
            auto min = [](double a, double b) { return a < b ? a : b;};
            if (max(root1, root2) <= definition->getMinValue())
                return is_a_zero ? b->getPositionRelativelyZero() : a->getPositionRelativelyZero();
            if (min(root1, root2) >= definition->getMaxValue())
                return is_a_zero ? b->getPositionRelativelyZero() * -1 : a->getPositionRelativelyZero();
        }
    }
    //если имеется линейная функция от нескольких переменных, то можно очень просто посчитать ее минимум и максимум на области определения переменных в ней.
    //в каждом множителе может быть только 1 переменная
    auto max_rule = [](VariablesDefinition * def)->double { return def->getMaxValue();};
    auto min_rule = [](VariablesDefinition * def)->double { return def->getMinValue();};
    double max_value = 0, min_value = 0;
    for (auto &it : this->monomials)
    {
        auto monoms_vars = it->getSetOfVariables();
        if (monoms_vars.size() > 1)
            return 0;
        if (!monoms_vars.empty() && !(it->getMaxDegreeOfVariable(*monoms_vars.begin()).isOne()))
            return 0;
        max_value += (it->getCoefficient().compareWith(0) > 0 ? it->getApproximateValue(max_rule) : it->getApproximateValue(min_rule));
        min_value += (it->getCoefficient().compareWith(0) > 0 ? it->getApproximateValue(min_rule) : it->getApproximateValue(max_rule));
        if (std::isnan(max_value) || std::isnan(min_value))
            return 0;
    }
    if (max_value >= 0 && min_value >= 0)
        return 1;
    if (min_value <= 0 && max_value <= 0)
        return -1;
    return 0;
}
double Polynomial::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    double value = 0.0;
    for (auto &it : this->monomials)
        value += it->getApproximateValue(choosing_value_rule);
    return value;
}
void Polynomial::changeSign()
{
    for (auto &it : this->monomials)
        it->setCoefficinet(it->getCoefficient() * -1);
    this->monomials.sort(&Fractal::lessFrac);
}

bool Polynomial::isNeedToChangeSignToLeadTheAcceptedForm()
{
    int min_id = std::numeric_limits<int>::max();
    Number max_degree = -1;
#define monoms this->monomials
    bool is_max_negative = false;
    for (auto &it : monoms)
    {
        auto num = it->getFractal().first;
        for (auto &it1 : *num)
        {
            AbstractExpression * arg = Degree::getArgumentOfDegree(it1.get());
            if (arg->getId() > 0)
            {
                if (arg->getId() < min_id)
                {
                    min_id = arg->getId();
                    max_degree = -1;
                }
                if (arg->getId() == min_id)
                {
                    abs_ex degr = Degree::getDegreeOfExpression(it1.get());
                    if (degr->getId() == NUMBER)
                    {
                        if (static_cast<Number*>(degr.get())->compareWith(max_degree) > 0)
                        {
                            max_degree = *static_cast<Number*>(degr.get());
                            is_max_negative = it->getCoefficient().compareWith(0) < 0;
                        }
                    }
                    else
                    {
                        max_degree = Number::makeErrorNumber();
                        break;
                    }
                }
            }
        }
        if (max_degree.compareWith(-2) == 0)
            break;
    }
    if (max_degree.compareWith(0) > 0 && is_max_negative)
        return true;
    return false;
}

std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> > Polynomial::checkTrigonometricalFunctions()
{
    std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> params;

    for (auto &it : this->monomials)
    {
        it->checkTrigonometricalFunctionsItHas(params);
    }
    return params;
}


 std::map<QString, TrigonometricalFunctionsCastType> Polynomial::getTrigonometricalFunctionToTurnIntoIt()
{
     //имеет ли триг. функцию означает все степени кроме квадрата
     //нулевое - имеет ли синус квадрат, первое - имеет ли синус, второе - имеет ли косинус квадрат, третье - имеет ли косинус, четвертое - имеет ли тангенс квадрат
     //пятое - имеет ли тангенс,
     //шестое - имеет ли котангенс квадрат, седьмое - имеет ли котангенс

    std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> params = this->checkTrigonometricalFunctions();
    return chooseConversionRules(params);

}
void Polynomial::castTrigonometricalFunctions()
{
    auto cast_types = this->getTrigonometricalFunctionToTurnIntoIt();
    bool has_to_convert = false;
    for (auto &it : cast_types)
        if (it.second != NONE)
            has_to_convert = true;
    if (!has_to_convert)
        return;
    for (auto &it : this->monomials)
        it->convertTrigonometricalFunctionsByFormulas(cast_types);
    this->casted_trigonometry = true;
}
