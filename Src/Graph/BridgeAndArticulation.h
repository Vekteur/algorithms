#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<BiEdge<>>> bridgesAndArticulationPoints(const AdjList<L>& g) {
	std::vector<int> num(g.size()); // Time of entry into node
	// low[i] = lowest num of a node to which any node in the subtree of i has a direct link
	std::vector<int> low(g.size());
	std::vector<int> vis(g.size(), false);

	int root;
	int rootChildren; // Number of children of the root
	int count = 0; // Number of nodes already visited
	std::vector<int> articulationPoints;
	std::vector<BiEdge<>> bridges;

	std::function<void(int, int)> dfs = 
		[&dfs, &g, &num, &low, &vis, &articulationPoints, &bridges, &root, &rootChildren, &count](int u, int parent) {
		// num[i] < num[j] <=> i is an ancestor of j
		num[u] = low[u] = count++;
		vis[u] = true;
		for (Edge<L> v : g.adj[u]) {
			if (!vis[v.to]) {
				if (u == root)
					++rootChildren;
				dfs(v.to, u);
				// u is an articulation point iff no node in the sub-tree of v.to has a link to a strict ancestor of u
				if (num[u] <= low[v.to] && u != root)
					articulationPoints.push_back(u);
				// (u, v.to) is a bridge iff no node in the sub-tree of v.to has a link to u or one of its ancestors
				if (num[u] < low[v.to])
					bridges.push_back({ u, v.to });
				low[u] = std::min(low[u], low[v.to]);
			} else if (v.to != parent) { // If already visited and (u, v.to) do not directly go back to the parent
				low[u] = std::min(low[u], num[v.to]);
			}
		}
	};

	for (int u = 0; u < g.size(); ++u) {
		if (!vis[u]) {
			root = u;
			rootChildren = 0;
			dfs(u, -1);
			if (rootChildren > 1)
				articulationPoints.push_back(root);
		}
	}

	return { articulationPoints, bridges };
}