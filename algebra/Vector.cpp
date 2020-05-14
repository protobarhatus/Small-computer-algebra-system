#include "Vector.h"
#include "Matrix.h"
#include <cmath>
#include "number.h"
#include "some_algebra_expression_conversions.h"
#include "degree.h"
Vector::Vector()
{
}

Vector Vector::create(int size)
{
	Vector res;
	res.vector.resize(size);
	res._size = size;
	return res;
}

Vector::Vector(const Vector& vec)
{
    this->vector.resize(vec.vector.size());
    for (int i = 0; i < this->vector.size(); ++i)
        this->vector[i] = copy(vec.vector[i]);
	this->_size = this->vector.size();
}
Vector::Vector(Vector&& vec) noexcept
{
	this->vector = std::move(vec.vector);
	this->_size = this->vector.size();
}
Vector::Vector(const std::vector<abs_ex>& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = copy(vec[i]);
	this->_size = this->vector.size();
}
Vector::Vector(std::vector<abs_ex>&& vec)
{
	this->vector = std::move(vec);
	this->_size = this->vector.size();
}

/*Vector::Vector(const Vector2& vec)
{
	this->vector = getVector(vec.x(), vec.y());
	this->_size = 2;
}

Vector::Vector(const Vector3d& vec)
{
	this->vector = getVector(vec.x(), vec.y(), vec.z());
	this->_size = 3;
}

Vector& Vector::operator=(const Vector2& vec)
{
	*this = Vector(vec);
	return *this;
}

Vector& Vector::operator=(const Vector3d& vec)
{
	*this = Vector(vec);
	return *this;
}*/

Vector& Vector::operator=(const Vector& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = copy(vec[i]);
	this->_size = this->vector.size();
	return *this;
}
Vector& Vector::operator=(Vector&& vec) noexcept
{
	this->vector = std::move(vec.vector);
	this->_size = this->vector.size();
	return *this;
}

Vector& Vector::operator=(const std::vector<abs_ex>& vec)
{
    this->vector.resize(vec.size());
    for (int i = 0; i < vec.size(); ++i)
        this->vector[i] = copy(vec[i]);
	this->_size = this->vector.size();
	return *this;
}
Vector& Vector::operator=(std::vector<abs_ex>&& vec)
{
    this->vector = std::move(vec);
	this->_size = this->vector.size();
	return *this;
}
Vector Vector::null() const
{
    std::vector<abs_ex> vec(this->vector.size());
    for (auto &it : vec)
        it = copy(zero);
	return vec;
}
abs_ex& Vector::operator[](int ind)
{
	if (ind > this->_size)
		throw "Index out of borders";
	return this->vector[ind];
}
const abs_ex& Vector::operator[](int ind) const
{
	if (ind > this->_size)
		throw "Index out of borders";
	return this->vector[ind];
}
const abs_ex& Vector::x() const
{
	return this->vector[0];
}
const abs_ex& Vector::y() const
{
	if (this->_size == 1)
		throw "reference to nonexistent dimension";
	return this->vector[1];
}
const abs_ex& Vector::z() const
{
	if (this->_size < 3)
		throw "reference to nonexistent dimension";
	return this->vector[2];
}
Vector Vector::operator+(const Vector& second) const
{
	if (this->_size != second._size)
		throw "Addition of vectors with different sizes";
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] + second.vector[i];
	return res;
}
Vector Vector::operator-(const Vector& second) const
{
	if (this->_size != second._size)
		throw "Substraction of vectors with different sizes";
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] - second.vector[i];
	return res;
}
Vector Vector::operator*(const abs_ex& num) const
{
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] * num;
	return res;
}
Vector Vector::operator/(const abs_ex& num) const
{
	Vector res = *this;
	for (int i = 0; i < this->_size; ++i)
        res.vector[i] = res.vector[i] / num;
	return res;
}

Vector Vector::operator*(const Matrix& second) const
{
	return second * *this;
}
Vector& Vector::operator+=(const Vector& second)
{
	if (this->_size != second._size)
		throw "Addition of vectors with different sizes";
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] + second.vector[i];
	return *this;
}
Vector& Vector::operator-=(const Vector& second)
{
	if (this->_size != second._size)
		throw "Substraction of vectors with different sizes";
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] - second.vector[i];
	return *this;
}
Vector& Vector::operator*=(const abs_ex& num)
{
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] * num;
	return *this;
}
Vector& Vector::operator/=(const abs_ex& num)
{
	for (int i = 0; i < this->_size; ++i)
        this->vector[i] = this->vector[i] / num;
	return *this;
}

Vector operator*(abs_ex num, const Vector& vec)
{
	return vec * num;
}
int Vector::size() const
{
	return this->_size;
}
abs_ex scalar(const Vector& a, const Vector& b)
{
	if (a.size() != b.size())
		throw "Scalar multiplication of different dimension vectors";
    abs_ex res = copy(zero);
	for (int i = 0; i < a.size(); ++i)
        res = res +( a[i] * b[i]);
	return res;
}

abs_ex distance(const Vector& a, const Vector& b)
{
	if (a.size() != b.size())
		throw "Cannot count distance of vectors with different size";
    abs_ex res = copy(zero);
	for (int i = 0; i < a.size(); ++i)
        res = res + (a[i] - b[i]) * (a[i] - b[i]);
	return sqrt(res);
}
