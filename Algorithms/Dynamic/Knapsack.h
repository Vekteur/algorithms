#pragma once

#include <vector>
#include <algorithm>

namespace {
struct KnapsackObject { int w, v; };

int knapsack(std::vector<KnapsackObject> objs, int cap) {
	std::vector<int> values(cap + 1, 0);
	for (auto obj : objs)
		for (int c = cap; c - obj.w >= 0; --c)
			values[c] = std::max(values[c], values[c - obj.w] + obj.v);
	return *std::max_element(values.begin(), values.end());
}
}