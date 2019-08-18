#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace {
std::vector<int> zAlgorithmBuild(const std::string& s) {
	int n = int(s.size());
	std::vector<int> z(n, 0);
	z[0] = n;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		// Invariant : s[l : r[ = s[0, r - l[
		if (i < r)
			z[i] = std::min(r - i, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]])
			++z[i];
		if (i + z[i] > r) {
			l = i;
			r = i + z[i];
		}
		// z[i] = max of 0 <= k < n - i such that s[0 : k[ = s[i : i + k[
	}
	return z;
}

std::vector<int> zAlgorithmMatch(const std::string& text, const std::string& pattern) {
	int n = int(text.size()), m = int(pattern.size());
	std::vector<int> matches;
	std::vector<int> z = zAlgorithmBuild(pattern + '$' + text);
	for (int i = 0; i < n - m + 1; ++i) {
		if (z[m + 1 + i] == m)
			matches.push_back(i);
	}
	return matches;
}

int zAlgorithmDistinctSubstrings(const std::string& s) {
	int n = int(s.size());
	int distincts = 0;
	for (int d = 1; d <= n; ++d) {
		std::string pfx = s.substr(0, d);
		std::reverse(pfx.begin(), pfx.end());
		std::vector<int> z = zAlgorithmBuild(pfx);
		int maxZ = 0;
		for (int i = 1; i < d; ++i)
			maxZ = std::max(maxZ, z[i]);
		// Remove the number of times a suffix of the prefix of length d
		// is present in the string of length d - 1
		distincts += d - maxZ;
	}
	return distincts;
}
}