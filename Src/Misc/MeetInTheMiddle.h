#pragma once

#include <vector>
#include <algorithm>

#include "Dynamic/Knapsack.h"

namespace {
int meetInTheMiddleKnapsack(const std::vector<KnapsackObject>& objs, int cap) {
	auto findMaxValueSubsets = [](const std::vector<KnapsackObject>& objs) {
		std::vector<KnapsackObject> objSubsets{ {0, 0} };
		for (auto obj : objs) {
			int size = objSubsets.size();
			for (int i = 0; i < size; ++i) {
				auto setSum = objSubsets[i];
				objSubsets.push_back({ setSum.w + obj.w, setSum.v + obj.v });
			}
		}
		std::sort(objSubsets.begin(), objSubsets.end(), 
				[](auto obj1, auto obj2) { return obj1.w < obj2.w; });

		std::vector<KnapsackObject> maxValueSubsets;
		int bestValue = -1;
		for (auto objSubset : objSubsets) {
			if (objSubset.v > bestValue) {
				bestValue = objSubset.v;
				maxValueSubsets.push_back(objSubset);
			}
		}
		return maxValueSubsets;
	};

	int mid = int(objs.size()) / 2;
	auto left = findMaxValueSubsets({ objs.begin(), objs.begin() + mid });
	auto right = findMaxValueSubsets({ objs.begin() + mid, objs.end() });

	int maxValue = 0;
	int i = 0, j = int(right.size()) - 1;
	while (i < int(right.size()) && j >= 0) {
		if (left[i].w + right[j].w <= cap) {
			maxValue = std::max(maxValue, left[i].v + right[j].v);
			++i;
		} else {
			--j;
		}
	}
	return maxValue;
}
}