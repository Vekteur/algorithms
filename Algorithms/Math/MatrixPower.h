#pragma once

#include "Matrix.h"

namespace {
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