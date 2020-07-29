#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <algebra/abstractexpression.h>
#include <memory>
#include <list>
#include <algebra/number.h>
#include <tuple>
#include "trigonometrical_conversions.h"
#include <array>
class Fractal;

class Polynomial : public AbstractExpression
{
public:
    Polynomial();
    Polynomial(std::list<std::unique_ptr<Fractal>> & list);
    Polynomial(std::list<std::unique_ptr<Fractal>> && list);
    Polynomial(const Polynomial & pol);
    Polynomial(Polynomial && pol);
    Polynomial(AbstractExpression * fsum, AbstractExpression * secsum = std::make_unique<Number>(0).get());
    Polynomial(std::unique_ptr<Fractal> && first, std::unique_ptr<Fractal> && second);
    Polynomial(const abs_ex & expr);
    Polynomial(abs_ex && expr);
    Polynomial& operator=(Polynomial && pol);
    void addMonomial(Fractal* fractal);
    void addMonomial(std::unique_ptr<Fractal> && monom);
    virtual void simplify() override;
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    bool isMonomial();
    virtual bool canDowncastTo() override;
    virtual abs_ex downcastTo() override;
    //assert if cannot
    //i don't remember why do i need this function, maybe should delete?
    abs_ex turnIntoMonomial();
    bool canBecameFractal();
    bool isZero();
    void merge(Polynomial * polynomial);
    void merge(std::unique_ptr<Polynomial> && polynom);
    //makes coefficients's GCF = 1. Returned number is number polynomial has divided on
    Number reduce();
    //returns result and remainder. makes copy of both divider and dividend. both of them must be simplified
    std::pair<std::unique_ptr<Polynomial>, std::unique_ptr<Polynomial>> divide(Polynomial * dividend, bool
                                                                               can_return_fractional_coefficients = true);
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    std::list<Fractal*> getMonomialsPointers() const;
    Polynomial getCoefficientOfMaxDegreeOfVariable(int id);
    virtual void _qDebugOut() override;

    Polynomial operator+(AbstractExpression * expr);
    Polynomial operator-(AbstractExpression * expr);
    Polynomial operator*(AbstractExpression * expr);
    Fractal operator/(AbstractExpression * expr);
    virtual QString makeStringOfExpression() const override;
    virtual QString makeWolframString() const override;
    QString toString() const override;
    abs_ex reduceCommonPart();
    // if it is a sum like sqrt(3)+sqrt(5), it returns result on formula (a+b)(a-b)=a^2-b^2. returns result and multiplier
    std::pair<abs_ex, abs_ex> multiplyIrrationalSumOnAppropriateFormula();
    //is sum of rational and irrational numbers without any variables
    bool isIrrationalSum();
    bool canGetRidOfIrrationalytyByMultiplying();
    abs_ex tryToTakeRoot(long long int root);
    abs_ex toDegree(long long int degree);

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    void changeSign();
    //нужно для четных/нечетных функций. проверяет выражения такие как a - b или b - a чтобы привести их к единой форме
    bool isNeedToChangeSignToLeadTheAcceptedForm();

