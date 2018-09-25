#pragma once

#include "Mat.h"

namespace {
Mat<int> mult(const Mat<int>& a, const Mat<int>& b) {
	Mat<int> r{ a.size() };
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < a.size(); ++j)
			for (int k = 0; k < a.size(); ++k)
				r(i, j) += a(i, k) * b(k, j);
	return r;
}

Mat<int> identityMatrix(int n) {
	Mat<int> r{ n };
	for (int i = 0; i < n; ++i)
		r(i, i) = 1;
	return r;
}

Mat<int> power(const Mat<int>& b, int n) {
	if (n == 0)
		return identityMatrix(b.size());
	Mat<int> p = power(b, n / 2);
	p = mult(p, p);
	if (n & 1)
		p = mult(p, b);
	return p;
}
}