#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Sorting/BinarySearch.h"

struct SuffixArray {
	int n;
	std::string text;
	std::vector<int> sa;

	// See http://e-maxx.ru/algo/suffix_array
	std::vector<int> sortCyclicShifts(const std::string& s, char minChar = 'a', int nbChars = 26) {
		int n = int(s.size());
		// p = permutation such that p[i] = index of ith smallest element in s
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
		// c = sorted equivalence classes of indices in s
		std::vector<int> c(n);
		int classesNb;
		{
			c[p[0]] = 0;
			int classes = 0;
			for (int i = 1; i < n; ++i) {
				if (s[p[i]] != s[p[i - 1]])
					++classes;
				c[p[i]] = classes;
			}
			classesNb = classes + 1;
		}

		for (int h = 0; (1 << h) < n; ++h) {
			// p = permutation such that p[i] = index of ith smallest element in c
			std::vector<int> pn(n);
			// pn = permutation of first half
			for (int i = 0; i < n; ++i) {
				pn[i] = p[i] - (1 << h);
				if (pn[i] < 0)
					pn[i] += n;
			}

			// Sort first half with stable counting sort
			std::vector<int> cnt(classesNb, 0);
			for (int i = 0; i < n; ++i)
				++cnt[c[i]];
			for (int i = 1; i < classesNb; ++i)
				cnt[i] += cnt[i - 1];
			for (int i = n - 1; i >= 0; --i)
				p[--cnt[c[pn[i]]]] = pn[i];

			std::vector<int> cn(n); // Next sorted equivalence classes
			int classes = 0;
			cn[p[0]] = classes;
			for (int i = 1; i < n; ++i) {
				// Next classes are equal if the first and second half are equal
				int mid1 = (p[i] + (1 << h)) % n, mid2 = (p[i - 1] + (1 << h)) % n;
				if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2])
					++classes;
				cn[p[i]] = classes;
			}
			classesNb = classes + 1;
			c.swap(cn);
		}
		return p;
	}

	SuffixArray(std::string s, char minChar = 'a', int nbChars = 26) : text(s) {
		n = int(s.size());
		s += minChar - 1;
		sa = sortCyclicShifts(s, minChar - 1, nbChars + 1);
		sa.erase(sa.begin());
	}

	std::vector<int> match(const std::string& pattern) {
		int m = int(pattern.size());
		std::vector<int> matches;
		// If the pattern is less than the smallest string, it must return -1
		int start = binarySearch(-1, n - 1, [this, &pattern, &m](int i) {
			return i != -1 && text.substr(sa[i], std::min(m, n - sa[i])) < pattern;
		}) + 1;
		int end = binarySearch(-1, n - 1, [this, &pattern, &m](int i) {
			return i != -1 && text.substr(sa[i], std::min(m, n - sa[i])) <= pattern;
		});
		for (int i = start; i <= end; ++i)
			matches.push_back(sa[i]);
		return matches;
	}
};