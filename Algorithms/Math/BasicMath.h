#pragma once

#include "Constants.h"

namespace {
int gcd(int a, int b) {
	return (b == 0) ? a : gcd(b, a % b);
}

int lcm(int a, int b) {
	return a * (b / gcd(a, b));
}

int safeMod(int x, int m) {
	return (x % m + m) % m;
}

int fact(int n) {
	int prod = 1;
	for (int k = 2; k <= n; ++k)
		prod *= k;
	return prod;
}

int modPow(int b, int n, int m = INF) {
	if (n == 0) return 1;
	long long p = modPow(b, n / 2, m);
	p = (p * p) % m;
	if ((n & 1) == 1) p = (b * p) % m;
	return int(p);
}
}
