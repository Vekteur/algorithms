#pragma once

#include <algorithm>

using namespace std;

const double pi{ atan(1) * 4 };
const double epsilon = 1e-7;

int sgn(int x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

bool eq(double a, double b) { return abs(a - b) <= epsilon; }
bool le(double a, double b) { return a < b + epsilon; }
bool leq(double a, double b) { return a <= b + epsilon; }
