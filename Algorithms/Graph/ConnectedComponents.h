#pragma once

#include <functional>

#include "Graph.h"
#include "Toposort.h"

template<typename L>
bool isConnected(const AdjList<L>& g) {
	if (g.size() <= 1)
		return true;
	int count = 0;
	std::vector<int> vis(g.size(), false);

	std::function<void(int)> dfs = [&g, &dfs, &count, &vis](int u) {
		if (vis[u])
			return;
		vis[u] = true;
		++count;
		for (Edge<L> v : g.adj[u])
			dfs(v.to);
	};
	dfs(0);

	return count == g.size();
}

template<typename L>
std::vector<std::vector<int>> connectedComponents(const AdjList<L>& g) {
	std::vector<std::vector<int>> ccs;
	std::vector<int> vis(g.size(), false);
	for (int u = 0; u < g.size(); ++u) {
		if (!vis[u]) {
			std::vector<int> cc;

			std::function<void(int)> dfs = [&g, &dfs, &cc, &vis](int u) {
				if (vis[u])
					return;
				vis[u] = true;
				cc.push_back(u);
				for (Edge<L> v : g.adj[u])
					dfs(v.to);
			};
			dfs(u);

			ccs.push_back(cc);
		}
	}
	return ccs;
}

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