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
    Variable(int id, QString name, VariablesDefinition * def);
    ~Variable();
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    virtual void simplify() override;
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    virtual bool canDowncastTo() override;
    virtual abs_ex downcastTo() override;
    QString getName() const;
    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() const override;
    virtual QString makeWolframString() const override;
    QString toString() const override;
    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) override;
    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
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

    void setName(const QString & name);
    void setRange(const FunctionRange & range);

    bool canBeZero() const override;

    bool hasUndefinedVariable() const override;

private:
    friend Variable getVariable(int id);
    friend Variable integratingConstant();
    friend Variable integratingConstant(const FunctionRange & range);
    friend Variable systemVar(int min, int max);
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    friend void deleteVariables();
    Variable(int id, VariablesDefinition * def);
    Variable();
    virtual bool operator<(const AbstractExpression & right) const override;
    int id;
    static int id_counter;
    static int system_id_counter;
    static int integrating_constant_id_counter;
    QString name;
    VariablesDefinition * definition;
    friend VariablesDistributor;
    friend Variable systemVar();
    friend Variable systemVar(const abs_ex & min, const abs_ex & max, bool min_included, bool max_included);
    friend Variable systemVar(const FunctionRange & range);
};

abs_ex varToAbsEx(const Variable & var);
#endif // VARIABLE_H
