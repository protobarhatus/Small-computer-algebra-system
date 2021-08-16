#ifndef DIFFERENTIAL_H
#define DIFFERENTIAL_H
#include "abstractexpression.h"
//представляет собой самостоятельный объект, которым оперируют в дифференциальных уравнениях. Необходим как множитель при интегрировании всего, кроме нуля
class Differential : public AbstractExpression
{
    abs_ex argument;
public:
    Differential(const abs_ex & arg);
    Differential(abs_ex & arg);
    Differential(const Differential & cop);
    Differential(Differential && mov);
    virtual ~Differential();
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
    virtual abs_ex changeSomePartOn(QString part, const abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, const abs_ex & on_what) override;
    abs_ex getArgumentsCopy() const;
    virtual abs_ex derivative(int var) const override;
    AbstractExpression* getArgument();
    virtual abs_ex antiderivative(int var) const override;
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
    virtual bool operator<(const AbstractExpression & expr) const override;
};
abs_ex D(const abs_ex & arg);
abs_ex D(abs_ex && arg);
abs_ex fullDifferential(const abs_ex & expr);
#endif // DIFFERENTIAL_H
