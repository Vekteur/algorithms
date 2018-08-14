#pragma once

#include <iostream>
#include <algorithm>
#include <functional>

const int MAXN{ 1'000 }, MAXC{ 2'000 };

struct Obj { int w, v; };

Obj objs[MAXN];
int best[MAXN][MAXC + 1];

int n, C;

int knapsack() {
	for (int c = 0; c <= C; ++c)
		if (c >= objs[0].w)
			best[0][c] = objs[0].v;
		else
			best[0][c] = 0;

	for (int i = 1; i < n; ++i)
		for (int c = 0; c <= C; ++c)
			if (c < objs[i].w)
				best[i][c] = best[i - 1][c];
			else
				best[i][c] = std::max(best[i - 1][c], best[i - 1][c - objs[i].w] + objs[i].v);

	return best[n - 1][C];
}