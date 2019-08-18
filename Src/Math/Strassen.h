#pragma once

#include "Math/Matrix.h"
#include "Math/MatrixPower.h"

namespace {
Matrix<int> strassen(const Matrix<int>& a, const Matrix<int>& b) {
	int n = a.size();
	if (n == 1) {
		Matrix<int> r(n);
		r(0, 0) = a(0, 0) * b(0, 0);
		return r;
	}

	int m = n / 2;
	auto splitMatrix = [&m](const Matrix<int> matrix) {
		Matrix<Matrix<int>> r(2, Matrix<int>(m));
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int i2 = 0; i2 < m; ++i2)
					for (int j2 = 0; j2 < m; ++j2)
						r(i, j)(i2, j2) = matrix(i * m + i2, j * m + j2);
		return r;
	};
	auto as = splitMatrix(a), bs = splitMatrix(b);

	auto m1 = strassen(as(0, 0) + as(1, 1), bs(0, 0) + bs(1, 1));
	auto m2 = strassen(as(1, 0) + as(1, 1), bs(0, 0));
	auto m3 = strassen(as(0, 0), bs(0, 1) - bs(1, 1));
	auto m4 = strassen(as(1, 1), bs(1, 0) - bs(0, 0));
	auto m5 = strassen(as(0, 0) + as(0, 1), bs(1, 1));
	auto m6 = strassen(as(1, 0) - as(0, 0), bs(0, 0) + bs(0, 1));
	auto m7 = strassen(as(0, 1) - as(1, 1), bs(1, 0) + bs(1, 1));

	Matrix<Matrix<int>> cs(2);
	cs(0, 0) = m1 + m4 - m5 + m7;
	cs(0, 1) = m3 + m5;
	cs(1, 0) = m2 + m4;
	cs(1, 1) = m1 - m2 + m3 + m6;

	auto mergeMatrices = [&n, &m](Matrix<Matrix<int>> matrices) {
		Matrix<int> r(n);
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int i2 = 0; i2 < m; ++i2)
					for (int j2 = 0; j2 < m; ++j2)
						r(i * m + i2, j * m + j2) = matrices(i, j)(i2, j2);
		return r;
	};
	return mergeMatrices(cs);
}
}