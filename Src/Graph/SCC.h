#pragma once

#include <functional>

#include "Graph.h"
#include "Toposort.h"

template<typename L>
AdjList<L> invertAdjList(const AdjList<L>& g) {
	AdjList<L> gInv{ g.size() };
	for (int u = 0; u < g.size(); ++u) {
		for (Edge<L> v : g.adj[u]) {
			gInv.addEdge(v.to, u);
		}
	}
	return gInv;
}

template<typename L>
std::vector<std::vector<int>> stronglyConnectedComponents(const AdjList<L>& g) {
	std::vector<int> toposort = toposortDfs(g);
	std::vector<int> vis(g.size(), false);

	AdjList<L> gInv = invertAdjList(g);
	std::vector<std::vector<int>> sccs;

	for (int u : toposort) {
		if (!vis[u]) {
			std::vector<int> scc;
			std::function<void(int)> dfs = [&gInv, &dfs, &vis, &scc](int u) {
				if (vis[u])
					return;
				vis[u] = true;
				scc.push_back(u);
				for (Edge<L> v : gInv.adj[u])
					dfs(v.to);
			};
			dfs(u);
			sccs.push_back(scc);
		}
	}

	return sccs;
}