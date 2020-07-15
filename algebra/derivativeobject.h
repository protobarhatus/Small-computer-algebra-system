#ifndef DERIVATIVEOBJECT_H
#define DERIVATIVEOBJECT_H
#include "abstractexpression.h"

class DerivativeObject : public AbstractExpression
{
public:
    DerivativeObject(int func_var, int arg_var, int order);
    DerivativeObject(const abs_ex & arg, int func_arg_var, int order);
    DerivativeObject(abs_ex && arg, int func_arg_var, int order);
    DerivativeObject(const DerivativeObject & copy);
    DerivativeObject(DerivativeObject && cop);
    void simplify() override;
    bool canDowncastTo() override;
    abs_ex downcastTo() override;
    AlgebraExpression getId() const override;
    bool operator==(AbstractExpression & right) override;

    std::set<int> getSetOfVariables() const override;
    std::set<int> getSetOfPolyVariables() const override;
    std::set<QString> getSetOfFunctions() const override;
    Number getMaxDegreeOfVariable(int id) override;
    void _qDebugOut() override;
    QString makeStringOfExpression() const override;
    QString toString() const override;
    QString makeWolframString() const override;

    double getApproximateValue() override;
    double getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule) override;
    abs_ex changeSomePartOn(QString function, abs_ex &on_what) override;
    abs_ex changeSomePartOnExpression(QString part, abs_ex &on_what) override;
    abs_ex derivative(int var) const override;
    abs_ex antiderivative(int var) const override;

    void setSimplified(bool simpl) override;
    std::set<abs_ex> getTrigonometricalFunctions() const override;
    long long getLcmOfDenominatorsOfDegreesOfVariable(int var) const override;
    long long getGcdOfNumeratorsOfDegrees(int var) const override;
    FunctionRange getRange() const override;
    bool hasDifferential() const override;
    bool tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second) override;
    abs_ex tryToFindExponentialFunction(int var) const override;
    void getRidOfAbsoluteValues() override;

    void doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const override;
private:
    int func_argument_var;
    abs_ex argument;
    int order;
    int getPositionRelativelyZeroIfHasVariables() override;
    bool operator<( const AbstractExpression & expr) const override;
};
abs_ex derivative(const abs_ex & argument, int var, int order);
abs_ex derivative(abs_ex && argument, int var, int order);
#endif // DERIVATIVEOBJECT_H
