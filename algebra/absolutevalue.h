#ifndef ABSOLUTEVALUE_H
#define ABSOLUTEVALUE_H
#include "abstractexpression.h"

class AbsoluteValue : public AbstractExpression
{
public:
    AbsoluteValue(std::unique_ptr<AbstractExpression> & expr);
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
    virtual Number getMaxDegreeOfVariable(int id) override;

    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() override;

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    //returns moved expression so it's not valid and must be destroyed
    std::unique_ptr<AbstractExpression> open();

private:
    void checkIfNeedToCheckSignToLeadToTheAcceptedForm();
    virtual int getPositionRelativelyZeroIfHasVariables() override;

    virtual bool operator<(const AbstractExpression& expr) const override;
    std::unique_ptr<AbstractExpression> expression;
    int expr_position;
};

#endif // ABSOLUTEVALUE_H
