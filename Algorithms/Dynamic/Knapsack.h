#pragma once

#include <vector>
#include <algorithm>
#include <functional>

namespace {
struct Obj { int w, v; };

int knapsack(std::vector<Obj> objs, int cap) {
	std::vector<int> values(cap + 1, 0);
	for (int i = 0; i < int(objs.size()); ++i)
		for (int c = cap; c - objs[i].w >= 0; --c)
			values[c] = std::max(values[c], values[c - objs[i].w] + objs[i].v);
	return *std::max_element(values.begin(), values.end());
}
}