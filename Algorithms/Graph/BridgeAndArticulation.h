#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<BiEdge<>>> bridgesAndArticulationPoints(const AdjList<L>& g, int u) {
	std::vector<int> num(g.size()); // Timestamp
	std::vector<int> low(g.size()); // Lowest timestamp to which any node in the subtree of i has a direct link
	std::vector<int> vis(g.size(), false);

	int root = u;
	int rootChildren = 0; // Number of children of the root
	int cnt = 0;
	std::vector<int> articulationPoints;
	std::vector<BiEdge<>> bridges;

	std::function<void(int, int)> dfs = 
		[&dfs, &g, &num, &low, &vis, &articulationPoints, &bridges, &root, &rootChildren, &cnt](int u, int parent) {
		// Property used : num[i] < num[j] <=> i is parent of j
		num[u] = low[u] = cnt++; // Increment cnt
		vis[u] = true;
		for (Edge<L> v : g.adj[u]) {
			if (!vis[v.to]) // If not visited
			{
				dfs(v.to, u); // Recursion
				if (u == root)
					++rootChildren;
				// u is an articulation point iff no node in the sub-tree of v has a link to a strict ancestor of u
				if (num[u] <= low[v.to] && u != root)
					articulationPoints.push_back(u);
				// (u, v) is a bridge iff no node in the sub-tree of v has a link to u or one of its ancestors other than (u, v)
				if (num[u] < low[v.to])
					bridges.push_back({ u, v.to });
				low[u] = std::min(low[u], low[v.to]);
			} else if (v.to != parent) // If already visited and not the parent -> cycle
				low[u] = std::min(low[u], num[v.to]);
		}
	};
	dfs(root, -1);

	if (rootChildren > 1)
		articulationPoints.push_back(u);

	return { articulationPoints, bridges };
}