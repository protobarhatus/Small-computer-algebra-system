#ifndef MATRIX_CPP
#define MATRIX_CPP
#include "Matrix.h"
#include <algorithm>
#include "some_algebra_expression_conversions.h"
#include "number.h"
template<typename T>
Matrix<T>::Matrix(const std::vector<Vector<T>>& _matrix)
{
    this->matrix = _matrix;
    this->_lines = _matrix.size();
    this->_columns = _matrix[0].size();
}
template<typename T>
Matrix<T>::Matrix(std::vector<Vector<T>>&& _matrix)
{
    this->matrix = std::move(_matrix);
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
}
template<typename T>
Matrix<T>::Matrix(int m, int n)
{
    this->matrix = std::vector<Vector<T>>(m, std::vector<T>(n, T(0)));
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
}
template<typename T>
Matrix<T>::Matrix(int m, int n, const std::function<T(int, int)>& formula)
{
    this->matrix = std::vector<Vector<T>>(m, std::vector<T>(n, T(0)));
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
            this->matrix[i][j] = formula(i, j);
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
}
template<typename T>
Matrix<T>::Matrix(const Matrix<T>& copy)
{
    this->matrix = copy.matrix;
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
}
template<typename T>
Matrix<T>::Matrix(Matrix<T>&& matr) noexcept
{
    this->matrix = std::move(matr.matrix);
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
}
template<typename T>
Matrix<T>& Matrix<T>::operator=(const std::vector<Vector<T>>& matr)
{
    this->matrix = matr;
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::operator=(std::vector<Vector<T>>&& matr)
{
    this->matrix = std::move(matr);
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matr)
{
    this->matrix = matr.matrix;
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& matr)
{
    this->matrix = std::move(matr.matrix);
    this->_lines = matrix.size();
    this->_columns = matrix[0].size();
	return *this;
}
template<typename T>
Vector<T>& Matrix<T>::operator[](int i)
{
    return this->matrix[i];
}
template<typename T>
const Vector<T>& Matrix<T>::operator[](int i) const
{
    return this->matrix[i];
}
template<typename T>
int Matrix<T>::lines() const
{
	return this->_lines;
}
template<typename T>
int Matrix<T>::columns() const
{
	return this->_columns;
}
template<typename T>
int Matrix<T>::size() const
{
	if (this->_lines != this->_columns)
        throw "Size of non square Matrix<T> is not defined";
	return this->_lines;
}
template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& sec) const
{
	if (this->_lines != sec._lines || this->_columns != sec._columns)
        throw "Addition of different size Matrix<T>es";
    Matrix<T> res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
            res[i][j] = this->matrix[i][j] + sec.matrix[i][j];
	return res;
}
template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& sec) const
{
	if (this->_lines != sec._lines || this->_columns != sec._columns)
        throw "Subtraction of different size Matrix<T>es";
    Matrix<T> res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
            res[i][j] = this->matrix[i][j] - sec.matrix[i][j];
	return res;
}
template<typename T>
Matrix<T> Matrix<T>::operator*(const T& num) const
{
    Matrix<T> res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
            res[i][j] = this->matrix[i][j] * num;
	return res;
}
template<typename T>
Matrix<T> Matrix<T>::operator/(const T& num) const
{
    Matrix<T> res(this->_lines, this->_columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < this->_columns; ++j)
            res[i][j] = this->matrix[i][j] / num;
	return res;
}
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& sec) const
{
	if (this->_columns != sec._lines)
        throw "Multiplication of incompatible Matrix<T>es";
    Matrix<T> res(this->_lines, sec._columns);
	for (int i = 0; i < this->_lines; ++i)
		for (int j = 0; j < sec._columns; ++j)
		{
            T sum = 0;
			for (int r = 0; r < this->_columns; ++r)
                sum = sum + this->matrix[i][r] * sec[r][j];
			res[i][j] = std::move(sum);
		}
	return res;
}
template<typename T>
Vector<T> Matrix<T>::operator*(const Vector<T>& sec) const
{
	if (!this->isSquare() || this->size() != sec.size())
        throw "Multiplication of incompatible Matrix<T> and Vector<T>";
    Vector<T> res = sec.null();
	for (int i = 0; i < res.size(); ++i)
	{
		for (int j = 0; j < res.size(); ++j)
		{
            res[i] = res[i] + this->matrix[i][j] * sec[j];
		}
	}
	return res;
}
template<typename T>
Matrix<T> Matrix<T>::null()
{
    Matrix<T> res(this->_lines, this->_columns);
	return res;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& sec)
{
	*this = *this + sec;
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& sec)
{
	*this = *this - sec;
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::operator*=(const T& num)
{
	*this = *this * num;
	return *this;
}
template<typename T>
Matrix<T>& Matrix<T>::operator/=(const T& num)
{
	*this = *this / num;
	return *this;
}
template<typename T>
bool Matrix<T>::isSquare() const
{
    return this->_lines == this->_columns;
}
/*template<typename T>
void Matrix<T>::out() const
{
    for (int i = 0; i < this->lines(); ++i)
    {
        QString to_out = "( ";
        for (int j = 0; j < this->columns(); ++j)
            to_out += this->matrix[i][j]->makeStringOfExpression() + " ; ";
        to_out += " )";
        qDebug() << to_out;
    }
}*/
template<typename T>
Matrix<T> makeUnitMatrix(int size)
{
    return Matrix<T>(size, size, [](int i, int j) {return (i == j ? 1 : 0); });
}
template<typename T>
T det(const Matrix<T>& matrix)
{
    if (!matrix.isSquare())
        throw "Attemt to count determinant of non square Matrix<T>";
    if (matrix.lines() == 1)
        return matrix[0][0];
    if (matrix.lines() == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    T d = 0;
    for (int i = 0; i < matrix.columns(); ++i)
        d = d + (i % 2 == 0 ? 1 : -1) * matrix[i][0] * det(Matrix<T>(matrix.lines() - 1, matrix.columns() - 1, [i, &matrix](int k, int j)->T { return matrix[k + (k >= i ? 1 : 0)][j + 1]; }));
	return d;
}
template<typename T>
Matrix<T> inverse(Matrix<T> matrix)
{
    if (!matrix.isSquare())
        throw "Attempt to inverse non square Matrix<T>";
    Matrix<T> result = makeUnitMatrix<T>(matrix.size());
    int height = matrix.size();
    int width = matrix[0].size();
    auto get_index_of_first_non_zero = [](Vector<T>& vec)->int {
		for (int i = 0; i < vec.size(); ++i)
            if (vec[i] != 0)
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
        T lead_el = matrix[x][x];
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
            T multiplier = -matrix[i][n] / matrix[n][n];
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
            T multiplier = -matrix[i][n] / matrix[n][n];
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
template<typename T>
std::vector<std::vector<T> > gauss(Matrix<T>&& extended_equation_matrix)
{
    int height = extended_equation_matrix.lines();
    int width = extended_equation_matrix.columns();
    auto get_index_of_first_non_zero = [](Vector<T>& vec)->int {
		for (int i = 0; i < vec.size(); ++i)
            if (vec[i] != 0)
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
            //outMatrix<T>(extended_equation_Matrix<T>);

		}
		sort_rows();
	}
	//!!!!!!!!!!!!!!!
    /*for (int i = 0; i < extended_equation_matrix.lines(); ++i)
        for (int j = 0; j < extended_equation_matrix[i].size(); ++j)
            if (abs(extended_equation_matrix[i][j]) < 1e-15)
                extended_equation_matrix[i][j] = 0;*/
	int first_non_zero = height - 1;
    //!ВОТ В ЭТОМ МОМЕНТЕ ИДЕТ СРАВНЕНИЕ С НУЛЕМ. ЕСЛИ используется тип double, то он может быть оочень маленьким тогда, когда он предполагается быть нулем
    //! из-за этого может вылетать. Необходимо при использовании типа double раскомментить верхий коммент (с занулением) либо ввести универсальную шаблонную функцию
    //! для сравнения с нулем
    for (first_non_zero = height - 1; extended_equation_matrix[first_non_zero][width - 2] == 0; --first_non_zero)
        if (extended_equation_matrix[first_non_zero][width - 1] != 0)
            return std::vector<std::vector<T> >();
    auto div_on_lead_element = [](Vector<T>& vec)->void {
        T lead_el = 0;
		for (int i = 0; i < vec.size(); ++i)
		{
            if (lead_el == 0)
                lead_el = vec[i];
            if (lead_el != 0)
                vec[i] = vec[i]/ lead_el;
		}
	};

    for (int i = 0; i < extended_equation_matrix.lines(); ++i)
        div_on_lead_element(extended_equation_matrix[i]);
	int amount_of_variables = width - 1;
	//������ ������ - ������ ���������� ������� ��������, ������ - ������ �������, ������ ��� ���������� - ���������
    std::vector<std::vector<T> > result(amount_of_variables);
	int current_params_index = width - 2;
	for (int i = first_non_zero; i >= 0; --i)
	{
        while (extended_equation_matrix[i][current_params_index] != 1)
			--current_params_index;
		result[current_params_index].resize(amount_of_variables + 1);
		for (int j = width - 1; j > current_params_index; --j)
		{
            result[current_params_index][j] = (j == width - 1 ? 1 : -1) * extended_equation_matrix[i][j];
		}
		for (int j = i - 1; j >= 0; --j)
		{
            extended_equation_matrix[j] = extended_equation_matrix[j] + (extended_equation_matrix[i] * (-extended_equation_matrix[j][current_params_index]));
		}
	}

    for (int i = 0; i < result.size(); ++i)
    {
        if (result[i].size() == 0)
        {
            result[i].resize(amount_of_variables + 1);
            result[i][i] = 1;
        }
    }
	return result;
	
}

template<typename T>
Matrix<T> rotationMatrix2D(const T& angle)
{
    return Matrix<T>({ Vector<T>(cos(angle), -sin(angle)), Vector<T>(sin(angle), cos(angle))});
}



#endif
