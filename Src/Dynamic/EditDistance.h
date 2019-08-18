#pragma once

#include <vector>
#include <algorithm>

namespace {
int editDistance(const std::vector<int>& a, const std::vector<int>& b) {
	int n = a.size(), m = b.size();
	std::vector<std::vector<int>> d(n + 1, std::vector<int>(m + 1));
	for (int i = 0; i <= n; ++i)
		d[i][0] = i;
	for (int j = 0; j <= m; ++j)
		d[0][j] = j;
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			d[i][j] = std::min({d[i][j - 1] + 1, d[i - 1][j] + 1, 
				d[i - 1][j - 1] + (a[i - 1] != b[j - 1]) });
		}
	}
	return d[n][m];
}
}