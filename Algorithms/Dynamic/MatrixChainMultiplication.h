#pragma once

#include <vector>
#include <algorithm>

#include "Constants.h"

namespace {
int matrixChainMultiplication(const std::vector<int>& dims) {
	int n = dims.size();
	std::vector<std::vector<int>> d(n, std::vector<int>(n, 0));
	for (int len = 3; len <= n; ++len) {
		for (int i = 0; i < n - len + 1; ++i) {
			int j = i + len - 1;
			d[i][j] = INF;
			for (int k = i + 1; k <= j - 1; ++k) {
				d[i][j] = std::min(d[i][j], d[i][k] + d[k][j] + dims[i] * dims[k] * dims[j]);
			}
		}
	}
	return d[0][n - 1];
}
}