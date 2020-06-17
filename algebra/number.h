#ifndef NUMBER_H
#define NUMBER_H
#include "abstractexpression.h"
//actually, it's fractal constists of int
class Number : public AbstractExpression
{
public:
    Number(long long int value = 0);
    Number(const Number & num);
    Number(long long int numerator, long long int denumerator);
    Number & operator=(const Number & num);
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    bool operator !=(const Number & num) const;
    long long int getNumerator() const;
    long long int getDenominator() const;
    virtual void simplify() override;
    virtual bool canDowncastTo() override;
    virtual abs_ex downcastTo() override;
    Number & operator=(long long int value);
    //the most important values
    bool isOne() const;
    bool isZero() const;
    //that's algebrasting function and comparing is made with all rules
    short int compareWith(long long int value) const;
    short int compareWith(Number num) const;
    bool isInteger() const;
    Number operator+(const Number & number) const;
    Number operator-(const Number & number) const;
    Number operator*(const Number & number) const;
    Number operator/(const Number & number) const;
    Number operator+=(const Number & number);
    Number operator-=(const Number & number);
    Number operator*=(const Number & number);
    Number operator/=(const Number & number);
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    static Number makeErrorNumber();
    bool isCorrect() const;
    virtual Number getMaxDegreeOfVariable(int id) override;


    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() const override;
    virtual QString makeWolframString() const override;

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    bool operator==(long long int num);
    bool operator==(const Number & num);
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) override;
    bool less(const Number & b) const;
    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
    virtual void setSimplified(bool simpl) override;
    virtual std::set<abs_ex > getTrigonometricalFunctions() const override;
    virtual long long int getLcmOfDenominatorsOfDegreesOfVariable(int var) const override;
    virtual long long int getGcdOfNumeratorsOfDegrees(int var) const override;
private:
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    long long int numerator;
    long long int denominator;
    virtual bool operator<(const AbstractExpression & right) const override;

};

Number abs(Number num);
Number max(Number a, Number b);
Number min(Number a, Number b);
Number pow(Number a, int deg);
abs_ex toAbsEx(Number n);
bool isIntegerNumber(const abs_ex & expr);
/*

abs_ex one = abs_ex(new Number(1));
abs_ex two = abs_ex(new Number(2));
abs_ex half = abs_ex(new Number(1, 2));
abs_ex zero = abs_ex(new Number(0));
abs_ex minus_one = abs_ex(new Number(-1));*/
extern abs_ex one;
extern abs_ex two;
extern abs_ex half;
extern abs_ex zero;
extern abs_ex minus_one;
extern abs_ex three;
extern abs_ex four;
#endif // NUMBER_H
