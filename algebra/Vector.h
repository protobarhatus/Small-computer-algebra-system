#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
//#include "abstractexpression.h"
#include <memory>
//#include "AlgExpr.h"




template <class T,class ... D>
void _fillVector(std::vector<T>& vec, int ind_of_start, D ... args);
template<class T,class D>
void _fillVector(std::vector<T>& vec, int ind_of_start, D arg);
template <class T,class first, class ... others>
void _fillVector(std::vector<T>& vec, int ind_of_start, first arg, others ... args);
template <class T,class ... type>
std::vector<T> getVector(type ... args);

template<typename T>
class Matrix;

template<typename T>
class Vector {
    std::vector<T> vector;
	int _size;
public:
	Vector();
    static Vector create(int size);
    template <class ... doubl>
    Vector(T a, doubl ... args)
	{
        this->vector = getVector<T>(a,  args...);
		this->_size = this->vector.size();
	}
	Vector(const Vector& vec);
	Vector(Vector&& vec) noexcept;
    Vector(const std::vector<T>& vec);
    Vector(std::vector<T>&& vec);
	//Vector(const Vector2& vec);
	//Vector(const Vector3d& vec);
	//Vector& operator=(const Vector2& vec);
	//Vector& operator=(const Vector3d& vec);
	Vector& operator=(const Vector& vec);
	Vector& operator=(Vector&& vec) noexcept;
    Vector& operator=(const std::vector<T>& vec);
    Vector& operator=(std::vector<T>&& vec);
	Vector null() const;
    T& operator[](int ind);
    const T& operator[](int ind) const;
    const T& x() const;
    const T& y() const;
    const T& z() const;
	
	Vector operator+(const Vector& second) const;
	Vector operator-(const Vector& second) const;
    Vector operator*(const T& num) const;
    Vector operator/(const T& num) const;

    Vector operator*(const Matrix<T>& second) const;

	Vector& operator+=(const Vector& second);
	Vector& operator-=(const Vector& second);
    Vector& operator*=(const T& num);
    Vector& operator/=(const T& num);

	int size() const;
    bool isZero() const;

};
template<typename T>
bool areColliniar(const Vector<T> & a, const Vector<T> & b);
template<typename T>
bool operator==(const Vector<T> & a, const Vector<T> & b);
template<typename T>
Vector<T> addDimension(const Vector<T> & vec, const T & z);
template<typename T>
Vector<T> addDimension(Vector<T> && vec, const T & z);
template<typename T>
Vector<T> addDimension(const Vector<T> & vec, T && z);
template<typename T>
Vector<T> addDimension(Vector<T> && vec, T && z);

class AlgExpr;
typedef Vector<AlgExpr> AlgVector;


template<typename T>
Vector<T> operator*(const T& num, const Vector<T>& vec);

template<typename T>
Vector<T> operator*(const Vector<T> & a, const Vector<T> & b);


template<typename T>
T scalar(const Vector<T>& a, const Vector<T>& b);
template<typename T>
T distance(const Vector<T>& a, const Vector<T>& b);
template<typename T>
T len(const Vector<T>& a);
template<typename T>
T divCollinear(const Vector<T> & a, const Vector<T> & b);

template<typename T,typename D>
void _fillVector(std::vector<T>& vec, int ind_of_start, D arg)
{
	vec[ind_of_start] = arg;
}
template <typename T,typename first, typename ... others>
void _fillVector(std::vector<T>& vec, int ind_of_start, first arg, others ... args)
{
	vec[ind_of_start] = arg;
	_fillVector(vec, ++ind_of_start, args...);
}

template <typename T,typename ... type>
std::vector<T> getVector(type ... args)
{
    std::vector<T> vec(sizeof ... (args));
	_fillVector(vec, 0, args...);
	return vec;
}
#include "Vector.inl"
#endif

