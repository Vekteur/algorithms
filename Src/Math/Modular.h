#pragma once

#include <vector>
#include <tuple>

#include "BasicMath.h"
#include "Factorization.h"

namespace {
struct EuclidSol { int x, y, gcd; };

EuclidSol extendedEuclid(int a, int b) {
	if (b == 0) return { 1, 0, a };
	EuclidSol e = extendedEuclid(b, a % b);
	return { e.y, e.x - (a / b) * e.y, e.gcd };
}

int modInverse(int x, int m) {
	return extendedEuclid(x, m).x;
}

int modInverse2(int x, int m) {
	return modPow(x, eulerTotient(m) - 1, m);
}

EuclidSol diaphantineEq(int a, int b, int c) {
	EuclidSol eucl = extendedEuclid(a, b);
	int ratio = c / eucl.gcd;
	return { eucl.x * ratio, eucl.y * ratio, eucl.gcd };
}

struct Rem { int r, m; };

Rem chineseRemainder(std::vector<Rem> eqs) {
	Rem sol{ 0, 1 };
	for (Rem eq : eqs) {
		EuclidSol eucl = extendedEuclid(sol.m, eq.m);
		eq.m /= eucl.gcd; // Modulos does not have to be coprime 
		if ((eq.r - sol.r) % eucl.gcd != 0) // No solution
			return { -1, -1 };
		sol.r += safeMod((eq.r - sol.r) / eucl.gcd * eucl.x, eq.m) * sol.m;
		sol.m *= eq.m; // sol.m is the interval between solutions
	}
	return sol;
}
}