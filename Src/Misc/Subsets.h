#pragma once

#include <vector>
#include <functional>

template<typename T>
std::vector<std::vector<T>> subsets(const std::vector<T>& a) {
	std::vector<std::vector<T>> currSubsets;
	std::vector<T> currSubset;
	std::function<void(int)> subsets_rec = [&subsets_rec, &a, &currSubsets, &currSubset](int k) {
		if (k == a.size()) {
			currSubsets.push_back(currSubset);
			return;
		}
		subsets_rec(k + 1);
		currSubset.push_back(a[k]);
		subsets_rec(k + 1);
		currSubset.pop_back();
	};
	subsets_rec(0);
	return currSubsets;
}

template<typename T>
std::vector<std::vector<T>> subsets2(const std::vector<T>& a) {
	std::vector<std::vector<T>> currSubsets;
	currSubsets.push_back({});
	for (int i = 0; i < int(a.size()); ++i) {
		int s = int(currSubsets.size());
		for (int j = 0; j < s; ++j) {
			std::vector<T> subset = currSubsets[j];
			subset.push_back(a[i]);
			currSubsets.push_back(std::move(subset));
		}
	}
	return currSubsets;
}

template<typename T>
std::vector<std::vector<T>> subsets3(const std::vector<T>& a) {
	std::vector<std::vector<T>> currSubsets;
	for (int b = 0; b < int(1 << a.size()); ++b) {
		std::vector<T> subset;
		for (int i = 0; i < int(a.size()); ++i) {
			if (b & (1 << i))
				subset.push_back(a[i]);
		}
		currSubsets.push_back(std::move(subset));
	}
	return currSubsets;
}