#ifndef TANGENT_H
#define TANGENT_H
#include "abstractexpression.h"
class Fractal;
class Tangent : public AbstractExpression
{
public:
    Tangent(abs_ex & argument);
    Tangent(abs_ex && argument);
    Tangent(const Tangent & copy);
    Tangent(Tangent && mov);
    virtual ~Tangent();
    virtual AlgebraExpression getId() const override;
    virtual void simplify() override;
    virtual bool operator==(AbstractExpression & right) override;
    //переход к другим триг. функциям происходит в другом интерфейсе
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
    QString getStringArgument() const;

    abs_ex getArgumentMoved();
private:
    abs_ex argument;
    //Pi_member - это pi, домноженное на некоторое рациональное число. ничего больше в этой дроби быть не может
    std::unique_ptr<Fractal> pi_member;
    bool is_pi_member_only = false;
    virtual bool operator<(const AbstractExpression & right) const override;
};

#endif // TANGENT_H
