#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
bool containsEulerianCycle(const AdjList<L>& g) {
	for (int i = 1; i < g.size(); ++i) {
		if ((g.adj[i].size() & 1) != 0)
			return false;
	}
	return true;
}

template<typename L>
bool containsEulerianPath(const AdjList<L>& g) {
	int odd = 0;
	for (int i = 1; i < g.size(); ++i) {
		if ((g.adj[i].size() & 1) != 0)
			++odd;
	}
	// odd == 0 if the graph contains an eulerian cycle
	return odd == 2 || odd == 0;
}

template<typename L>
std::vector<int> findEulerianPath(AdjList<L> g, int start) {
	std::vector<int> path;
	std::function<void(int)> dfs = [&dfs, &path, &g](int u) {
		for (Edge<L>& v : g.adj[u]) {
			if (!v.vis) {
				v.vis = true;
				dfs(v.to);
			}
		}
		path.push_back(u);
	};
	dfs(start);
	reverse(path.begin(), path.end());
	return path;
}