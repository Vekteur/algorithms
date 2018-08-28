#pragma once

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
}
