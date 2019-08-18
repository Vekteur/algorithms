#pragma once

#include <vector>
#include <functional>
#include <random>

namespace {
void quicksort(std::vector<int>& a) {
	std::mt19937 rng;
	auto partition = [&a, &rng](int l, int r) {
		int pivot = a[std::uniform_int_distribution<int>(l, r - 1)(rng)];
		int li = l - 1;
		int ri = r + 1;
		while (true) {
			do { --ri; } while (a[ri] > pivot);
			do { ++li; } while (a[li] < pivot);
			if (li < ri)
				std::swap(a[li], a[ri]);
			else
				return ri;
		}
	};

	std::function<void(int, int)> quicksortRec = [&quicksortRec, &partition, &a](int l, int r) {
		if (l < r) {
			int pivot = partition(l, r);
			quicksortRec(l, pivot);
			quicksortRec(pivot + 1, r);
		}
	};
	
	quicksortRec(0, int(a.size()) - 1);
}
}