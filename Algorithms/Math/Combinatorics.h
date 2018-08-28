#pragma once

#include <vector>

namespace {
	int binomialCoeff(int n, int k) {
		int prod = 1;
		// The division is always accurate because prod
		// has always already been multiplied by a multiple of i
		for (int i = 1; i <= k; ++i)
			prod = (prod * (n - k + i)) / i;
		return prod;
	}

	// Requires the sum of elements of ks to be n
	int multinomialCoeff(int n, std::vector<int> ks) {
		int prod = 1;
		int k = 1;
		int ki = 1;
		for (int i = ks[0] + 1; i <= n; ++i) {
			if (ki > ks[k]) {
				++k; ki = 1;
			}
			prod = (prod * i) / ki;
			++ki;
		}
		return prod;
	}

	int catalan(int n) {
		return binomialCoeff(2 * n, n) / (n + 1);
	}

	int catalanSlow(int n) {
		if (n == 0) return 1;
		int sum = 0;
		for (int i = 0; i < n; ++i)
			sum += catalanSlow(i) * catalanSlow(n - i - 1);
		return sum;
	}

	int derangements(int n) {
		std::vector<int> d(n + 1);
		d[1] = 0;
		d[2] = 1;
		for (int i = 3; i <= n; ++i)
			d[i] = (i - 1) * (d[i - 2] + d[i - 1]);
		return d[n];
	}
}