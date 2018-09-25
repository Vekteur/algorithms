#pragma once

#include <vector>
#include <functional>

namespace {
void mergeSort(std::vector<int>& a) {
	std::function<void(int, int, int)> merge = [&a](int low, int mid, int high) {
		std::vector<int> b;
		b.reserve(high - low + 1);
		int i = low, j = mid + 1;
		while (true) {
			if (a[i] <= a[j]) {
				b.push_back(a[i]);
				if (++i == mid + 1) {
					for (int k = j; k <= high; ++k)
						b.push_back(a[k]);
					break;
				}
			} else {
				b.push_back(a[j]);
				if (++j == high + 1) {
					for (int k = i; k <= mid; ++k)
						b.push_back(a[k]);
					break;
				}
			}
		}
		for (int k = 0; k < int(b.size()); ++k) {
			a[k + low] = b[k];
		}
	};

	std::function<void(int, int)> mergeSort = [&mergeSort, &merge, &a](int low, int high) {
		if (low == high)
			return;
		int mid = low + (high - low) / 2;
		mergeSort(low, mid);
		mergeSort(mid + 1, high);
		merge(low, mid, high);
	};
	mergeSort(0, a.size() - 1);
}
}