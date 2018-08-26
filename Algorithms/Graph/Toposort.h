#pragma once

#include <queue>
#include <functional>

#include "Graph.h"

template<typename L>
std::vector<int> toposortInDegree(const AdjList<L>& g) {
	std::vector<int> inDegree(g.size()); // Number of incoming edges for each node
	std::queue<int> zeroIn; // All nodes that have zero incoming edges
	std::vector<int> toposort;

	// Compute inDegree and zeroIn
	for (int u = 0; u < g.size(); ++u)
		for (Edge<L> v : g.adj[u])
			++inDegree[v.to];

	for (int u = 0; u < g.size(); ++u)
		if (inDegree[u] == 0)
			zeroIn.push(u);

	// Compute toposort
	while (!zeroIn.empty()) {
		int u = zeroIn.front(); zeroIn.pop();
		toposort.push_back(u);

		for (Edge<L> v : g.adj[u]) {
			--inDegree[v.to];
			if (inDegree[v.to] == 0)
				zeroIn.push(v.to);
		}
	}

	return toposort; // Every node is before its children
}

template<typename L>
std::vector<int> toposortDfs(const AdjList<L>& g) {
	std::vector<int> toposort;
	std::vector<int> vis(g.size());

	std::function<void(int)> dfs = [&vis, &toposort, &dfs, &g](int u) {
		if (vis[u])
			return;
		vis[u] = true;
		for (Edge<L> v : g.adj[u])
			dfs(v.to);
		toposort.push_back(u);
	};

	for (int u = 0; u < g.size(); ++u) {
		dfs(u);
	}
	reverse(toposort.begin(), toposort.end());
	return toposort;
}