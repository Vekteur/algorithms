#pragma once

#include <algorithm>

using namespace std;

namespace {
const double epsilon = 1e-7;

int sgn(int x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

bool between(double x, double a, double b) {
	return min(a, b) <= x && x <= max(a, b);
}

bool eq(double a, double b) { return abs(a - b) <= epsilon; }
bool le(double a, double b) { return a < b + epsilon; }
bool leq(double a, double b) { return a <= b + epsilon; }
}