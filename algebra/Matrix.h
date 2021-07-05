#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <functional>
#include "Vector.h"
template<typename T>
class Matrix {
    std::vector<Vector<T>> matrix;
	int _lines, _columns;
public:
    Matrix(const std::vector<Vector<T>> & matrix);
    Matrix(std::vector<Vector<T>>&& matrix);
	Matrix(int m, int n);
	
    Matrix(int m, int n, const std::function<T(int, int)>& formula);
	Matrix(const Matrix& copy);
	Matrix(Matrix&& matr) noexcept;

    Matrix& operator=(const std::vector<Vector<T> >& matrix);
    Matrix& operator=(std::vector<Vector<T>>&& matrix);
	Matrix& operator=(const Matrix& matr);
	Matrix& operator=(Matrix&& matr);

    Vector<T>& operator[](int i);
    const Vector<T>& operator[](int i) const;

	int lines() const;
	int columns() const;
	int size() const;

	Matrix operator+(const Matrix& sec) const;
	Matrix operator-(const Matrix& sec) const;
    Matrix operator*(const T& num) const;
    Matrix operator/(const T& num) const;
	Matrix operator*(const Matrix& sec) const;
    Vector<T> operator*(const Vector<T>& sec) const;

	Matrix null();
	

	Matrix& operator+=(const Matrix& sec);
	Matrix& operator-=(const Matrix& sec);
    Matrix& operator*=(const T& num);
    Matrix& operator/=(const T& num);
	bool isSquare() const;
   // void out() const;
};
template<typename T>
Matrix<T> makeUnitMatrix(int size);
template<typename T>
T det(const Matrix<T>& matrix);

//пусть есть система {a1_1 * x1 + a1_2 * x2 + ... + a1_n * xn == A1; a2_1 * x1 + ... == A2; ...; an_1 * x1 + ... + == An}
//на вход принимается матрица { {a1_1, a1_2, a1_3, ..... , A1}, .... } размера n X (n + 1)
//на выходе матрица r[n][n + 1]
//значение i-ной переменной (назовем задается строкой r[i] и выглядит как
//x_i  = r[i][0] * x_1 + r[i][1] * x_2 + ... + r[i][n],
//при этом если r[i][k] при k != n не нулевые, то переменные, к которым они относятся являются параметрами, решение-
//параметрическое относительно них, при этом r[k][k] = 1, а все остальные r[k][w] = 0
//!если используется тип, который необходимо инициализировать, то нулевые значения r могут быть не созданы (или nullptr)
//! При использовании с типом с плавающей точкой может возникать ошибка, чек комментарий внутри функции
template<typename T>
std::vector<std::vector<T> > gauss(Matrix<T>&& extended_equation_matrix);
template<typename T>
Matrix<T> inverse(Matrix<T> matrix);
//Угол в радианах и ПРОТИВ часовой стрелки. При повороте ПО часовой стрелке угол ОТРИЦАТЕЛЬНЫЙ
template<typename T>
Matrix<T> rotationMatrix2D(const T& angle);

#include "Matrix.inl"
#endif
