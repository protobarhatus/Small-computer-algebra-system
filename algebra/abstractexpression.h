#ifndef ABSTRACTEXPRESSION_H
#define ABSTRACTEXPRESSION_H
#include <memory>
#include <set>
#include <QDebug>
#include <QString>
#include <vector>
#define SIM_IF_NEED if (this->simplified) return; else this->simplified = true;
#define NONCONST this->simplified = false;
#define abs_ex std::unique_ptr<AbstractExpression>
//order there influences on order in polynomials, but it's important only for output
//Id > 0 is for variables
//На данном этапе развития модуль не сможет работать с тригонометрическими функциями как с переменными. теоретически,
//это можно улучшить, но потребует изменения кода. этот функционал просто не нужен
//Почему я оставляю некоторые комментарии на русском, а некоторые - на английском?
enum AlgebraExpression
{
    POLYNOMIAL = -1,
    FRACTAL = -3,
    DEGREE = -4,
    ABSOLUTE_VALUE = -5,
    SINUS = -6,
    COSINUS = -7,
    TANGENT = -8,
    COTANGENT = -9,
    CONSTANT = -10,
    NUMBER = 0
};

class Number;
class VariablesDefinition;

//less or equally operators are not algebraistic but system. for algebra comparing use appropriate functions
//operators +, -, *, / are algebraistic
//almost all functions assumes that objects that call them are simplified.
class AbstractExpression
{
public:
    AbstractExpression();
    AbstractExpression & operator=(AbstractExpression & expr);
    AbstractExpression(AbstractExpression & expr);
    virtual ~AbstractExpression();
    virtual void simplify() = 0;
    virtual AlgebraExpression getId() const = 0;
    //also system comparing, not math
    static bool less(const AbstractExpression * left, const AbstractExpression * right);
    //function for std::sort
    static bool lessToSort(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right);

    virtual bool operator==(AbstractExpression & right) = 0;
    bool operator!=(AbstractExpression & right);
    //returns simplified and downcasted fractal
    std::unique_ptr<AbstractExpression> operator*(AbstractExpression & expr);
    std::unique_ptr<AbstractExpression> operator/(AbstractExpression & expr);
    //returns simplified and downcasted polynomial
    std::unique_ptr<AbstractExpression> operator+(AbstractExpression & expr);
    std::unique_ptr<AbstractExpression> operator-(AbstractExpression & expr);
    virtual bool canDowncastTo(AlgebraExpression expr) = 0;
    //need an addition when new types of expression added
    bool canDowncast();

    //ВНИМАНИЕ!: сейчас downcastTo занимается копированием, однако, мне, наверно стоит изменить это на перемещение
    virtual std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) = 0;

    //does downcasting till can for unlimitted amount of levels
    std::unique_ptr<AbstractExpression> downcast();
    //in expression like x*tan(y)^z returns only x
    virtual std::set<int> getSetOfPolyVariables() const = 0;
    //in expression like x^z returns both of them
    virtual std::set<int> getSetOfVariables() const = 0;
    //returns string discription of all trigonometrical, logarithmic and other functions
    virtual std::set<QString> getSetOfFunctions() const = 0;
    //if variable has'nt been found or it's max degree isn't Number or it's inside other function, it returns Number::makeErrorNumber()
    virtual Number getMaxDegreeOfVariable(int id) = 0;

    virtual void _qDebugOut() = 0;
    virtual QString makeStringOfExpression() const = 0;
    virtual double getApproximateValue() = 0;
    //the difference between overload without argument is that this function choose a value for variable from definition by lambda, but other cannot be used with variables andjust throw assert()
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) = 0;
    //1 is bigger than 0 (or equally), -1 is less, 0 is undefined
    int getPositionRelativelyZero();

    virtual abs_ex changeSomePartOn(QString function, abs_ex & on_what) = 0;
private:
    //subclasses assume that right is the same subclass, so they downcasting it momentally. if it not the same, assert is calling
    virtual bool operator<(const AbstractExpression & right) const = 0;
protected:
    bool simplified = false;
    virtual int getPositionRelativelyZeroIfHasVariables() = 0;

};
QString getStringArgumentOfTrigonometricalFunction(abs_ex & expr);
QString getStringArgumentOfTrigonometricalFunction(AbstractExpression * expr);
std::vector<abs_ex> replaceEveryFunctionOnSystemVariable(abs_ex & expr);
void replaceSystemVariablesBackToFunctions(abs_ex & expr, std::vector<abs_ex> & functions);
abs_ex getArgumentOfTrigonometricalFunction(abs_ex && expr);
bool isDegreeOfTrigonometricalFunction(abs_ex & expr);
std::unique_ptr<AbstractExpression> operator*(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right);
std::unique_ptr<AbstractExpression> operator/(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right);

std::unique_ptr<AbstractExpression> operator+(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right);
std::unique_ptr<AbstractExpression> operator-(const std::unique_ptr<AbstractExpression> & left, const std::unique_ptr<AbstractExpression> & right);
#endif // ABSTRACTEXPRESSION_H
