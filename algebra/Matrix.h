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
    //void out() const;
};
template<typename T>
Matrix<T> makeUnitMatrix(int size);
template<typename T>
T det(const Matrix<T>& matrix);
template<typename T>
std::vector<std::vector<T> > gauss(Matrix<T>&& extended_equation_matrix);
template<typename T>
Matrix<T> inverse(Matrix<T> matrix);
//Угол в радианах и ПРОТИВ часовой стрелки. При повороте ПО часовой стрелке угол ОТРИЦАТЕЛЬНЫЙ
template<typename T>
Matrix<T> rotationMatrix2D(const T& angle);

#include "Matrix.inl"
#endif
