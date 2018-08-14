#pragma once

using ll = long long;

#include <vector>

class Sieve {
public:
	std::vector<int> primes;
	std::vector<char> primeArr;
	int N;

	Sieve(int _n) : N{ _n }, primeArr(_n, true)
	{
		for (ll i = 2; i < N; ++i) {
			if (primeArr[i]) {
				for (ll j = i * i; j < N; j += i)
					primeArr[j] = false;
				primes.push_back((int)i);
			}
		}
	}

	bool isPrime(int n) {
		if (n < N)
			return primeArr[n];
		for (int i = 0; primes[i] * primes[i] <= n; ++i)
			if (n % primes[i] == 0)
				return false;
		return true;
	}
};