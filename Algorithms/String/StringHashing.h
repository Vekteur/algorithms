#pragma once

#include <vector>
#include <string>
#include <algorithm>

std::vector<int> modPowers(int n, int m, int p) {
	std::vector<int> pows(n);
	long long pow = 1;
	for (int i = 0; i < n; ++i) {
		pows[i] = int(pow);
		pow = (pow * p) % m;
	}
	return pows;
}

// minChar is the lowest character of the string
// p is a prime factor that must be greater than maxChar - minChar + 1
long long hashString(std::string const& s, int m, int p = 31, char minChar = 'a') {
	int n = int(s.size());
	long long hash = 0;
	std::vector<int> pows = modPowers(n, m, p);
	for (int i = 0; i < n; ++i)
		hash = (hash + (s[i] - minChar + 1) * long long(pows[i])) % m;
	return hash;
}

std::vector<int> rabinKarp(const std::string& text, const std::string& pattern,
	int m, int p = 31, char minChar = 'a') {

	int n = int(text.size()), l = int(pattern.size());
	std::vector<int> pows = modPowers(n, m, p);

	long long hashPattern = hashString(pattern, m, p, minChar);
	std::vector<long long> hashsText(n);
	for (int i = 0; i < n; ++i)
		hashsText[i] = ((i == 0 ? 0 : hashsText[i - 1]) + (text[i] - minChar + 1) * long long(pows[i])) % m;
	std::vector<int> matches;
	for (int i = 0; i + l - 1 < n; ++i) {
		long long hash = (hashsText[i + l - 1] + m - (i == 0 ? 0 : hashsText[i - 1])) % m;
		if (hash == (hashPattern * pows[i]) % m)
			matches.push_back(i);
	}
	return matches;
}