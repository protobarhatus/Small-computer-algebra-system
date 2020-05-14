#pragma once
#include <vector>
#include <functional>
#include "Vector.h"
class Matrix {
	std::vector<Vector> matrix;
	int _lines, _columns;
public:
	Matrix(const std::vector<Vector> & matrix);
	Matrix(std::vector<Vector>&& matrix);
	Matrix(int m, int n);
	
	Matrix(int m, int n, const std::function<abs_ex(int, int)>& formula);
	Matrix(const Matrix& copy);
	Matrix(Matrix&& matr) noexcept;

	Matrix& operator=(const std::vector<Vector >& matrix);
	Matrix& operator=(std::vector<Vector>&& matrix);
	Matrix& operator=(const Matrix& matr);
	Matrix& operator=(Matrix&& matr);

	Vector& operator[](int i);
	const Vector& operator[](int i) const;

	int lines() const;
	int columns() const;
	int size() const;

	Matrix operator+(const Matrix& sec) const;
	Matrix operator-(const Matrix& sec) const;
	Matrix operator*(const abs_ex& num) const;
	Matrix operator/(const abs_ex& num) const;
	Matrix operator*(const Matrix& sec) const;
	Vector operator*(const Vector& sec) const;

	Matrix null();
	

	Matrix& operator+=(const Matrix& sec);
	Matrix& operator-=(const Matrix& sec);
	Matrix& operator*=(const abs_ex& num);
	Matrix& operator/=(const abs_ex& num);
	bool isSquare() const;
};

Matrix makeUnitMatrix(int size);
abs_ex det(const Matrix& matrix);
std::vector<std::vector<abs_ex> > gauss(Matrix&& extended_equation_matrix);
Matrix inverse(Matrix matrix);
