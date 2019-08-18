#pragma once

#include <vector>
#include <functional>

template<typename T>
std::vector<std::vector<T>> permutations(const std::vector<T>& a) {
	std::vector<std::vector<T>> currPermutations;
	std::vector<T> currPermutation;
	std::vector<bool> chosen(a.size(), false);
	std::function<void()> permutations_rec = 
		[&permutations_rec, &currPermutations, &currPermutation, &chosen, &a]() {
		if (currPermutation.size() == a.size()) {
			currPermutations.push_back(currPermutation);
			return;
		}
		for (int i = 0; i < int(a.size()); ++i) {
			if (chosen[i])
				continue;
			chosen[i] = true;
			currPermutation.push_back(a[i]);
			permutations_rec();
			chosen[i] = false;
			currPermutation.pop_back();
		}
	};
	permutations_rec();
	return currPermutations;
}