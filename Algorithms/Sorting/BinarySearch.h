#pragma once

#include <vector>

namespace {
	int binary_search(std::vector<int> arr, int v) {
		int low = 0;
		int high = arr.size() - 1;
		while (low < high) {
			int mid = (low + high) / 2;
			if (arr[mid] < v)
				low = mid + 1;
			else
				high = mid;
		}
		return arr[low] == v ? low : -1;
	}
}