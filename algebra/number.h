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
    long long int getNumerator();
    long long int getDenominator();
    virtual void simplify() override;
    virtual bool canDowncastTo(AlgebraExpression expr) override;
    virtual std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) override;
    Number & operator=(long long int value);
    //the most important values
    bool isOne();
    bool isZero();
    //that's algebrasting function and comparing is made with all rules
    short int compareWith(long long int value) const;
    short int compareWith(Number num) const;
    bool isInteger();
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
    static Number makeErrorNumber();
    bool isCorrect() const;
    virtual Number getMaxDegreeOfVariable(int id) override;


    virtual void _qDebugOut() override;
    virtual QString makeStringOfExpression() override;

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    bool operator==(long long int num);
    bool operator==(const Number & num);
private:
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    long long int numerator;
    long long int denominator;
    virtual bool operator<(const AbstractExpression & right) const override;

};
Number abs(Number num);
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
#endif // NUMBER_H
