#pragma once

#include <vector>
#include <complex>

using cd = std::complex<double>;

void fft(std::vector<cd> & a, bool invert) {
	int n = a.size();
	
	for (int i = 1, k = 0; i < n; ++i) {
		int bit = n >> 1;
		for (; k & bit; bit >>= 1)
			k ^= bit;
		k ^= bit;

		if (i < k)
			swap(a[i], a[k]);
	}

	for (int len = 2; len <= n; len <<= 1) {
		double ang = 2 * PI / len * (invert ? -1 : 1);
		cd wlen(cos(ang), sin(ang));
		for (int i = 0; i < n; i += len) {
			cd w(1);
			for (int j = 0; j < len / 2; ++j) {
				cd u = a[i + j], v = a[i + j + len / 2] * w;
				a[i + j] = u + v;
				a[i + j + len / 2] = u - v;
				w *= wlen;
			}
		}
	}

	if (invert) {
		for (cd & x : a)
			x /= n;
	}
}

void fftRecursive(std::vector<cd> & a, bool invert) {
	int n = a.size();
	if (n == 1)
		return;

	std::vector<cd> a0(n / 2), a1(n / 2);
	for (int i = 0; 2 * i < n; i++) {
		a0[i] = a[2 * i];
		a1[i] = a[2 * i + 1];
	}
	fftRecursive(a0, invert);
	fftRecursive(a1, invert);

	double ang = 2 * PI / n * (invert ? -1 : 1);
	cd w(1), wn(cos(ang), sin(ang));
	for (int i = 0; 2 * i < n; i++) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert) {
			a[i] /= 2;
			a[i + n / 2] /= 2;
		}
		w *= wn;
	}
}

std::vector<int> multiplyPolynoms(std::vector<int> const& a, std::vector<int> const& b) {
	std::vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
	int n = 1;
	while (n < a.size() + b.size())
		n <<= 1;
	fa.resize(n);
	fb.resize(n);

	fft(fa, false);
	fft(fb, false);
	for (int i = 0; i < n; i++)
		fa[i] *= fb[i];
	fft(fa, true);

	std::vector<int> result(n);
	for (int i = 0; i < n; i++)
		result[i] = round(fa[i].real());
	return result;
}

std::vector<int> multiplyPolynomsNaive(std::vector<int> const& a, std::vector<int> const& b) {
	std::vector<int> res(a.size() + b.size());
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < b.size(); ++j)
			res[i + j] += a[i] * b[j];
	return res;
}