#pragma once

#include <vector>

bool isPrime(int n) {
	for (int i = 2; i * i <= n; ++i)
		if (n % i == 0)
			return false;
	return true;
}

std::vector<int> factors(int n) {
	std::vector<int> f;
	for (int i = 2; i * i <= n; ++i) {
		while (n % i == 0) {
			f.push_back(i);
			n /= i;
		}
	}
	if (n != 1) f.push_back(n);
	return f;
}

int gcd(int a, int b) {
	return (b == 0) ? a : gcd(b, a % b);
}

int lcm(int a, int b) {
	return a * (b / gcd(a, b));
}