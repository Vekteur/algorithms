#include "catch.hpp"

#include <iostream>
#include <vector>
#include <set>

#include "Math/BasicMath.h"
#include "Math/Sieve.h"
#include "Math/MatrixFastExponent.h"
#include "Math/TortoiseAndHare.h"
#include "Math/Prime.h"
#include "Math/Modular.h"

using namespace std;

TEST_CASE("Math") {
	SECTION("GCD and LCM") {
		REQUIRE(gcd(36, 48) == 12);
		REQUIRE(lcm(9, 15) == 45);
	}
	SECTION("Prime") {
		REQUIRE(isPrime(11));
		REQUIRE(!isPrime(12));
		vector<Factor> expected{ {2, 2}, {3, 1} };
		vector<Factor> factors12 = factors(12);
		bool eq = equal(factors12.begin(), factors12.end(), expected.begin(),
			[](Factor f1, Factor f2) { return f1.value == f2.value && f1.count == f2.count; });
		REQUIRE(eq);

		REQUIRE(numberOfFactors(12) == 6);
		REQUIRE(sumOfFactors(12) == 1 + 2 + 3 + 4 + 6 + 12);
		REQUIRE(productOfFactors(12) == 1 * 2 * 3 * 4 * 6 * 12);
		REQUIRE(eulerTotient(12) == 4);
	}
	SECTION("Modular") {
		REQUIRE(modPow(2, 10, 1e9) == 1024);
		REQUIRE(modPow(3, 5, 1e9) == 243);

		// ax + by == gcd(a, b)
		// Sols : { x + kb / gcd(a, b), y - ka / gcd(a, b) }
		// 39 * 2 + 15 * (-5) == 3
		EuclidSol euclSol = extendedEuclid(39, 15);
		REQUIRE(euclSol.x == 2);
		REQUIRE(euclSol.y == -5);
		REQUIRE(euclSol.gcd == 3);

		// (6 * 3) % 17 == 1
		REQUIRE(modInverse(6, 17) == 3);
		REQUIRE(modInverse2(6, 17) == 3);

		// 4 * 6 + 7 * (-3) == 3
		EuclidSol diophSol = diaphantineEq(4, 7, 3);
		REQUIRE(diophSol.x == 6);
		REQUIRE(diophSol.y == -3);

		// x % 6 == 2
		// x % 3 == 2
		// x % 5 == 0
		// => x == 20
		// Sols : { x + km }
		vector<Rem> eqs{ { 2, 6 }, { 2, 3 }, { 0, 5 } };
		Rem sol = chinese(eqs);
		REQUIRE(sol.r == 20);
		REQUIRE(sol.m == 30);
		REQUIRE(sol.m == lcm(6, lcm(3, 5)));
	}
	SECTION("Sieve") {
		Sieve sieve{ 2000 };
		REQUIRE(!sieve.isPrime(4));
		REQUIRE(sieve.isPrime(5));
		REQUIRE(!sieve.isPrime(1'000'002));
		REQUIRE(sieve.isPrime(1'000'003));
	}
	SECTION("Matrix") {
		Mat<int> fib{ 2, 1 };
		fib(0, 0) = 0;
		REQUIRE(power(fib, 10)(0, 1) == 55);
		REQUIRE(power(fib, 20)(0, 1) == 6765);
	}
	SECTION("Tortoise and hare") {
		vector<int> next{ 1, 2, 3, 4, 5, 3 };
		int tail, cycle;
		tie(tail, cycle) = tortoiseAndHare(1, [&next](int x) { return next[x]; });
		REQUIRE(tail == 2);
		REQUIRE(cycle == 3);
	}
}