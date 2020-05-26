#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
//#include "abstractexpression.h"
#include <memory>
//#include "AlgExpr.h"




template <typename T,typename ... D>
void _fillVector(std::vector<T>& vec, int ind_of_start, D ... args);
template<typename T,typename D>
void _fillVector(std::vector<T>& vec, int ind_of_start, D arg);
template <typename T,typename first, typename ... others>
void _fillVector(std::vector<T>& vec, int ind_of_start, first arg, others ... args);
template <typename T,typename ... type>
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
	template <typename ... doubl>
	Vector(doubl ... args)
	{
		this->vector = getVector(args...);
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


};
template<typename T>
Vector<T> operator*(const T& num, const Vector<T>& vec);
template<typename T>
T scalar(const Vector<T>& a, const Vector<T>& b);
template<typename T>
T distance(const Vector<T>& a, const Vector<T>& b);
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
