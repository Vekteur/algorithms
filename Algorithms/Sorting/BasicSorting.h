#pragma once

#include <vector>

namespace {
	void bubbleSort(std::vector<int>& a) {
		bool modified = true;
		while (modified) {
			modified = false;
			for (int j = 0; j < a.size() - 1; ++j) {
				if (a[j] > a[j + 1]) {
					std::swap(a[j], a[j + 1]);
					modified = true;
				}
			}
		}
	}

	void selectionSort(std::vector<int>& a) {
		for (int i = 0; i < a.size() - 1; ++i) {
			int minJ = i;
			for (int j = i + 1; j < a.size(); ++j) {
				if (a[j] < a[minJ])
					minJ = j;
			}
			std::swap(a[i], a[minJ]);
		}
	}

	void insertionSort(std::vector<int>& a) {
		for (int i = 1; i < a.size(); ++i) {
			for (int j = i; j >= 1 && a[j - 1] > a[j]; --j) {
				std::swap(a[j - 1], a[j]);
			}
		}
	}

	void countingSort(std::vector<int>& a, int maxValue) {
		std::vector<int> count(maxValue + 1);
		for (int el : a)
			++count[el];
		int aIndex = 0;
		for (int c = 0; c < count.size(); ++c)
			for (int k = 0; k < count[c]; ++k)
				a[aIndex++] = c;
	}
}