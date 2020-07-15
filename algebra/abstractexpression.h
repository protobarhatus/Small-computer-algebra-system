#ifndef ABSTRACTEXPRESSION_H
#define ABSTRACTEXPRESSION_H
#include <memory>
#include <set>
#include <QDebug>
#include <QString>
#include <vector>
#include <array>
#include "functionrange.h"
#define SIM_IF_NEED if (this->simplified) return; //else this->simplified = true;
#define NONCONST this->simplified = false;
class AbstractExpression;
typedef std::unique_ptr<AbstractExpression> abs_ex ;
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
    LOGARITHM = -11,
    DIFFERENTIAL = -19,
    ARCTANGENT = -12,
    ARCSINUS = -13,
    DERIVATIVE_OBJECT = -14,
    NUMBER = 0
};

class Number;
class VariablesDefinition;

//less or equally operators are not algebraistic but system. for algebra comparing use appropriate functions
//operators +, -, *, / are algebraistic
//almost all functions assumes that objects that call them are simplified.
class AbstractExpression
{
   // int obj_id;
public:
    AbstractExpression();
    AbstractExpression & operator=(const AbstractExpression & expr);
    AbstractExpression(const AbstractExpression & expr);
    virtual ~AbstractExpression();
    virtual void simplify() = 0;
    virtual AlgebraExpression getId() const = 0;
    //also system comparing, not math
    static bool less(const AbstractExpression * left, const AbstractExpression * right);
    //function for std::sort
    static bool lessToSort(const abs_ex & left, const abs_ex & right);

    virtual bool operator==(AbstractExpression & right) = 0;
    bool operator!=(AbstractExpression & right);
    //returns simplified and downcasted fractal
    /*abs_ex operator*(AbstractExpression & expr);
    abs_ex operator/(AbstractExpression & expr);
    //returns simplified and downcasted polynomial
    abs_ex operator+(AbstractExpression & expr);
    abs_ex operator-(AbstractExpression & expr);*/
    virtual bool canDowncastTo() = 0;
    //need an addition when new types of expression added
    bool canDowncast();

    //ВНИМАНИЕ!: сейчас downcastTo занимается копированием, однако, мне, наверно стоит изменить это на перемещение
    virtual abs_ex downcastTo() = 0;

    //does downcasting till can for unlimitted amount of levels
    abs_ex downcast();
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
    virtual QString makeWolframString() const = 0;
    //отличия от makeStringOfExpression() в том, что makeStringOfExpression расставляет кучу лишних скобочек,
    //а если попытаться это изменить, то полетит часть с заменой переменных на выражения и я хз почему
    virtual QString toString() const = 0;
    virtual double getApproximateValue() = 0;
    //the difference between overload without argument is that this function choose a value for variable from definition by lambda, but other cannot be used with variables andjust throw assert()
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) = 0;
    //1 is bigger than 0 (or equally), -1 is less, 0 is undefined
    int getPositionRelativelyZero();
    bool hasVariable(int var);
    virtual abs_ex changeSomePartOn(QString function, abs_ex & on_what) = 0;
    //отличие от changeSomePartOn в том, что то не снимает simplified, а это - снимает
    virtual abs_ex changeSomePartOnExpression(QString part, abs_ex & on_what) = 0;
    virtual abs_ex derivative(int var) const = 0;
    //поиск первообразной и интеграла тут отличается. Интеграл требует наличие множителя-дифференциала. первообразная проверяет только по таблице,
    //интеграл разделяет сумму на сумму интегралов, запускает поиск первообразной и, если это не табличный случай, переходит к другим методам.
    //если первообразную найти не удается, возвращает nullptr
    //поиск первообразной не добавляет константу интегрирования, это делает интеграл
    virtual abs_ex antiderivative(int var) const = 0;
    virtual void setSimplified(bool simpl) = 0;
    //в выражении по типу cos(1+cos(x)) вернет ТОЛЬКО cos(1+cos(x))
    virtual std::set<abs_ex> getTrigonometricalFunctions() const = 0;
    virtual long long int getLcmOfDenominatorsOfDegreesOfVariable(int var) const = 0 ;
    virtual long long int getGcdOfNumeratorsOfDegrees(int var) const = 0;
    //не считает область значений сложной функции
    virtual FunctionRange getRange() const = 0;
    virtual bool hasDifferential() const = 0;
    virtual bool tryToMergeIdenticalBehindConstantExpressions(const abs_ex & second) = 0;
    virtual abs_ex tryToFindExponentialFunction(int var) const = 0;
    bool isOnlyVarsIntegratingConstantsThatCanBeChanged() const;

    std::set<abs_ex> getUniqueTrigonometricalFunctions() const;
    virtual void getRidOfAbsoluteValues() = 0;

    virtual void doSomethingInDerivativeObject(const std::function<void (int, int, int)> & func) const = 0;
