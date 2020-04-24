#ifndef ABSOLUTEVALUE_H
#define ABSOLUTEVALUE_H
#include "abstractexpression.h"

class AbsoluteValue : public AbstractExpression
{
public:
    AbsoluteValue(const std::unique_ptr<AbstractExpression> & expr);
    AbsoluteValue(std::unique_ptr<AbstractExpression> && expr);
    AbsoluteValue(AbsoluteValue & value);
    AbsoluteValue(AbsoluteValue && value);

    virtual void simplify() override;
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression &expr) override;
    virtual bool canDowncastTo(AlgebraExpression expr) override;
    virtual std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) override;

    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;

    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() const override;

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    //returns moved expression so it's not valid and must be destroyed
    std::unique_ptr<AbstractExpression> open();
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
private:
    void checkIfNeedToCheckSignToLeadToTheAcceptedForm();
    virtual int getPositionRelativelyZeroIfHasVariables() override;

    virtual bool operator<(const AbstractExpression& expr) const override;
    std::unique_ptr<AbstractExpression> expression;
    int expr_position;
};
abs_ex abs(const abs_ex & expr);
abs_ex abs(abs_ex && expr);
#endif // ABSOLUTEVALUE_H
