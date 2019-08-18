#pragma once

#include <algorithm>
#include <functional>
#include <queue>

#include "Graph/Graph.h"

template<typename L>
std::vector<int> prim(const AdjList<L>& g, int start) {
	std::vector<int> vis(g.size());
	std::vector<int> pred(g.size());

	using SavedEdge = BiEdge<WeightLabel>;
	struct GreaterEdge {
		bool operator()(const SavedEdge& e1, const SavedEdge& e2) { return e1.w > e2.w; }
	};
	std::priority_queue < SavedEdge, std::vector<SavedEdge>, GreaterEdge > pq;
	pq.push({ -1, start, { 0 } });

	while (!pq.empty()) {
		SavedEdge minEdge = pq.top(); pq.pop();
		int u = minEdge.to;
		// Only process unvisited nodes
		if (!vis[u]) {
			vis[u] = true;
			pred[u] = minEdge.from;
			for (Edge<L> v : g.adj[u]) {
				// Don't push already visited nodes (optional but faster)
				if (!vis[v.to]) {
					pq.push({ u, v.to, { v.w } });
				}
			}
		}
	}
	return pred;
}