private:
    //subclasses assume that right is the same subclass, so they downcasting it momentally. if it not the same, assert is calling
    virtual bool operator<(const AbstractExpression & right) const = 0;
protected:
    bool simplified = false;
    virtual int getPositionRelativelyZeroIfHasVariables() = 0;


};
bool canBeConsideredAsConstant(const AbstractExpression * expr);
bool canBeConsideredAsConstant(const abs_ex & expr);
bool canBeConsideredAsConstantThatCanBeChanged(const AbstractExpression * expr);
bool canBeConsideredAsConstantThatCanBeChanged(const abs_ex & expr);
bool lower(const abs_ex & left, const abs_ex & right);
bool bigger(const abs_ex & left, const abs_ex & right);
bool lowerOrEquall(const abs_ex & left, const abs_ex & right);
bool biggerOrEquall(const abs_ex & left, const abs_ex & right);
bool subCompare(const abs_ex & a, const abs_ex & b);
bool isDegreeOfSomeFunction(const abs_ex & expr);

abs_ex getExpressionWithoutAddictiveWithoutVariable(const abs_ex & expr, int var);

abs_ex getExpressionWithoutAbsoluteValues(const abs_ex & expr);
QString getStringArgumentOfTrigonometricalFunction(abs_ex & expr);
QString getStringArgumentOfTrigonometricalFunction(AbstractExpression * expr);
//std::vector<abs_ex> replaceEveryFunctionOnSystemVariable(abs_ex & expr);
std::map<int, abs_ex> replaceEveryFunctionOnSystemVariable(abs_ex & expr, std::map<QString, int> & funcs);
void replaceSystemVariablesBackToFunctions(abs_ex &expr, std::map<int, abs_ex> & funcs);
std::map<int, abs_ex> replaceEveryFunctionOnSystemVariable(AbstractExpression * expr, std::map<QString, int> & funcs);
void replaceSystemVariablesBackToFunctions(AbstractExpression *expr, std::map<int, abs_ex> & funcs);
//bool canOpenByTrigonometricalSum(abs_ex & expr);
abs_ex tryToOpenByTrigonometricalSum(abs_ex &expr);

void replaceIntegratingConstantsOnSystemVariables(abs_ex & expr, std::map<int, abs_ex> & system_vars_to_consts_indexes);
void replaceSystemVariablesBackOnIntegratingConstants(abs_ex & expr, std::map<int, abs_ex> & system_vars_to_consts_indexes);
void replaceIntegratingConstantsOnSystemVariables(AbstractExpression * expr, std::map<int, abs_ex> & system_vars_to_consts_indexes);
void replaceSystemVariablesBackOnIntegratingConstants(AbstractExpression * expr, std::map<int, abs_ex> & system_vars_to_consts_indexes);

void setUpExpressionIntoVariable(abs_ex & func, const abs_ex & expr, int var);
void setUpExpressionIntoVariable(abs_ex & func, abs_ex && expr, int var);
void setUpExpressionIntoVariable(AbstractExpression * func, const abs_ex & expr, int var);
void setUpExpressionIntoVariable(AbstractExpression * func, abs_ex && expr, int var);
//отличие от ...BackToFunctions в том, что она снимает simplified с выражения и упрощает его.
//делать это в ...BackToFunctions нельзя, так как это вызовет бесконечную рекурсию
void replaceSystemVariablesToExpressions(AbstractExpression *expr, std::map<int, abs_ex> & funcs);
void replaceSystemVariablesToExpressions(abs_ex &expr, std::map<int, abs_ex> & funcs);
//void replaceSystemVariablesBackToFunctions(abs_ex & expr, std::vector<abs_ex> & functions);
//работает на все функции, не только тригонометрические
abs_ex getArgumentOfFunction(abs_ex && expr);
abs_ex getArgumentOfFunction(const abs_ex & expr);
abs_ex getArgumentOfFunction(AbstractExpression * expr);
void setNewArgumentToFunction(abs_ex & expr, const abs_ex & new_arg);

