#pragma once

#include <vector>
#include "Constants.h"

namespace {
int longestIncreasingSubsequence(const std::vector<int>& a) {
	int n = a.size();
	std::vector<int> last(n, INF);
	for (int i = 0; i < n; ++i) {
		// Invariant : for 0 <= j < i, last[j] is the smallest last element
		// of a subsequence of a[0, i[ of length j + 1
		*std::lower_bound(last.begin(), last.end(), a[i]) = a[i];
	}
	return std::lower_bound(last.begin(), last.end(), INF) - last.begin();
}
}