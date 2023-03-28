#ifndef TANGENT_H
#define TANGENT_H
#include "abstractexpression.h"
class Fractal;
class Tangent : public AbstractExpression
{
public:
    Tangent(const abs_ex & argument);
    Tangent(abs_ex && argument);
    Tangent(const Tangent & copy);
    Tangent(Tangent && mov);
    virtual ~Tangent();
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
    QString toString() const override;
    QString makeRenderString() const override;
    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule) override;
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    QString getStringArgument() const;
    virtual QString makeWolframString() const override;

    abs_ex getArgumentMoved();
    virtual abs_ex changeSomePartOn(QString part, const abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, const abs_ex & on_what) override;
    abs_ex getArgumentsCopy();

    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;

    const abs_ex& getArgument() const;
    virtual void setSimplified(bool simpl) override;
    virtual std::set<abs_ex > getTrigonometricalFunctions() const override;
    virtual long long int getLcmOfDenominatorsOfDegreesOfVariable(int var) const override;
    virtual long long int getGcdOfNumeratorsOfDegrees(int var) const override;
    virtual FunctionRange getRange() const override;
    bool hasDifferential() const override;
    bool tryToMergeIdenticalBehindConstantExpressions(const abs_ex &second) override;
    abs_ex tryToFindExponentialFunction(int var) const override;
    void getRidOfAbsoluteValues() override;

    void doSomethingInDerivativeObject(const std::function<void (int, int, int)> &func) const override;

    bool canBeZero() const override;

    bool hasUndefinedVariable() const override;
private:
    abs_ex argument;
    //Pi_member - это pi, домноженное на некоторое рациональное число. ничего больше в этой дроби быть не может
    std::unique_ptr<Fractal> pi_member;
    bool is_pi_member_only = false;
    virtual bool operator<(const AbstractExpression & right) const override;
};
abs_ex tan(const abs_ex & expr);
abs_ex tan(abs_ex && expr);
#endif // TANGENT_H
