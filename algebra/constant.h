#ifndef CONSTANT_H
#define CONSTANT_H
#include "abstractexpression.h"

class Constant : public AbstractExpression
{
public:
    Constant(double value, QString name);
    Constant(const Constant & copy);
    virtual ~Constant();
    virtual AlgebraExpression getId() const override;
    virtual void simplify() override;
    virtual bool operator==(AbstractExpression & right) override;

    virtual bool canDowncastTo(AlgebraExpression expr) override;
    virtual abs_ex downcastTo(AlgebraExpression expr) override;
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() override;
    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule) override;
    virtual int getPositionRelativelyZeroIfHasVariables() override;
private:
    virtual bool operator<(const AbstractExpression & right) const override;
    double value;
    QString name;
};
abs_ex getPi();
abs_ex getEuler();
#endif // CONSTANT_H
