#pragma once

using ll = long long;

#include <vector>

class Sieve {
public:
	std::vector<int> primes; // Primes in [0, n[
	std::vector<char> isPrimeArr;

	Sieve(int n) : isPrimeArr(n, true) {
		isPrimeArr[0] = false;
		for (int i = 0; i < 2; ++i)
			isPrimeArr[i] = false;
		for (int i = 2; i < isPrimeArr.size(); ++i) {
			if (isPrimeArr[i]) {
				// We can start iteration at i * i
				// because if a factor of i is less than i, it is already marked
				for (ll j = i * i; j < isPrimeArr.size(); j += i)
					isPrimeArr[j] = false;
				primes.push_back(i);
			}
		}
	}

	// k must be in [0, n * n[
	bool isPrime(int k) {
		if (k < isPrimeArr.size())
			return isPrimeArr[k];
		for (int i = 0; primes[i] * primes[i] <= k; ++i)
			if (k % primes[i] == 0)
				return false;
		return true;
	}
};