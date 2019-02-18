#pragma once

#include <vector>
#include <algorithm>

#include "Constants.h"

namespace {
struct KnapsackObject { int w, v; };

int knapsack(const std::vector<KnapsackObject>& objs, int cap) {
	std::vector<int> values(cap + 1, -INF);
	values[0] = 0;
	for (auto obj : objs)
		for (int c = cap; c - obj.w >= 0; --c)
			if (values[c - obj.w] != -INF)
				values[c] = std::max(values[c], values[c - obj.w] + obj.v);
	return *std::max_element(values.begin(), values.end());
}
}