bool isDegreeOfTrigonometricalFunction(const abs_ex & expr);
bool isDegreeOfArcTrigonometricalFunction(abs_ex & expr);
//выражения a^f(x) считаются экспоненциальной ф-цией, а g(x)^f(x) - нет (x->getId() == var)
bool isExponentialFunction(const abs_ex& expr, int var);
abs_ex absEx(int num);
//проверяет, является ли func линейной функцией относительно переменной var
//если да, то возвращает коэффициенты a и b в выражении ax+b. Если нет, то возвращает пару nullptr, nullptr. b может быть нулем
std::pair<abs_ex, abs_ex> checkIfItsLinearFunction(const abs_ex & func, int var);
std::pair<abs_ex, abs_ex> checkIfItsLinearFunction(const AbstractExpression * func, int var);
std::array<abs_ex, 3> checkIfItsQuadraticFunction(const AbstractExpression * func, int var);
std::array<abs_ex, 3> checkIfItsQuadraticFunction(const abs_ex & func, int var);
//моном здесь - выражение вида x^a*b, где a - рациональное число, b - выражение, не содержащее x, x - переменная с id = var
//возвращает {a, b} или {Number::makeErrorNumber(), nullptr} если выражение не подходит под тип (если нет переменной x, то возвращает {0, func}
std::pair<Number, abs_ex> checkIfItsMonomOfSomeDegree(const AbstractExpression * func, int var);
//в данном случае, полином может иметь любые коэффициенты, однако целые степени var-переменной
//если это не полином относительно этой переменной, то возвращает пустой вектор
std::vector<abs_ex> checkIfItsPolynom(const AbstractExpression * func, int var);
std::vector<abs_ex> checkIfItsIntegerPolynom(const AbstractExpression * func, int var);
std::vector<abs_ex> checkIfItsPolynom(const abs_ex & func, int var);
std::vector<abs_ex> checkIfItsIntegerPolynom(const abs_ex & func, int var);
bool isZero(const abs_ex & expr);
abs_ex numToAbs(int num);
//тригонометрические и логарифмическая функции
std::pair<abs_ex, abs_ex> checkIfItsFunctionOfLinearArgument(const abs_ex & func, int var);
std::pair<abs_ex, abs_ex> checkIfItsFunctionOfLinearArgument(const AbstractExpression * func, int var);
abs_ex operator*(const abs_ex & left, const abs_ex & right);
abs_ex operator/(const abs_ex & left, const abs_ex & right);
abs_ex operator+(const abs_ex & left, const abs_ex & right);
abs_ex operator-(const abs_ex & left, const abs_ex & right);

abs_ex operator*(abs_ex && left, const abs_ex & right);
abs_ex operator/(abs_ex && left, const abs_ex & right);
abs_ex operator+(abs_ex && left, const abs_ex & right);
abs_ex operator-(abs_ex && left, const abs_ex & right);

abs_ex operator*(const abs_ex & left, abs_ex && right);
abs_ex operator/(const abs_ex & left, abs_ex && right);
abs_ex operator+(const abs_ex & left, abs_ex && right);
abs_ex operator-(const abs_ex & left, abs_ex && right);

abs_ex operator*(abs_ex && left, abs_ex && right);
abs_ex operator/(abs_ex && left, abs_ex && right);
abs_ex operator+(abs_ex && left, abs_ex && right);
abs_ex operator-(abs_ex && left, abs_ex && right);


abs_ex operator-(const abs_ex & arg);
abs_ex operator-(abs_ex && arg);
#endif // ABSTRACTEXPRESSION_H
