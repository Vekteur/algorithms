#pragma once

#include <functional>

namespace {
// By default, the function must be strictly increasing then stricty decreasing
int ternarySearch(int low, int high, std::function<int(int)> f,
	std::function<bool(int, int)> comp = std::less<int>{}) {

	while (high - low >= 3) {
		double mid1 = low + (high - low) / 3;
		double mid2 = high - (high - low) / 3;
		if (comp(f(mid1), f(mid2)))
			low = mid1;
		else
			high = mid2;
	}
	int minI = low;
	for (int i = low + 1; i <= high; ++i) {
		if (comp(f(minI), f(i)))
			minI = i;
	}
	return minI;
}

double ternarySearch(double low, double high, std::function<double(double)> f,
	std::function<bool(double, double)> comp = std::less<double>{}) {

	const double epsilon = 1e-7;
	while (high - low > epsilon) {
		double mid1 = low + (high - low) / 3;
		double mid2 = high - (high - low) / 3;
		if (comp(f(mid1), f(mid2)))
			low = mid1;
		else
			high = mid2;
	}
	return low;
}
}