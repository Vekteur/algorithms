#pragma once

#include <algorithm>
#include <functional>
#include <queue>

#include "Graph.h"

template<typename L>
std::vector<int> prim(const AdjList<L>& g) {
	std::vector<int> vis(g.size());
	std::vector<int> pred(g.size());
	// Shortest edges are at the top of pq
	struct GreaterEdge {
		bool operator()(BiEdge<L> e1, BiEdge<L> e2) { return e1.w > e2.w; }
	};
	std::priority_queue< BiEdge<L>, std::vector<BiEdge<L>>, GreaterEdge > pq;

	BiEdge<L> first(-1, 0);
	first.w = 0;
	pq.push(first);
	while (!pq.empty()) {
		BiEdge<L> minEdge = pq.top(); pq.pop();
		int u = minEdge.to;

		// The priority queue can contain multiple edges to the same node but only the smallest accessible will be taken
		if (!vis[u]) {
			vis[u] = true;
			pred[u] = minEdge.from;
			for (Edge<L> v : g.adj[u]) {
				if (!vis[v.to]) { // Don't add edges to already processed nodes (optional but faster)
					BiEdge<L> cand(u, v.to);
					cand.w = v.w;
					pq.push(cand);
				}
			}
		}
	}
	return pred;
}