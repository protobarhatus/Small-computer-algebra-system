#include "Matrix.h"
#include <algorithm>
#include "some_algebra_expression_conversions.h"
#include "number.h"
Matrix::Matrix(const std::vector<Vector>& _matrix)
{
	this->matrix = _matrix;
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
}
Matrix::Matrix(std::vector<Vector>&& _matrix)
{
	this->matrix = std::move(_matrix);
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
}
Matrix::Matrix(int m, int n)
{
    this->matrix = std::vector<Vector>(m, std::vector<AlgExpr>(n, AlgExpr(0)));
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
}
Matrix::Matrix(int m, int n, const std::function<abs_ex(int, int)>& formula)
{
    this->matrix = std::vector<Vector>(m, std::vector<AlgExpr>(n, AlgExpr(0)));
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			this->matrix[i][j] = formula(i, j);
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
}
Matrix::Matrix(const Matrix& copy)
{
	this->matrix = copy.matrix;
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
}
Matrix::Matrix(Matrix&& matr) noexcept
{
	this->matrix = std::move(matr.matrix);
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
}
Matrix& Matrix::operator=(const std::vector<Vector>& matr)
{
	this->matrix = matr;
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
	return *this;
}
Matrix& Matrix::operator=(std::vector<Vector>&& matr)
{
	this->matrix = std::move(matr);
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
	return *this;
}
Matrix& Matrix::operator=(const Matrix& matr)
{
	this->matrix = matr.matrix;
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
	return *this;
}
Matrix& Matrix::operator=(Matrix&& matr)
{
	this->matrix = std::move(matr.matrix);
	this->_lines = matrix.size();
	this->_columns = matrix[0].size();
	return *this;
}

Vector& Matrix::operator[](int i)
{
	return this->matrix[i];
}

const Vector& Matrix::operator[](int i) const
{
	return this->matrix[i];
}

int Matrix::lines() const
{
	return this->_lines;
}

int Matrix::columns() const
{
	return this->_columns;
}

int Matrix::size() const
{
	if (this->_lines != this->_columns)
		throw "Size of non square matrix is not defined";
	return this->_lines;
}

Matrix Matrix::operator+(const Matrix& sec) const
{
	if (this->_lines != sec._lines || this->_columns != sec._columns)
		throw "Addition of different size matrixes";
	Matrix res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
			res[i][j] = this->matrix[i][j] + sec.matrix[i][j];
	return res;
}

Matrix Matrix::operator-(const Matrix& sec) const
{
	if (this->_lines != sec._lines || this->_columns != sec._columns)
		throw "Addition of different size matrixes";
	Matrix res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
			res[i][j] = this->matrix[i][j] - sec.matrix[i][j];
	return res;
}

Matrix Matrix::operator*(const abs_ex& num) const
{
	Matrix res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
			res[i][j] = this->matrix[i][j] * num;
	return res;
}

Matrix Matrix::operator/(const abs_ex& num) const
{
	Matrix res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
			res[i][j] = this->matrix[i][j] / num;
	return res;
}

Matrix Matrix::operator*(const Matrix& sec) const
{
	if (this->_columns != sec._lines)
		throw "Multiplication of incompatible matrixes";
	Matrix res(this->_lines, sec._columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < sec._columns; ++j)
		{
			abs_ex sum = copy(zero);
			for (int r = 0; r < this->_columns; ++r)
				sum = sum + this->matrix[i][r] * sec[r][j];
			res[i][j] = std::move(sum);
		}
	return res;
}

Vector Matrix::operator*(const Vector& sec) const
{
	if (!this->isSquare() || this->size() != sec.size())
		throw "Multiplication of incompatible matrix and vector";
	Vector res = sec.null();
	for (int i = 0; i < res.size(); ++i)
	{
		for (int j = 0; j < res.size(); ++j)
		{
			res[i] = res[i] + this->matrix[i][j] * sec[j];
		}
	}
	return res;
}

