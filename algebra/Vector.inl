#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"
#include "Matrix.h"
#include <cmath>
#include "number.h"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
template<typename T>
Vector<T>::Vector()
{
}
template<typename T>
Vector<T> Vector<T>::create(int size)
{
    Vector<T> res;
    res.vector.resize(size);
	res._size = size;
	return res;
}
template<typename T>
Vector<T>::Vector(const Vector<T>& vec)
{
    this->vector.resize(vec.vector.size());
    for (int i = 0; i < this->vector.size(); ++i)
        this->vector[i] = vec.vector[i];
    this->_size = this->vector.size();
}
template<typename T>
Vector<T>::Vector(Vector<T>&& vec)  noexcept
{
    this->vector = std::move(vec.vector);
    this->_size = this->vector.size();
}
template<typename T>
Vector<T>::Vector(const std::vector<T>& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = vec[i];
    this->_size = this->vector.size();
}
template<typename T>
Vector<T>::Vector(std::vector<T>&& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = std::move(vec[i]);
    this->_size = this->vector.size();
}


/*Vector<T>::Vector(const Vector<T>2& vec)
{
    this->vector = getVector<T>(vec.x(), vec.y());
	this->_size = 2;
}

Vector<T>::Vector(const Vector<T>3d& vec)
{
    this->vector = getVector<T>(vec.x(), vec.y(), vec.z());
	this->_size = 3;
}

Vector<T>& Vector<T>::operator=(const Vector<T>2& vec)
{
    *this = Vector<T>(vec);
	return *this;
}

Vector<T>& Vector<T>::operator=(const Vector<T>3d& vec)
{
    *this = Vector<T>(vec);
	return *this;
}*/
template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = vec[i];
    this->_size = this->vector.size();
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& vec) noexcept
{
    this->vector = std::move(vec.vector);
    this->_size = this->vector.size();
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::operator=(const std::vector<T>& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = vec[i];
    this->_size = this->vector.size();
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::operator=(std::vector<T>&& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = std::move(vec[i]);
    this->_size = this->vector.size();
	return *this;
}
template<typename T>
Vector<T> Vector<T>::null() const
{
    std::vector<T> vec(this->vector.size());
    for (auto &it : vec)
        it = 0;
	return vec;
}
template<typename T>
T& Vector<T>::operator[](int ind)
{
	if (ind > this->_size)
		throw "Index out of borders";
    return this->vector[ind];
}
template<typename T>
const T& Vector<T>::operator[](int ind) const
{
	if (ind > this->_size)
		throw "Index out of borders";
    return this->vector[ind];
}
template<typename T>
const T& Vector<T>::x() const
{
    return this->vector[0];
}
template<typename T>
const T& Vector<T>::y() const
{
	if (this->_size == 1)
		throw "reference to nonexistent dimension";
    return this->vector[1];
}
template<typename T>
const T& Vector<T>::z() const
{
	if (this->_size < 3)
		throw "reference to nonexistent dimension";
    return this->vector[2];
}
template<typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& second) const
{
	if (this->_size != second._size)
        throw "Addition of Vector<T>s with different sizes";
    Vector<T> res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] + second.vector[i];
	return res;
}
template<typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& second) const
{
	if (this->_size != second._size)
        throw "Substraction of Vector<T>s with different sizes";
    Vector<T> res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] - second.vector[i];
	return res;
}
template<typename T>
Vector<T> Vector<T>::operator*(const T& num) const
{
    Vector<T> res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] * num;
	return res;
}
template<typename T>
Vector<T> Vector<T>::operator/(const T& num) const
{
    Vector<T> res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] / num;
	return res;
}
template<typename T>
Vector<T> Vector<T>::operator*(const Matrix<T>& second) const
{
	return second * *this;
}
template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& second)
{
	if (this->_size != second._size)
        throw "Addition of Vector<T>s with different sizes";
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] + second.vector[i];
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& second)
{
	if (this->_size != second._size)
        throw "Substraction of Vector<T>s with different sizes";
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] - second.vector[i];
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::operator*=(const T& num)
{
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] * num;
	return *this;
}
template<typename T>
Vector<T>& Vector<T>::operator/=(const T& num)
{
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] / num;
	return *this;
}
template<typename T>
Vector<T> operator*(const T& num, const Vector<T>& vec)
{
	return vec * num;
}
template<typename T>
int Vector<T>::size() const
{
	return this->_size;
}
template<typename T>
T scalar(const Vector<T>& a, const Vector<T>& b)
{
	if (a.size() != b.size())
        throw "Scalar multiplication of different dimension Vector<T>s";
    T res = 0;
	for (int i = 0; i < a.size(); ++i)
        res = res +( a[i] * b[i]);
	return res;
}
template<typename T>
T distance(const Vector<T>& a, const Vector<T>& b)
{
	if (a.size() != b.size())
        throw "Cannot count distance of Vector<T>s with different size";
    T res = 0;
	for (int i = 0; i < a.size(); ++i)
        res = res + (a[i] - b[i]) * (a[i] - b[i]);
	return sqrt(res);
}


template<typename T>
Vector<T> operator*(const Vector<T> & a, const Vector<T> & b)
{
    if (a.size() != 3 || b.size() != 3)
        throw "Cannot multiply non 3 dimensional vectors";
     return Vector<T>(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
}

template<typename T>
T len(const Vector<T>& a)
{
    T res = 0;
    for (int i = 0; i < a.size(); ++i)
        res = res + a[i] * a[i];
    return sqrt(res);
}
template<typename T>
Vector<T> addDimension(const Vector<T> & vec, const T & z)
{
    Vector<T> new_vec = Vector<T>::create(vec.size() + 1);
    for (int i = 0; i < vec.size(); ++i)
        new_vec[i] = vec[i];
    new_vec[vec.size()] = z;
    return new_vec;
}
template<typename T>
Vector<T> addDimension(Vector<T> && vec, const T & z)
{
    Vector<T> new_vec = Vector<T>::create(vec.size() + 1);
    for (int i = 0; i < vec.size(); ++i)
        new_vec[i] = std::move(vec[i]);
    new_vec[vec.size()] = z;
    return new_vec;
}
template<typename T>
Vector<T> addDimension(const Vector<T> & vec, T && z)
{
    Vector<T> new_vec = Vector<T>::create(vec.size() + 1);
    for (int i = 0; i < vec.size(); ++i)
        new_vec[i] = vec[i];
    new_vec[vec.size()] = std::move(z);
    return new_vec;
}
template<typename T>
Vector<T> addDimension(Vector<T> && vec, T && z)
{
    Vector<T> new_vec = Vector<T>::create(vec.size() + 1);
    for (int i = 0; i < vec.size(); ++i)
        new_vec[i] = std::move(vec[i]);
    new_vec[vec.size()] = std::move(z);
    return new_vec;
}
template<typename T>
bool Vector<T>::isZero() const
{
    for (auto &it : this->vector)
        if (it != 0)
            return false;
    return true;
}
template<typename T>
T divCollinear(const Vector<T> & a, const Vector<T> & b)
{
    if (a == b)
        return 1;
    if (b.x() != 0)
        return a.x() / b.x();
    if (b.y() != 0)
        return a.y() / b.y();
    if (b.z() != 0)
        return a.z() / b.z();
    assert(false);
    return 0;
}
template<typename T>
bool operator==(const Vector<T> & a, const Vector<T> & b)
{
    if (a.size() != b.size())
        throw (QString)"Comparation of incomparable vectors of different size";
    for (int i = 0; i < a.size(); ++i)
        if (a[i] != b[i])
            return false;
    return true;
}


#endif
