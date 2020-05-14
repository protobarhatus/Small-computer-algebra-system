#pragma once
#include <vector>
#include "abstractexpression.h"
#include <memory>

template <typename ... D>
void _fillVector(std::vector<abs_ex>& vec, int ind_of_start, D ... args);
template<typename D>
void _fillVector(std::vector<abs_ex>& vec, int ind_of_start, D arg);
template <typename first, typename ... others>
void _fillVector(std::vector<abs_ex>& vec, int ind_of_start, first arg, others ... args);
template <typename ... type>
std::vector<abs_ex> getVector(type ... args);
class Matrix;
class AbstractExpressio 	{
public:
    AbstractExpressio();
    AbstractExpressio & operator=(const AbstractExpressio & expr);
    AbstractExpressio(const AbstractExpressio & expr);
    virtual ~AbstractExpressio();
    virtual void simplify() = 0;
    virtual AlgebraExpression getId() const = 0;
    //also system comparing, not math
    static bool less(const AbstractExpressio * left, const AbstractExpressio * right);
    //function for std::sort
    static bool lessToSort(const std::unique_ptr<AbstractExpressio> & left, const std::unique_ptr<AbstractExpressio> & right);

    virtual bool operator==(AbstractExpressio & right) = 0;
    bool operator!=(AbstractExpressio & right);
    //returns simplified and downcasted fractal
    std::unique_ptr<AbstractExpressio> operator*(AbstractExpressio & expr);
    std::unique_ptr<AbstractExpressio> operator/(AbstractExpressio & expr);
    //returns simplified and downcasted polynomial
    std::unique_ptr<AbstractExpressio> operator+(AbstractExpressio & expr);
    std::unique_ptr<AbstractExpressio> operator-(AbstractExpressio & expr);
    virtual bool canDowncastTo(AlgebraExpression expr) = 0;
    //need an addition when new types of expression added
    bool canDowncast();

    //ВНИМАНИЕ!: сейчас downcastTo занимается копированием, однако, мне, наверно стоит изменить это на перемещение
    virtual std::unique_ptr<AbstractExpressio> downcastTo(AlgebraExpression expr) = 0;

    //does downcasting till can for unlimitted amount of levels
    std::unique_ptr<AbstractExpressio> downcast();
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
private:
    //subclasses assume that right is the same subclass, so they downcasting it momentally. if it not the same, assert is calling
    virtual bool operator<(const AbstractExpressio & right) const = 0;
protected:
    bool simplified = false;
    virtual int getPositionRelativelyZeroIfHasVariables() = 0;

};
class C : public AbstractExpressio {

};
class Vector {
    std::vector<std::unique_ptr<AbstractExpressio>> vect;
    std::vector<abs_ex> vector;
	int _size;
public:
	Vector();
	static Vector create(int size);
	template <typename ... doubl>
	Vector(doubl ... args)
	{
		this->vector = getVector(args...);
		this->_size = this->vector.size();
	}
	Vector(const Vector& vec);
	Vector(Vector&& vec) noexcept;
	Vector(const std::vector<abs_ex>& vec);
	Vector(std::vector<abs_ex>&& vec);
	//Vector(const Vector2& vec);
	//Vector(const Vector3d& vec);
	//Vector& operator=(const Vector2& vec);
	//Vector& operator=(const Vector3d& vec);
	Vector& operator=(const Vector& vec);
	Vector& operator=(Vector&& vec) noexcept;
	Vector& operator=(const std::vector<abs_ex>& vec);
	Vector& operator=(std::vector<abs_ex>&& vec);
	Vector null() const;
	abs_ex& operator[](int ind);
	const abs_ex& operator[](int ind) const;
	const abs_ex& x() const;
	const abs_ex& y() const;
	const abs_ex& z() const;
	
	Vector operator+(const Vector& second) const;
	Vector operator-(const Vector& second) const;
	Vector operator*(const abs_ex& num) const;
	Vector operator/(const abs_ex& num) const;

	Vector operator*(const Matrix& second) const;

	Vector& operator+=(const Vector& second);
	Vector& operator-=(const Vector& second);
	Vector& operator*=(const abs_ex& num);
	Vector& operator/=(const abs_ex& num);

	
	int size() const;


};
Vector operator*(abs_ex num, const Vector& vec);
abs_ex scalar(const Vector& a, const Vector& b);
abs_ex distance(const Vector& a, const Vector& b);
template<typename D>
void _fillVector(std::vector<abs_ex>& vec, int ind_of_start, D arg)
{
	vec[ind_of_start] = arg;
}
template <typename first, typename ... others>
void _fillVector(std::vector<abs_ex>& vec, int ind_of_start, first arg, others ... args)
{
	vec[ind_of_start] = arg;
	_fillVector(vec, ++ind_of_start, args...);
}

template <typename ... type>
std::vector<abs_ex> getVector(type ... args)
{
	std::vector<abs_ex> vec(sizeof ... (args));
	_fillVector(vec, 0, args...);
	return vec;
}
