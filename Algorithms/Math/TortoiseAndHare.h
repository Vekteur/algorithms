#pragma once

#include <tuple>
#include <functional>

std::tuple<int, int> tortoiseAndHare(int x0, std::function<int(int)> f) {
	int t = x0, h = x0, tail = 0, cycle = 0;
	do { t = f(t); h = f(f(h)); } while (t != h);
	h = x0;
	while (t != h) { t = f(t); h = f(h); ++tail; };
	do { t = f(t); ++cycle; } while (t != h);
	return { tail, cycle };
}