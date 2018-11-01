#include "catch.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <bitset>

#include "Math/BasicMath.h"
#include "Math/Sieve.h"
#include "Math/Matrix.h"
#include "Math/TortoiseAndHare.h"
#include "Math/Prime.h"
#include "Math/Modular.h"
#include "Math/Combinatorics.h"
#include "Math/GaussElimination.h"
#include "Math/FastFourier.h"
#include "Math/Simplex.h"

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
	SECTION("Sieve") {
		Sieve sieve{ 2000 };
		REQUIRE(!sieve.isPrime(4));
		REQUIRE(sieve.isPrime(5));
		REQUIRE(!sieve.isPrime(1'000'002));
		REQUIRE(sieve.isPrime(1'000'003));
	}
	SECTION("Modular") {
		REQUIRE(modPow(2, 10) == 1024);
		REQUIRE(modPow(3, 5, 5) == 3);

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
		Rem sol = chineseRemainder(eqs);
		REQUIRE(sol.r == 20);
		REQUIRE(sol.m == 30);
		REQUIRE(sol.m == lcm(6, lcm(3, 5)));
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
	SECTION("Combinatorics") {
		REQUIRE(binomialCoeff(6, 4) == 15);
		REQUIRE(multinomialCoeff(6, { 4, 2 }) == 15);
		REQUIRE(multinomialCoeff(6, { 2, 2, 2 }) == 90);
		REQUIRE(catalan(3) == 5);
		REQUIRE(catalanSlow(3) == 5);
		REQUIRE(derangements(3) == 2);
	}
	SECTION("Gauss elimination") {
		SECTION("double") {
			vector<vector<double>> a{ { 1, 2 }, { 3, 4 } };
			vector<double> b{ 5, 6 };
			vector<double> ans;
			Solutions sol;
			tie(sol, ans) = gauss(a, b);
			REQUIRE(sol == Solutions::ONE);
			vector<double> expected{ -4., 4.5 };
			for (int i = 0; i < int(b.size()); ++i)
				REQUIRE(ans[i] == Approx(expected[i]));
		}
		SECTION("Mod 2") {
			vector<bitset<3>> a{ { 0b110 }, { 0b101 } };
			bitset<2> ans;
			Solutions sol;
			tie(sol, ans) = gaussMod2<2>(a);
			REQUIRE(ans == bitset<2>{ 0b11 });
		}
	}
	SECTION("Determinant") {
		vector<vector<double>> a{ { 1, 2 }, { 3, 4 } };
		REQUIRE(determinant(a) == Approx(-2));
	}
	SECTION("Fast Fourier Transform") {
		vector<int> pol1{ 1, 2, 0, 1 };
		vector<int> pol2{ 3, 1, 4, 1 };
		vector<int> expected{ 3, 7, 6, 12, 3, 4, 1, 0 };
		REQUIRE(multiplyPolynoms(pol1, pol2) == expected);
		REQUIRE(multiplyPolynomsNaive(pol1, pol2) == expected);
	}
	SECTION("Simplex") {
		vector<double> c{ -5, -4, -3, 0, 0, 0 };
		vector<vector<double>> a{
			{1, 0, 0, 1, 0, 0},
			{2, 1, 1, 0, 1, 0},
			{2, 2, 3, 0, 0, 1}
		};
		vector<double> b{ 400, 1000, 1600 };
		Simplex simplex{a, b, c};
		vector<double> expected{ 200, 600, 0, 200, 0, 0 };
		vector<double> sol = simplex.solution();
		REQUIRE(simplex.objective() == -3400);
		for (int i = 0; i < int(sol.size()); ++i)
			REQUIRE(sol[i] == Approx(expected[i]));
	}
}