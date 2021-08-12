#ifndef ABSOLUTEVALUE_H
#define ABSOLUTEVALUE_H
#include "abstractexpression.h"

class AbsoluteValue : public AbstractExpression
{
public:
    AbsoluteValue(const abs_ex & expr);
    AbsoluteValue(abs_ex && expr);
    AbsoluteValue(const AbsoluteValue & value);
    AbsoluteValue(AbsoluteValue && value);

    virtual void simplify() override;
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression &expr) override;
    virtual bool canDowncastTo() override;
    virtual abs_ex downcastTo() override;

    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;

    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() const override;
    virtual QString makeWolframString() const override;
    QString toString() const override;

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    //returns moved expression so it's not valid and must be destroyed
    abs_ex open();
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) override;
    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
    AbstractExpression* getExpression();
    abs_ex getExpressionCopy() const;
    abs_ex getExpressionMoved();
    virtual void setSimplified(bool simpl) override;
    virtual std::set<abs_ex > getTrigonometricalFunctions() const override;
    virtual long long int getLcmOfDenominatorsOfDegreesOfVariable(int var) const override;
    virtual long long int getGcdOfNumeratorsOfDegrees(int var) const override;
    virtual FunctionRange getRange() const override;
    virtual bool hasDifferential() const override;
    bool tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second) override;
    abs_ex tryToFindExponentialFunction(int var) const override;
    void getRidOfAbsoluteValues() override;

    void doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const override;

    bool canBeZero() const override;

    bool hasUndefinedVariable() const override;
private:
    void checkIfNeedToCheckSignToLeadToTheAcceptedForm();
    virtual int getPositionRelativelyZeroIfHasVariables() override;

    virtual bool operator<(const AbstractExpression& expr) const override;
    abs_ex expression;
    int expr_position;
};
abs_ex abs(const abs_ex & expr);
abs_ex abs(abs_ex && expr);
#endif // ABSOLUTEVALUE_H