Matrix Matrix::null()
{
	Matrix res(this->_lines, this->_columns);
	return res;
}


Matrix& Matrix::operator+=(const Matrix& sec)
{
	*this = *this + sec;
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& sec)
{
	*this = *this - sec;
	return *this;
}

Matrix& Matrix::operator*=(const abs_ex& num)
{
	*this = *this * num;
	return *this;
}

Matrix& Matrix::operator/=(const abs_ex& num)
{
	*this = *this / num;
	return *this;
}

bool Matrix::isSquare() const
{
	return this->_lines == this->_columns;
}

Matrix makeUnitMatrix(int size)
{
	return Matrix(size, size, [](int i, int j) {return (i == j ? copy(one) : copy(zero)); });
}

abs_ex det(const Matrix& matrix)
{
	if (!matrix.isSquare())
		throw "Attemt to count determinant of non square matrix";
	if (matrix.lines() == 1)
        return copy(matrix[0][0]);
	if (matrix.lines() == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    abs_ex d = copy(zero);
	for (int i = 0; i < matrix.columns(); ++i)
        d = d + (i % 2 == 0 ? copy(one) : -one) * det(Matrix(matrix.lines() - 1, matrix.columns() - 1, [i, &matrix](int k, int j)->abs_ex { return copy(matrix[k + 1][j + (j >= i ? 1 : 0)]); }));
	return d;
}

Matrix inverse(Matrix matrix)
{
	if (!matrix.isSquare())
		throw "Attempt to inverse non square matrix";
	Matrix result = makeUnitMatrix(matrix.size());
	int height = matrix.size();
	int width = matrix[0].size();
	auto get_index_of_first_non_zero = [](Vector& vec)->int {
		for (int i = 0; i < vec.size(); ++i)
            if (*vec[i] != *zero)
				return i;
		return vec.size();
	};
	std::vector<std::pair<int, int> > indexes_of_first_non_zero(height);
	auto sort_rows = [height, &get_index_of_first_non_zero, &matrix, &indexes_of_first_non_zero, &result]()->void {
		for (int i = 0; i < height; ++i)
		{
			indexes_of_first_non_zero[i] = { i, get_index_of_first_non_zero(matrix[i]) };
		}
		std::sort(indexes_of_first_non_zero.begin(), indexes_of_first_non_zero.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
			return a.second < b.second; });
		for (int i = 0; i < height; ++i)
		{
			if (i < indexes_of_first_non_zero[i].first)
			{
				std::swap(matrix[i], matrix[indexes_of_first_non_zero[i].first]);
				std::swap(result[i], result[indexes_of_first_non_zero[i].first]);
			}
		}
	};
	auto divide_on_lead_element = [&matrix, &result](int x)->void {
        abs_ex lead_el = copy(matrix[x][x]);
		for (int i = x; i < matrix.size(); ++i)
		{
            matrix[x][i] = matrix[x][i] / lead_el;
		}
		for (int i = 0; i < matrix.size(); ++i)
		{
            result[x][i]  = result[x][i] / lead_el;
		}
	};
	sort_rows();

	for (int n = 0; n < height; ++n) {
		divide_on_lead_element(n);
		for (int i = n + 1; i < height; ++i)
		{
			abs_ex multiplier = -matrix[i][n] / matrix[n][n];
			matrix[i] = matrix[i] + matrix[n] * (multiplier);
			result[i] = result[i] + result[n] * (multiplier);


		}

		//sort_rows();
	}
    /*for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j < matrix[i].size(); ++j)
			if (abs(matrix[i][j]) < 1e-15)
                matrix[i][j] = 0;*/
	for (int n = height - 1; n >= 0; --n)
	{
		for (int i = n - 1; i >= 0; --i)
		{
			abs_ex multiplier = -matrix[i][n] / matrix[n][n];
			matrix[i] = matrix[i] + matrix[n] * (multiplier);
			result[i] = result[i] + result[n] * (multiplier);
		}
	}
    /*for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j < matrix[i].size(); ++j)
			if (abs(matrix[i][j]) < 1e-15)
                matrix[i][j] = 0;*/
	return result;
}
std::vector<std::vector<abs_ex> > gauss(Matrix&& extended_equation_matrix)
{
	int height = extended_equation_matrix.lines();
	int width = extended_equation_matrix.columns();
	auto get_index_of_first_non_zero = [](Vector& vec)->int {
		for (int i = 0; i < vec.size(); ++i)
            if (*vec[i] != *zero)
				return i;
		return vec.size();
	};
	std::vector<std::pair<int, int> > indexes_of_first_non_zero(height);
	auto sort_rows = [height, &get_index_of_first_non_zero, &extended_equation_matrix, &indexes_of_first_non_zero]()->void {
		for (int i = 0; i < height; ++i)
		{
			indexes_of_first_non_zero[i] = { i, get_index_of_first_non_zero(extended_equation_matrix[i]) };
		}
		std::sort(indexes_of_first_non_zero.begin(), indexes_of_first_non_zero.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
			return a.second < b.second; });
		for (int i = 0; i < height; ++i)
		{
			if (i < indexes_of_first_non_zero[i].first)
				std::swap(extended_equation_matrix[i], extended_equation_matrix[indexes_of_first_non_zero[i].first]);
		}
	};

	sort_rows();

	for (int n = 0; n < height; ++n) {
		for (int i = n + 1; i < height; ++i)
		{
			if (indexes_of_first_non_zero[i].second == indexes_of_first_non_zero[n].second)
				extended_equation_matrix[i] = extended_equation_matrix[i] +
				extended_equation_matrix[n] * (-extended_equation_matrix[i][indexes_of_first_non_zero[i].second] / extended_equation_matrix[n][indexes_of_first_non_zero[n].second]);
			//outMatrix(extended_equation_matrix);

		}
		sort_rows();
	}
	//!!!!!!!!!!!!!!!
    /*for (int i = 0; i < extended_equation_matrix.lines(); ++i)
		for (int j = 0; j < extended_equation_matrix[i].size(); ++j)
			if (abs(extended_equation_matrix[i][j]) < 1e-15)
                extended_equation_matrix[i][j] = 0;*/
	int first_non_zero = height - 1;
    for (first_non_zero = height - 1; *extended_equation_matrix[first_non_zero][width - 2] == *zero; --first_non_zero)
        if (*extended_equation_matrix[first_non_zero][width - 1] != *zero)
			return std::vector<std::vector<abs_ex> >();
	auto div_on_lead_element = [](Vector& vec)->void {
        abs_ex lead_el = copy(zero);
		for (int i = 0; i < vec.size(); ++i)
		{
            if (*lead_el == *zero)
                lead_el = copy(vec[i]);
            if (*lead_el != *zero)
                vec[i] = vec[i]/ lead_el;
		}
	};

	for (int i = 0; i < extended_equation_matrix.lines(); ++i)
		div_on_lead_element(extended_equation_matrix[i]);
	int amount_of_variables = width - 1;
	//������ ������ - ������ ���������� ������� ��������, ������ - ������ �������, ������ ��� ���������� - ���������
	std::vector<std::vector<abs_ex> > result(amount_of_variables);
	int current_params_index = width - 2;
	for (int i = first_non_zero; i >= 0; --i)
	{
        while (*extended_equation_matrix[i][current_params_index] != *one)
			--current_params_index;
		result[current_params_index].resize(amount_of_variables + 1);
		for (int j = width - 1; j > current_params_index; --j)
		{
            result[current_params_index][j] = (j == width - 1 ? copy(one) : -one) * extended_equation_matrix[i][j];
		}
		for (int j = i - 1; j >= 0; --j)
		{
			extended_equation_matrix[j] = extended_equation_matrix[j] + (extended_equation_matrix[i] * (-extended_equation_matrix[j][current_params_index]));
		}
	}
	return result;
	
}
