#pragma once

#include <utility>

int tail = 5, cycle = 10;

int f(int u) {
	if (u >= tail)
		return tail + (u - tail + 1) % cycle;
	else
		return u + 1;
}

std::pair<int, int> findCycle(int x0) {
	int t = x0, h = x0, tail = 0, cycle = 0;
	do { t = f(t); h = f(f(h)); } while (t != h);
	h = x0;
	while (t != h) { t = f(t); h = f(h); ++tail; };
	do { t = f(t); ++cycle; } while (t != h);
	return std::make_pair(tail, cycle);
}