#pragma once

#include <vector>
#include <functional>

namespace {
void mergeSort(std::vector<int>& a) {
	auto merge = [&a](int l, int mid, int r) {
		std::vector<int> b;
		b.reserve(r - l + 1);
		int i = l, j = mid + 1;
		while (i <= mid && j <= r) {
			if (a[i] <= a[j])
				b.push_back(a[i++]);
			else
				b.push_back(a[j++]);
		}
		while (i <= mid)
			b.push_back(a[i++]);
		while (j <= r)
			b.push_back(a[j++]);
		for (int k = 0; k < int(b.size()); ++k)
			a[k + l] = b[k];
	};

	std::function<void(int, int)> mergeSortRec = [&mergeSortRec, &merge, &a](int l, int r) {
		if (l == r)
			return;
		int mid = l + (r - l) / 2;
		mergeSortRec(l, mid);
		mergeSortRec(mid + 1, r);
		merge(l, mid, r);
	};

	mergeSortRec(0, int(a.size()) - 1);
}
}