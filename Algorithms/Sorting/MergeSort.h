#pragma once

#include <vector>
#include <functional>

namespace {
void mergeSort(std::vector<int>& a) {
	int n = a.size();
	std::vector<int> buf(n);
	auto merge = [&a, &buf](int l, int mid, int r) {
		int i = l, j = mid;
		int buf_i = l;
		while (i < mid && j < r) {
			if (a[i] <= a[j])
				buf[buf_i++] = a[i++];
			else
				buf[buf_i++] = a[j++];
		}
		while (i < mid)
			buf[buf_i++] = a[i++];
		while (j < r)
			buf[buf_i++] = a[j++];
		for (int k = l; k < r; ++k)
			a[k] = buf[k];
	};

	std::function<void(int, int)> mergeSortRec = [&mergeSortRec, &merge, &a](int l, int r) {
		if (r - l <= 1)
			return;
		int mid = l + (r - l) / 2;
		mergeSortRec(l, mid);
		mergeSortRec(mid, r);
		merge(l, mid, r);
	};

	mergeSortRec(0, n);
}
}