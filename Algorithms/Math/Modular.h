#pragma once

#include <vector>
#include <tuple>

#include "BasicMath.h"
#include "Prime.h"

using ll = long long;

int modPow(int b, int n, int m) {
	if (n == 0) return 1;
	ll p = modPow(b, n / 2, m);
	p = (p * p) % m;
	if ((n & 1) == 1) p = (b * p) % m;
	return int(p);
}

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

Rem chinese(std::vector<Rem> eqs) {
	Rem a{ 0, 1 };
	for (Rem e : eqs) {
		EuclidSol eucl = extendedEuclid(a.m, e.m);
		e.r %= e.m;
		e.m /= eucl.gcd;
		if ((e.r - a.r) % eucl.gcd != 0)
			return { 0, 0 };
		a.r += safeMod((e.r - a.r) / eucl.gcd * eucl.x, e.m) * a.m;
		a.m *= e.m;
	}
	return a;
}
