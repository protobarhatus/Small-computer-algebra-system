#ifndef ARCSINUS_H
#define ARCSINUS_H
#include "abstractexpression.h"

class ArcSinus : public AbstractExpression
{
    abs_ex argument;
public:
    ArcSinus(const abs_ex & arg);
    ArcSinus(abs_ex && arg);
    ArcSinus(const ArcSinus & cop);
    ArcSinus(ArcSinus && mov);
    virtual ~ArcSinus();
    virtual AlgebraExpression getId() const override;
    virtual void simplify() override;
    virtual bool operator==(AbstractExpression & right) override;
    //переход к другим триг. функциям происходит в другом интерфейсе
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
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule) override;
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    QString getStringArgument() const;

    abs_ex getArgumentMoved();
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) override;
    abs_ex getArgumentsCopy();
    abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
    const abs_ex& getArgument() const;
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
private:
    virtual bool operator<(const AbstractExpression & right) const override;
};
abs_ex asin(const abs_ex & arg);
abs_ex asin(abs_ex && arg);
abs_ex acos(const abs_ex & arg);
abs_ex acos(abs_ex && arg);
#endif // ARCSINUS_H
