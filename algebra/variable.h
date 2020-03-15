#ifndef VARIABLE_H
#define VARIABLE_H
#include "abstractexpression.h"
#include <QString>

class VariablesDistributor;
class VariablesDefinition;
class Variable : public AbstractExpression
{
public:

    Variable(const Variable & var);
    //only copy existing from id
    Variable(int id);
    //это конструктор чисто на время разработки, по-хорошему им пользоваться нельзя. Он не будет пополняться или изменяться
    Variable(int id, QString name);
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    virtual void simplify() override;
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    virtual bool canDowncastTo(AlgebraExpression expr) override;
    virtual std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) override;
    QString getName() const;
    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() const override;
    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
private:
    friend Variable getVariable(int id);
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    friend void deleteVariables();
    Variable(int id, VariablesDefinition * def);
    Variable();
    virtual bool operator<(const AbstractExpression & right) const override;
    int id;
    static int id_counter;
    static int system_id_counter;
    QString name;
    VariablesDefinition * definition;
    friend VariablesDistributor;
    friend Variable systemVar();
};
#endif // VARIABLE_H
