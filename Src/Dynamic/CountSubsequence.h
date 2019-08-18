#pragma once

#include <string>
#include <vector>

namespace {
int countSubsequence(const std::vector<int>& text, const std::vector<int>& pattern) {
	int n = text.size(), m = pattern.size();
	std::vector<std::vector<int>> d(n + 1, std::vector<int>(m + 1, 0));
	for (int i = 0; i <= n; ++i)
		d[i][0] = 1;
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			d[i][j] = d[i - 1][j];
			if (text[i - 1] == pattern[j - 1])
				d[i][j] += d[i - 1][j - 1];
		}
	}
	return d[n][m];
}
}