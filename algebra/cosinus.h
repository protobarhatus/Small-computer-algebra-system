#ifndef COCosinus_H
#define COCosinus_H

#include "abstractexpression.h"
class Fractal;
class Cosinus : public AbstractExpression
{
public:
    Cosinus(const abs_ex & argument);
    Cosinus(abs_ex && argument);
    Cosinus(const Cosinus & copy);
    Cosinus(Cosinus && mov);
    virtual ~Cosinus();
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
private:
    abs_ex argument;
    //Pi_member - это pi, домноженное на некоторое рациональное число. ничего больше в этой дроби быть не может
    std::unique_ptr<Fractal> pi_member;
    bool is_pi_member_only = false;
    virtual bool operator<(const AbstractExpression & right) const override;
};
abs_ex cos(const abs_ex & expr);
abs_ex cos(abs_ex && expr);

#endif // COsinus_H
