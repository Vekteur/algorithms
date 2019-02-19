#pragma once

#include "Matrix.h"

namespace {
Matrix<int> operator+(const Matrix<int>& a, const Matrix<int>& b) {
	Matrix<int> r{ a.size() };
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < a.size(); ++j)
			r(i, j) = a(i, j) + b(i, j);
	return r;
}

Matrix<int> operator-(const Matrix<int>& a) {
	Matrix<int> r{ a.size() };
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < a.size(); ++j)
			r(i, j) = -a(i, j);
	return r;
}

Matrix<int> operator-(const Matrix<int>& a, const Matrix<int>& b) {
	return a + -b;
}

bool operator==(const Matrix<int>& a, const Matrix<int>& b) {
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < a.size(); ++j)
			if (a(i, j) != b(i, j))
				return false;
	return true;
}

Matrix<int> operator*(const Matrix<int>& a, const Matrix<int>& b) {
	Matrix<int> r{ a.size() };
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < a.size(); ++j)
			for (int k = 0; k < a.size(); ++k)
				r(i, j) += a(i, k) * b(k, j);
	return r;
}

Matrix<int> identityMatrix(int n) {
	Matrix<int> r{ n };
	for (int i = 0; i < n; ++i)
		r(i, i) = 1;
	return r;
}

Matrix<int> power(const Matrix<int>& b, int n) {
	if (n == 0)
		return identityMatrix(b.size());
	Matrix<int> p = power(b, n / 2);
	p = p * p;
	if (n & 1)
		p = p * b;
	return p;
}
}