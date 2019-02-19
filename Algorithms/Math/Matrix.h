#pragma once

#include <iostream>

template<typename T>
class Matrix {
public:
	Matrix(int n = 0, T defaultValue = T())
		: n{ n }, vec(n * n, defaultValue)
	{ }

	T& operator ()(int row, int col) {
		return vec[n * row + col];
	}

	const T& operator ()(int row, int col) const {
		return vec[n * row + col];
	}

	int size() const {
		return n;
	}

	Matrix<T> operator+(const Matrix<T>& m) const {
		Matrix<T> r(size());
		for (int i = 0; i < size(); ++i)
			for (int j = 0; j < size(); ++j)
				r(i, j) = (*this)(i, j) + m(i, j);
		return r;
	}

	Matrix<T> operator-() const {
		Matrix<T> r(size());
		for (int i = 0; i < size(); ++i)
			for (int j = 0; j < size(); ++j)
				r(i, j) = -(*this)(i, j);
		return r;
	}

	Matrix<T> operator-(const Matrix<T>& m) const {
		return (*this) + -m;
	}

	bool operator==(const Matrix<T>& m) const {
		for (int i = 0; i < size(); ++i)
			for (int j = 0; j < size(); ++j)
				if ((*this)(i, j) != m(i, j))
					return false;
		return true;
	}

	Matrix<T> operator*(const Matrix<T>& m) const {
		Matrix<T> r(size());
		for (int i = 0; i < size(); ++i)
			for (int j = 0; j < size(); ++j)
				for (int k = 0; k < size(); ++k)
					r(i, j) += (*this)(i, k) * m(k, j);
		return r;
	}

private:
	int n;
	std::vector<T> vec;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat) {
	for (int i = 0; i < mat.size(); ++i) {
		for (int j = 0; j < mat.size(); ++j)
			out << mat(i, j) << ' ';
		out << '\n';
	}
	return out;
}