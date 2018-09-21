#pragma once

#include <functional>

namespace {
int binarySearch(int low, int high, std::function<bool(int)> valid) {
	// Return the maximum valid value
	while (low < high) {
		int mid = (low + high + 1) / 2;
		if (valid(mid))
			low = mid;
		else
			high = mid - 1;
	}
	return low;
}

double binarySearch(double low, double high, std::function<bool(double)> valid) {
	const double epsilon = 1e-7;
	while (high - low > epsilon) {
		double mid = (low + high) / 2;
		if (valid(mid))
			low = mid;
		else
			high = mid;
	}
	return low;
}
}