#pragma once

#include <vector>
#include <algorithm>

namespace {
int longestCommonSubsequence(const std::vector<int>& a, const std::vector<int>& b) {
	int n = a.size(), m = b.size();
	std::vector<std::vector<int>> d(n + 1, std::vector<int>(m + 1, 0));
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1] + 1;
			else
				d[i][j] = std::max(d[i][j - 1], d[i - 1][j]);
		}
	}
	return d[n][m];
}

int longestRepeatedSubsequence(const std::vector<int>& a) {
	int n = a.size();
	std::vector<std::vector<int>> d(n + 1, std::vector<int>(n + 1, 0));
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (a[i - 1] == a[j - 1] && i != j)
				d[i][j] = d[i - 1][j - 1] + 1;
			else
				d[i][j] = std::max(d[i][j - 1], d[i - 1][j]);
		}
	}
	return d[n][n];
}
}