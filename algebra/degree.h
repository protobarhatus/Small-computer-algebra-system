#ifndef DEGREE_H
#define DEGREE_H
#include<algebra/abstractexpression.h>
#include<list>
class Fractal;
typedef std::list<std::unique_ptr<AbstractExpression>> fractal_argument;
class Degree : public AbstractExpression
{
public:
    Degree(std::unique_ptr<AbstractExpression> && argument, std::unique_ptr<AbstractExpression>  && degree);
    Degree(const std::unique_ptr<AbstractExpression> & argument, const std::unique_ptr<AbstractExpression> & degree);
    Degree(const Degree & degree);
    Degree(std::unique_ptr<AbstractExpression> & argument, Number degree);
    Degree(std::unique_ptr<AbstractExpression> && argument, Number degree);
    //is this function used anywhere?
    bool isArgument();
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    virtual void simplify() override;

    virtual bool canDowncastTo(AlgebraExpression expr) override;
    std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) override;
    static AbstractExpression * getArgumentOfDegree(AbstractExpression * expr);
    static std::unique_ptr<AbstractExpression> getDegreeOfExpression(AbstractExpression * expr);
    bool isZeroDegree();
    //it is possible when degree is integer number
    bool canGetListOfArguments();
    //second parameter in pairs shows if degree is negative
    std::pair<std::unique_ptr<fractal_argument>, bool> getListOfArguments();
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    //abbreviation formulas
    bool canTurnIntoPolynomial();


    Number reduce();
    virtual void  _qDebugOut() override;
    bool isDegreeOfNumberThatLessThanOne();
    virtual QString makeStringOfExpression() override;
    bool isRootNumeric();
    long long int getRootValue();

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
private:
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    void reducePolynomialArgument();
    void transformPolynomialDegree(bool has_vars);
    void takeArgumentsMultipliersIntoDegree();
    virtual bool operator<(const AbstractExpression & right) const override;
    std::unique_ptr<AbstractExpression> degree;
    std::unique_ptr<AbstractExpression> argument;


};

bool canReduceWithDowngradingDegree(AbstractExpression * left, AbstractExpression * right);
std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> & argument, std::unique_ptr<AbstractExpression> & degree);
std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> && argument, std::unique_ptr<AbstractExpression> && degree);
std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> & argument, Number degree);
std::unique_ptr<AbstractExpression> takeDegreeOf(std::unique_ptr<AbstractExpression> && argument, Number degree);
std::unique_ptr<AbstractExpression> takeDegreeOf(Number argument, Number degree);
#endif // DEGREE_H
