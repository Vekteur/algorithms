#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Sorting/BinarySearch.h"
#include "DataStructure/SparseTable.h"

// See http://e-maxx.ru/algo/suffix_array
struct SuffixArray {
	int n;
	std::string text;
	std::vector<int> logs;
	std::vector<int> lcp;
	// p = permutation such that p[i] = index of ith smallest element in s
	std::vector<int> p;
	// c = sorted equivalence classes of indices in s
	std::vector<std::vector<int>> c;
	std::vector<int> pInv;
	SparseTable<int> st;

	std::tuple<std::vector<int>, std::vector<std::vector<int>>>
		sortCyclicShifts(const std::string& s, int maxLog, char minChar, int nbChars) {
		int n = int(s.size());

		std::vector<int> p(n);
		{
			std::vector<int> cnt(nbChars, 0);
			for (int i = 0; i < n; ++i)
				++cnt[s[i] - minChar];
			for (int i = 1; i < nbChars; ++i)
				cnt[i] += cnt[i - 1];
			for (int i = n - 1; i >= 0; --i)
				p[--cnt[s[i] - minChar]] = i;
		}

		int classesNb;
		std::vector<std::vector<int>> c(maxLog + 1, std::vector<int>(n));
		{
			c[0][p[0]] = 0;
			int classes = 0;
			for (int i = 1; i < n; ++i) {
				if (s[p[i]] != s[p[i - 1]])
					++classes;
				c[0][p[i]] = classes;
			}
			classesNb = classes + 1;
		}

		for (int h = 0; ; ++h) {
			int half = 1 << h;
			// Sort by first half with stable counting sort
			std::vector<int> cnt(classesNb, 0);
			for (int i = 0; i < n; ++i)
				++cnt[c[h][i]];
			for (int i = 1; i < classesNb; ++i)
				cnt[i] += cnt[i - 1];
			std::vector<int> pCopy = p;
			for (int i = n - 1; i >= 0; --i) {
				int firstHalf = pCopy[i] - half;
				if (firstHalf < 0)
					firstHalf += n;
				p[--cnt[c[h][firstHalf]]] = firstHalf;
			}

			if (2 * half >= n)
				break;

			std::vector<int> cn(n); // Next sorted equivalence classes
			int classes = 0;
			cn[p[0]] = classes;
			for (int i = 1; i < n; ++i) {
				// Next classes are equal if the first and second half are equal
				int curr = p[i], prev = p[i - 1];
				int secHalfCurr = (curr + half) % n, secHalfPrev = (prev + half) % n;
				if (c[h][curr] != c[h][prev] || c[h][secHalfCurr] != c[h][secHalfPrev])
					++classes;
				cn[curr] = classes;
			}
			classesNb = classes + 1;
			c[h + 1] = std::move(cn);
		}
		return { p, c };
	}

	std::vector<int> buildLcp(const std::string& s, const std::vector<int>& p) {
		int n = int(s.size());
		std::vector<int> lcp(n - 1, 0);
		for (int i = 0, k = 0; i < n; ++i) {
			if (pInv[i] == n - 1) {
				k = 0;
				continue;
			}
			int j = p[pInv[i] + 1];
			if (k != 0)
				--k;
			while (i + k < n && j + k < n && s[i + k] == s[j + k])
				++k;
			lcp[pInv[i]] = k;
		}
		return lcp;
	}

	SuffixArray(const std::string& text, char minChar = 'a', char maxChar = 'z')
		: text(text), n(int(text.size())), logs(n + 1), st({}) {

		logs[0] = -1;
		for (int k = 1; k <= n; ++k)
			logs[k] = logs[k / 2] + 1;

		int maxLog = logs[n];
		std::tie(p, c) = sortCyclicShifts(
			text + char(minChar - 1), maxLog, minChar - 1, maxChar - minChar + 2);
		p.erase(p.begin());
		for (int i = 0; i <= maxLog; ++i)
			c[i].pop_back();

		pInv.resize(p.size());
		for (int i = 0; i < n; ++i)
			pInv[p[i]] = i;

		lcp = buildLcp(text, p);
		st = SparseTable<int>(lcp);
	}

	std::vector<int> match(const std::string& pattern) {
		int m = int(pattern.size());
		std::vector<int> matches;
		// If the pattern is less than the smallest string, it must return -1
		int start = binarySearch(-1, n - 1, [this, &pattern, &m](int i) {
			return i != -1 && text.substr(p[i], std::min(m, n - p[i])) < pattern;
			}) + 1;
		int end = binarySearch(-1, n - 1, [this, &pattern, &m](int i) {
			return i != -1 && text.substr(p[i], std::min(m, n - p[i])) <= pattern;
			});
		for (int i = start; i <= end; ++i)
			matches.push_back(p[i]);
		return matches;
	}

	int compare(int i1, int i2, int l) {
		int d = logs[l];
		std::tuple<int, int> s1 = { c[d][i1], c[d][i1 + l - (1 << d)] };
		std::tuple<int, int> s2 = { c[d][i2], c[d][i2 + l - (1 << d)] };
		if (s1 == s2)
			return 0;
		else
			return s1 < s2 ? -1 : 1;
	}

	int longestCommonPrefix(int i1, int i2) {
		if (i1 == i2)
			return n - i1;
		i1 = pInv[i1], i2 = pInv[i2];
		if (i1 > i2)
			std::swap(i1, i2);
		return st.query(i1, i2 - 1);
	}

	int distinctSubstrings() {
		int res = (n * (n + 1)) / 2;
		for (int diffPfx : lcp)
			res -= diffPfx;
		return res;
	}
};

std::string longestCommonSubstringSuffixArray(const std::string& s1,
		const std::string& s2, char minChar = 'a', char maxChar = 'z') {
	int n = s1.size(), m = s2.size();
	std::string s = s1 + char(minChar - 1) + s2;
	SuffixArray sa(s, minChar, maxChar);
	int maxLcsLength = -1;
	int maxLcsIndex = -1;
	for (int i = 1; i < int(s.size()) - 1; ++i) {
		int i1 = sa.p[i], i2 = sa.p[i + 1];
		if (i1 > i2)
			std::swap(i1, i2);
		if (i1 < n && i2 > n) {
			int l = sa.lcp[i];
			if (l > maxLcsLength) {
				maxLcsLength = l;
				maxLcsIndex = i1;
			}
		}
	}
	return s1.substr(maxLcsIndex, maxLcsLength);
}