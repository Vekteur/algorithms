#pragma once

#include <vector>
#include <complex>

namespace {
// See http://e-maxx.ru/algo/fft_multiply
void fft(std::vector<std::complex<double>> & a, bool invert) {
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
		std::complex<double> wlen(cos(ang), sin(ang));
		for (int i = 0; i < n; i += len) {
			std::complex<double> w(1);
			for (int j = 0; j < len / 2; ++j) {
				std::complex<double> u = a[i + j], v = a[i + j + len / 2] * w;
				a[i + j] = u + v;
				a[i + j + len / 2] = u - v;
				w *= wlen;
			}
		}
	}

	if (invert) {
		for (auto& x : a)
			x /= n;
	}
}

// See http://e-maxx.ru/algo/fft_multiply
void fftRecursive(std::vector<std::complex<double>> & a, bool invert) {
	int n = a.size();
	if (n == 1)
		return;

	std::vector<std::complex<double>> a0(n / 2), a1(n / 2);
	for (int i = 0; 2 * i < n; i++) {
		a0[i] = a[2 * i];
		a1[i] = a[2 * i + 1];
	}
	fftRecursive(a0, invert);
	fftRecursive(a1, invert);

	double ang = 2 * PI / n * (invert ? -1 : 1);
	std::complex<double> w(1), wn(cos(ang), sin(ang));
	for (int i = 0; 2 * i < n; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert) {
			a[i] /= 2;
			a[i + n / 2] /= 2;
		}
		w *= wn;
	}
}

void dftNaive(std::vector<std::complex<double>>& a, bool invert) {
	int n = int(a.size());
	std::vector<std::complex<double>> dft(n);
	double ang = 2 * PI / n * (invert ? -1 : 1);
	std::complex<double> w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n; ++i) {
		std::complex<double> awn(1); // Evaluate polynom a in w;
		for (int j = 0; j < n; ++j) {
			dft[i] += a[j] * awn;
			awn *= w;
		}
		if (invert)
			dft[i] /= n;
		w *= wn;
	}
	a = dft;
}

// See http://e-maxx.ru/algo/fft_multiply
std::vector<int> multiplyPolynoms(std::vector<int> const& a, std::vector<int> const& b) {
	std::vector<std::complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end());
	int n = 1;
	while (n < int(a.size()) + int(b.size()))
		n <<= 1;
	fa.resize(n);
	fb.resize(n);

	fft(fa, false);
	fft(fb, false);
	for (int i = 0; i < n; i++)
		fa[i] *= fb[i];
	fft(fa, true);

	std::vector<int> result(n);
	for (int i = 0; i < n; ++i)
		result[i] = int(round(fa[i].real()));
	return result;
}

std::vector<int> multiplyPolynomsNaive(std::vector<int> const& a, std::vector<int> const& b) {
	std::vector<int> res(a.size() + b.size());
	for (int i = 0; i < int(a.size()); ++i)
		for (int j = 0; j < int(b.size()); ++j)
			res[i + j] += a[i] * b[j];
	return res;
}
}