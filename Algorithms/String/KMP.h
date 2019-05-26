#pragma once

#include <vector>
#include <string>
#include <algorithm>

std::vector<int> kmpBuild(const std::string& s) {
	int n = int(s.size());
	std::vector<int> pfx(n, 0);
	for (int i = 1, k = 0; i < n; ++i) {
		// Invariant : pfx[i] <= pfx[i - 1] + 1
		while (k > 0 && s[k] != s[i])
			k = pfx[k - 1];
		if (s[k] == s[i])
			++k;
		pfx[i] = k;
		// pfx[i] = max of 0 <= k < i such that s[0 : k[ = s]i - k : i]
	}
	return pfx;
}

std::vector<int> kmpMatch(const std::string& text, const std::string pattern) {
	int n = int(text.size()), m = int(pattern.size());
	std::vector<int> matches;
	std::vector<int> kmp = kmpBuild(pattern + '$' + text);
	for (int i = 0; i < n - m + 1; ++i) {
		if (kmp[2 * m + i] == m)
			matches.push_back(i);
	}
	return matches;
}

std::vector<int> kmpCountMatchEachPrefix(const std::string& s) {
	int n = int(s.size());
	std::vector<int> countPfx(n + 1);
	std::vector<int> kmp = kmpBuild(s);
	for (int i = 0; i < n; ++i)
		++countPfx[kmp[i]];
	for (int i = n - 1; i >= 1; --i)
		countPfx[kmp[i]] += countPfx[i];
	for (int i = 0; i <= n; ++i)
		++countPfx[i];
	return countPfx;
}

int kmpDistinctSubstrings(const std::string& s) {
	int n = int(s.size());
	int distincts = 0;
	for (int d = 1; d <= n; ++d) {
		std::string pfx = s.substr(0, d);
		std::reverse(pfx.begin(), pfx.end());
		std::vector<int> kmp = kmpBuild(pfx);
		int maxKmp = 0;
		for (int i = 0; i < d; ++i)
			maxKmp = std::max(maxKmp, kmp[i]);
		distincts += d - maxKmp;
	}
	return distincts;
}