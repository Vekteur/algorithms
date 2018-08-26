#pragma once

#include <vector>
#include <algorithm>
#include <functional>

struct Obj { int w, v; };

int knapsack(std::vector<Obj> objs, int cap) {
	std::vector<std::vector<int>> best(objs.size(), std::vector<int>(cap + 1));

	for (int c = 0; c <= cap; ++c)
		if (c >= objs[0].w)
			best[0][c] = objs[0].v;
		else
			best[0][c] = 0;

	for (int i = 1; i < objs.size(); ++i)
		for (int c = 0; c <= cap; ++c)
			if (c < objs[i].w)
				best[i][c] = best[i - 1][c];
			else
				best[i][c] = std::max(best[i - 1][c], best[i - 1][c - objs[i].w] + objs[i].v);

	return best[objs.size() - 1][cap];
}