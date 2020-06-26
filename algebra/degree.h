#ifndef DEGREE_H
#define DEGREE_H
#include<algebra/abstractexpression.h>
#include<list>
class Fractal;
typedef std::list<abs_ex> fractal_argument;
class Degree : public AbstractExpression
{
public:
    Degree(abs_ex && argument, abs_ex  && degree);
    Degree(const abs_ex & argument, const abs_ex & degree);
    Degree(const Degree & degree);
    Degree(const abs_ex & argument, Number degree);
    Degree(abs_ex && argument, Number degree);
    //is this function used anywhere?
    bool isArgument();
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    virtual void simplify() override;

    virtual bool canDowncastTo() override;
    abs_ex downcastTo() override;
    static AbstractExpression * getArgumentOfDegree(AbstractExpression * expr);
    static abs_ex getDegreeOfExpression(AbstractExpression * expr);
    bool isZeroDegree();
    //it is possible when degree is integer number
    bool canGetListOfArguments();
    //second parameter in pairs shows if degree is negative
    std::pair<std::unique_ptr<fractal_argument>, bool> getListOfArguments();
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    //abbreviation formulas
    bool canTurnIntoPolynomial();


    Number reduce();
    virtual void  _qDebugOut() override;
    bool isDegreeOfNumberThatLessThanOne();
    virtual QString makeStringOfExpression() const override;
    virtual QString makeWolframString() const override;
    QString toString() const override;
    bool isRootNumeric();
    long long int getRootValue();

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) override;
    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
    virtual void setSimplified(bool simpl) override;
    virtual std::set<abs_ex > getTrigonometricalFunctions() const override;
    virtual long long int getLcmOfDenominatorsOfDegreesOfVariable(int var) const override;
    virtual long long int getGcdOfNumeratorsOfDegrees(int var) const override;
    virtual FunctionRange getRange() const override;
    virtual bool hasDifferential() const override;
    bool tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second) override;
private:
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    void reducePolynomialArgument();
    void transformPolynomialDegree(bool has_vars);
    void takeArgumentsMultipliersIntoDegree();
    virtual bool operator<(const AbstractExpression & right) const override;
    abs_ex degree;
    abs_ex argument;


};

bool canReduceWithDowngradingDegree(AbstractExpression * left, AbstractExpression * right);
abs_ex takeDegreeOf(const abs_ex & argument, const abs_ex & degree);
abs_ex takeDegreeOf(abs_ex && argument, abs_ex && degree);
abs_ex takeDegreeOf(abs_ex && argument, const abs_ex & degree);
abs_ex takeDegreeOf(const abs_ex & argument, Number degree);
abs_ex takeDegreeOf(abs_ex && argument, Number degree);
abs_ex takeDegreeOf(Number argument, Number degree);

abs_ex pow(const abs_ex & arg, const abs_ex & deg);
abs_ex pow(abs_ex && arg, const abs_ex & deg);
abs_ex pow(abs_ex && arg, abs_ex && deg);
abs_ex pow(const abs_ex & arg, abs_ex && deg);
abs_ex pow(const abs_ex & arg, Number deg);
abs_ex pow(abs_ex && arg, Number deg);
abs_ex pow(Number arg, Number deg);
abs_ex sqrt(const abs_ex & arg);
abs_ex sqrt(abs_ex && arg);
abs_ex sqr(const abs_ex & arg);
abs_ex sqr(abs_ex && arg);
bool isSqrt(const abs_ex & expr);
#endif // DEGREE_H
