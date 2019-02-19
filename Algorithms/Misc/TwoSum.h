#pragma once

#include <vector>

namespace {
bool twoSum(const std::vector<int>& a, int sum) {
	int n = a.size();
	int i = 0, j = n - 1;
	while (i <= j) {
		int x = a[i] + a[j];
		if (x == sum) return true;
		else if (x < sum) ++i;
		else --j;
	}
	return false;
}
}