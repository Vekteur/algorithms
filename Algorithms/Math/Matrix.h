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