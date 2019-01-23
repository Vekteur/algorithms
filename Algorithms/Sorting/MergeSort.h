#pragma once

#include <vector>
#include <functional>

namespace {
void mergeSort(std::vector<int>& a) {
	auto merge = [&a](int low, int mid, int high) {
		std::vector<int> b;
		b.reserve(high - low + 1);
		int i = low, j = mid + 1;
		while (i <= mid && j <= high) {
			if (a[i] <= a[j])
				b.push_back(a[i++]);
			else
				b.push_back(a[j++]);
		}
		while (i <= mid)
			b.push_back(a[i++]);
		while (j <= high)
			b.push_back(a[j++]);
		for (int k = 0; k < int(b.size()); ++k)
			a[k + low] = b[k];
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