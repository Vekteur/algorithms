#pragma once

template<int A, int B>
using mat = array<array<int, B>, A>;

template<int A, int B>
ostream& operator<<(ostream& out, mat<A, B>& m) {
	for (int i = 0; i < A; ++i) {
		for (int j = 0; j < B; ++j)
			out << m[i][j] << ' ';
		out << '\n';
	}
	return out;
}

template<int A, int B, int C>
mat<A, C> multMatrices(const mat<A, B>& a, const mat<B, C>& b) {
	mat<A, C> r{};
	for (int i = 0; i < A; ++i)
		for (int j = 0; j < C; ++j)
			for (int k = 0; k < B; ++k)
				r[i][j] += a[i][k] * b[k][j];
	return r;
}

template<int A>
mat<A, A> getIdentity() {
	mat<A, A> r{};
	for (int i = 0; i < A; ++i)
		r[i][i] = 1;
	return r;
}

template<int A>
mat<A, A> nthPowerMatrix(const mat<A, A> b, int n) {
	if (n == 0)
		return getIdentity<A>();

	mat<A, A> power = nthPowerMatrix<A>(b, n / 2);
	power = multMatrices<A, A, A>(power, power);
	if (n & 1 == 1)
		power = multMatrices<A, A, A>(power, b);
	return power;
}