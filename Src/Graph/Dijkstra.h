#pragma once

#include <vector>
#include <queue>
#include <algorithm>

#include "Graph.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<int>> dijkstra(const AdjList<L>& g, int start) {
	std::vector<int> minDist(g.size(), -1);
	std::vector<int> pred(g.size(), -1);

	using SavedEdge = BiEdge<WeightLabel>;
	struct GreaterEdge {
		bool operator()(const SavedEdge& e1, const SavedEdge& e2) { return e1.w > e2.w; }
	};
	std::priority_queue < SavedEdge, std::vector<SavedEdge>, GreaterEdge > pq;
	pq.push({ -1, start, { 0 } });

	while (!pq.empty()) {
		SavedEdge nearest = pq.top(); pq.pop();
		int u = nearest.to;
		// Only process unvisited nodes
		if (minDist[u] == -1) {
			minDist[u] = nearest.w;
			pred[u] = nearest.from;
			for (Edge<L> v : g.adj[u]) {
				// Don't push already visited nodes (optional but faster)
				if (minDist[v.to] == -1) {
					pq.push({ u, v.to, { nearest.w + v.w } });
				}
			}
		}
	}
	return { minDist, pred };
}

std::vector<int> dijkstraBacktrack(const std::vector<int>& pred, int start, int dest) {
	std::vector<int> path;
	int curr = dest;
	while (curr != start) {
		path.push_back(curr);
		curr = pred[curr];
	}
	std::reverse(path.begin(), path.end());
	return path;
}