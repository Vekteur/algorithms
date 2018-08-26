#include "catch.hpp"

#include <iostream>
#include <vector>
#include <set>

#include "Math/BasicMath.h"
#include "Math/Sieve.h"
#include "Math/MatrixFastExponent.h"
#include "Math/TortoiseAndHare.h"

using namespace std;

TEST_CASE("Math") {
	SECTION("Prime") {
		REQUIRE(isPrime(5));
		REQUIRE(!isPrime(6));
	}
	SECTION("Factors") {
		SECTION("6") {
			vector<int> factors6 = factors(6);
			multiset<int> actual(factors6.begin(), factors6.end());
			multiset<int> expected{ 2, 3 };
			REQUIRE(actual == expected);
		}
		SECTION("9") {
			vector<int> factors9 = factors(9);
			multiset<int> actual(factors9.begin(), factors9.end());
			multiset<int> expected{ 3, 3 };
			REQUIRE(actual == expected);
		}
	}
	SECTION("GCD and LCM") {
		REQUIRE(gcd(36, 48) == 12);
		REQUIRE(lcm(9, 15) == 45);
	}
	SECTION("Fast exponent") {
		REQUIRE(nthPower(2, 10) == 1024);
		REQUIRE(nthPower(3, 5) == 243);
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
		REQUIRE(nthPower(fib, 10)(0, 1) == 55);
		REQUIRE(nthPower(fib, 20)(0, 1) == 6765);
	}
	SECTION("Tortoise and hare") {
		vector<int> next{ 1, 2, 3, 4, 5, 3 };
		int tail, cycle;
		tie(tail, cycle) = tortoiseAndHare(1, [&next](int x) { return next[x]; });
		REQUIRE(tail == 2);
		REQUIRE(cycle == 3);
	}
}