    //имеет ли триг. функцию означает все степени кроме квадрата
    //нулевое - имеет ли синус квадрат, первое - имеет ли синус, второе - имеет ли косинус квадрат, третье - имеет ли косинус, четвертое - имеет ли тангенс квадрат
    //пятое - имеет ли тангенс,
    //шестое - имеет ли котангенс квадрат, седьмое - имеет ли котангенс
    std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> checkTrigonometricalFunctions();
    void checkTrigonometricalFunctionsItHas(std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> > & params);
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) override;
    std::unique_ptr<Fractal> toCommonDenominator();
    bool isFractionalCoefficientsAllowed();
    void setFractionalCoefficientsAllowed(bool allow);
    std::pair<abs_ex, abs_ex> checkIfItIsLinearFunction(int var) const;

    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
    //проверяет, является ли полином квадратичной функцией и возвращает коэффициенты a, b, c в выражении вида ax^2+bx+c
    std::array<abs_ex, 3> checkQuadraticFunction(int var) const;
    abs_ex tryToDistinguishFullDegree() const;
    void tryToDistingushFullDegreeOfVariablePolynomial(abs_ex & polynom, Polynomial * polynom_ptr) const;
    std::pair<std::list<abs_ex>, Number> tryToFactorizeByDistingushesOfFullDegree() const;
    virtual long long int getLcmOfDenominatorsOfDegreesOfVariable(int var) const override;
    virtual long long int getGcdOfNumeratorsOfDegrees(int var) const override;
    virtual void setSimplified(bool simpl) override;
    virtual std::set<abs_ex > getTrigonometricalFunctions() const override;
    bool hasLogarithmicMonoms() const;
    void checkIfShouldOpenTrigonometricalSums();
    bool hasTrigonometryMultipliers() const;
    abs_ex tryToReturnPolynomOfOnlyTrigonometrical() const;
    int amountOfMonoms() const;
    void deleteAllTrigonometricalMonoms();
    virtual FunctionRange getRange() const override;
    std::list<std::unique_ptr<Fractal>>* getMonoms();
    bool hasDifferential() const override;
    //отличие также в том, что hasIntegratingConstantAddictive() вызывается внутри simplify()
    bool hasIntegratingConstantThatCanBeChanged() const;
    abs_ex takeAwayIntegragingConstantThatCanBeChanged();
    bool tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second) override;
    abs_ex tryToFindExponentialFunction(int var) const override;
    void convertTrigonometricalFunctionsByFormulas(const std::map<QString, TrigonometricalFunctionsCastType> &instructions);

    void getRidOfAbsoluteValues() override;

    void eraseAllAddictiveWithoutVar(int var);

    void doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const override;

    bool canBeZero() const override;
private:
    bool hasIntegratingConstantAddictiveThatCanBeChanged() const;
    void pullSomeMembersIntoOneIntegratingConstant();
    void checkIfCanSimplifyThisTrigonometryByTakingCommonPart();
    bool casted_trigonometry;
    void castTrigonometry();
    void castTrigonometryArguments();
    std::map<QString, TrigonometricalFunctionsCastType> getTrigonometricalFunctionToTurnIntoIt();
    void castTrigonometricalFunctions();
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    void pushBack(std::unique_ptr<Fractal> && fractal);
    abs_ex tryToTakeRootOfVariablesPolynomial(long long int root);
    //only square root
    abs_ex tryToTakeRootOfNonVariablesPolynomial();
    std::list<std::unique_ptr<Fractal>> monomials;
    //this functions is for situations when merging can make iterator invalid
    void mergeWithoutSimplifiyng(Polynomial * polynomial, Number coe = 1);
    void mergeWithoutSimplifiyng(std::unique_ptr<Polynomial> && polynom, Number coe = 1);
    void leadLikeMembers(std::list<std::unique_ptr<Fractal>>::iterator to_whom);
    void leadLikeMembers();
    void openParentheses();
    bool foldMonomials(std::list<std::unique_ptr<Fractal>>::iterator first, std::list<std::unique_ptr<Fractal>>::iterator second);
    void removeZeroes();
    int tryToGetPositionRelativelyZeroOfLinearFunction();
    virtual bool operator<(const AbstractExpression & right) const override;
    //for dividing
    bool is_fractional_coefficients_allowed = false;
};
abs_ex checkIfItsSinusOrCosinusFormulaPolynomial(std::unique_ptr<Polynomial> && polynom);
abs_ex checkIfItsTangentOrCotangentFormulaPolynomial(std::unique_ptr<Polynomial> && polynom);
std::unique_ptr<Polynomial> toPolynomialPointer(const abs_ex & expr);
std::unique_ptr<Polynomial> toPolynomialPointer(abs_ex && expr);

std::unique_ptr<Polynomial> toPolynomWithFractialCoefficients(const abs_ex & expr);
#endif // POLYNOMIAL_H
