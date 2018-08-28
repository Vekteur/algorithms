#pragma once

#include <vector>
#include <tuple>
#include <cmath>
#include <cmath>

namespace {
	bool isPrime(int n) {
		for (int i = 2; i * i <= n; ++i)
			if (n % i == 0)
				return false;
		return true;
	}

	struct Factor { int value, count; };

	std::vector<Factor> factors(int n) {
		std::vector<Factor> f;
		for (int i = 2; i * i <= n; ++i) {
			int count = 0;
			while (n % i == 0) {
				++count;
				n /= i;
			}
			if (count != 0) f.push_back({ i, count });
		}
		if (n != 1) f.push_back({ n, 1 });
		return f;
	}

	int numberOfFactors(int n) {
		int nb = 1;
		for (Factor f : factors(n))
			nb *= f.count + 1;
		return nb;
	}

	int sumOfFactors(int n) {
		int prod = 1;
		for (Factor f : factors(n))
			prod *= (pow(f.value, f.count + 1) - 1) / (f.value - 1);
		return prod;
	}

	int productOfFactors(int n) {
		return pow(n, numberOfFactors(n) / 2);
	}

	int eulerTotient(int n) {
		int prod = 1;
		for (Factor f : factors(n))
			prod *= pow(f.value, f.count - 1) * (f.value - 1);
		return prod;
	}
}