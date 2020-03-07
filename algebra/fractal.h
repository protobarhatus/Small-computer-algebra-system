#ifndef FRACTAL_H
#define FRACTAL_H
#include <algebra/abstractexpression.h>
#include <memory>
#include <list>
#include <algebra/number.h>
#include "trigonometrical_conversions.h"
typedef std::list<std::unique_ptr<AbstractExpression>> fractal_argument;


class Polynomial;
class Fractal : public AbstractExpression
{
public:


    Fractal();
    Fractal(const Fractal & expr);
    Fractal(Fractal && expr);
    Fractal(AbstractExpression * num, AbstractExpression * denum, Number coe = Number(1));
    Fractal(fractal_argument * num, fractal_argument * denum, Number coe = Number(1));
    Fractal(fractal_argument && num, fractal_argument && denum, Number coe = Number(1));
    Fractal(Number coefficient);
    Fractal(AbstractExpression * num, Number coe = Number(1));
    Fractal(fractal_argument * num, Number coe = 1);
    Fractal(std::unique_ptr<AbstractExpression> && num, Number coe = 1);
    virtual void simplify() override;
    virtual AlgebraExpression getId() const override;

    virtual bool canDowncastTo(AlgebraExpression expr) override;
    virtual std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) override;
    Number getCoefficient();
    void setCoefficinet(Number num);
    virtual bool operator==(AbstractExpression & right) override;
    //comparing without coefficient
    bool isLetterPartEquallTo(Fractal * frac);
    //this function is for degree's casting.
    std::pair<fractal_argument *, fractal_argument *> getFractal() const;

    std::unique_ptr<Fractal> operator+(const std::unique_ptr<Fractal> & second_summary);
    std::unique_ptr<Fractal> operator-(const std::unique_ptr<Fractal> & subtrahend);
    bool isDenominatorEqualToOne();
    bool isZero();
    static std::unique_ptr<Fractal> makeZeroFractal();
    bool isPolynomial();
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    std::unique_ptr<Fractal> getFractalWithoutVariable(int id);
    std::unique_ptr<Fractal> operator*(const std::unique_ptr<Fractal> & right);
    std::unique_ptr<Fractal> operator*(const Fractal & right);
    std::unique_ptr<Fractal> operator/(const std::unique_ptr<Fractal> & right);
    std::unique_ptr<Fractal> operator/(const Fractal & right);
    //can do if has polynom in numerator and empty denominator and coefficient with denominator = 1. transform expr like c(a+b)(d+e) into cad+cbd+cae+cbe
    bool canTurnIntoPolynomWithOpeningParentheses();
    std::unique_ptr<Polynomial> turnIntoPolynomWithOpeningParentheses();

    static bool lessFrac(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right);
    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() override;
    std::unique_ptr<AbstractExpression> findCommonPart(Fractal * frac);
    void sortVariablesInIncreasingOfTheirId();
    void pushBackToNumerator(std::unique_ptr<AbstractExpression> && expr);
    void pushBackToDenominator(std::unique_ptr<AbstractExpression> && expr);

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    //приведет такие выражения как 3(a-b) и -3(b-a) к одному виду для четных/нечетных функций
    void intoAcceptedSignForm();
    bool hasTangent();
    void convertTrigonometricalFunctionsByFormulas(const std::map<QString, TrigonometricalFunctionsCastType> & instructions);
    void checkTrigonometricalFunctionsItHas(std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> & params);
private:
    bool casted_trigonometry;
    void castTrigonometry();
    std::map<QString, TrigonometricalFunctionsCastType> getTrigonometricalFunctionToTurnIntoIt();


    virtual int getPositionRelativelyZeroIfHasVariables() override;
    void reduceSameMembers();
    void reduceMembersWithDowngradingDegree();
    void reduceComparibleDegrees();
    //made for polynomials for simpler dividing. turn expression like (a+b)^2 or a^2 into list of expressions like (a+b)(a+b) or aa
    void turnDegreesIntoList();
    void reducePolynomialsCoefficient();
    void reducePolynomials();
    void setSameMembersIntoDegree();
    void pullNumbersIntoCoefficient();
    void reduceDegrees();
    void takeCommonPartOfPolynomials();
    //i have to do it becouse sometimes they get new divisibility properties when multiplied
    void multiplyIrrationalSums();
    void getRidOfIrrationalityInDenominator();
    void getRidOfRootsInDenominator();
    void addFractal(Fractal * fractal, bool as_denominator = false);
    virtual bool operator<(const AbstractExpression & right) const override;
    fractal_argument numerator;

    fractal_argument denominator;

    Number coefficient;
};
abs_ex toAbsEx(std::unique_ptr<Fractal> & expr);
abs_ex toAbsEx(std::unique_ptr<Fractal> && expr);
fractal_argument operator*(fractal_argument & fmult, fractal_argument & smult);
std::unique_ptr<Fractal> operator*(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right);
std::unique_ptr<Fractal> operator/(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right);
std::unique_ptr<Fractal> operator+(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right);
std::unique_ptr<Fractal> operator-(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right);
bool operator==(fractal_argument & left, fractal_argument & right);
#endif // FRACTAL_H
