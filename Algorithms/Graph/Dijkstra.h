#pragma once

#include <vector>
#include <queue>
#include <algorithm>

#include "Graph.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<int>> dijkstra(const AdjList<L>& g, int start) {
	std::vector<int> minDist(g.size(), -1);
	std::vector<int> pred(g.size(), -1);

	struct GreaterEdge {
		bool operator()(const BiEdge<L>& a, const BiEdge<L>& b) { return a.w > b.w; }
	};
	std::priority_queue < BiEdge<L>, std::vector<BiEdge<L>>, GreaterEdge > pq;
	BiEdge<L> first(-1, start);
	first.w = 0;
	pq.push(first);

	while (!pq.empty()) {
		BiEdge<L> nearest = pq.top(); pq.pop();
		int u = nearest.to;

		if (minDist[u] == -1) {
			minDist[u] = nearest.w; // The unvisited nodes are at distance -1
			pred[u] = nearest.from;
			for (Edge<L> v : g.adj[u]) {
				int dist = v.w + nearest.w;
				if (minDist[v.to] == -1) {
					// If not already visited (optional but faster)
					BiEdge<L> cand(u, v.to);
					cand.w = dist;
					pq.push(cand);
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