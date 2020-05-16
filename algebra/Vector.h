#pragma once
#include <vector>
#include "abstractexpression.h"
#include <memory>
#include "algexpr.h"




template <typename ... D>
void _fillVector(std::vector<AlgExpr>& vec, int ind_of_start, D ... args);
template<typename D>
void _fillVector(std::vector<AlgExpr>& vec, int ind_of_start, D arg);
template <typename first, typename ... others>
void _fillVector(std::vector<AlgExpr>& vec, int ind_of_start, first arg, others ... args);
template <typename ... type>
std::vector<AlgExpr> getVector(type ... args);
class Matrix;


class Vector {
    std::vector<AlgExpr> vector;
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
    Vector(const std::vector<AlgExpr>& vec);
    Vector(std::vector<AlgExpr>&& vec);
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
void _fillVector(std::vector<AlgExpr>& vec, int ind_of_start, D arg)
{
	vec[ind_of_start] = arg;
}
template <typename first, typename ... others>
void _fillVector(std::vector<AlgExpr>& vec, int ind_of_start, first arg, others ... args)
{
	vec[ind_of_start] = arg;
	_fillVector(vec, ++ind_of_start, args...);
}

template <typename ... type>
std::vector<AlgExpr> getVector(type ... args)
{
    std::vector<AlgExpr> vec(sizeof ... (args));
	_fillVector(vec, 0, args...);
	return vec;
}
