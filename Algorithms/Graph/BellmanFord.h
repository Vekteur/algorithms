#pragma once

#include <vector>

#include "Graph.h"
#include "Constants.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<int>> bellmanFordDistances(const AdjList<L>& g, int start) {
	std::vector<int> minDist(g.size(), INF);
	minDist[start] = 0;
	std::vector<int> pred(g.size(), -1);

	for (int k = 0; k < g.size() - 1; ++k) {
		for (int u = 0; u < g.size(); ++u) {
			if (minDist[u] == INF) // Pass if not visited
				continue;
			for (Edge<L> v : g.adj[u]) { // For each edge
				if (v.w + minDist[u] < minDist[v.to]) { // Relaxation
					minDist[v.to] = v.w + minDist[u];
					pred[v.to] = u;
				}
			}
		}
	}
	return { minDist, pred };
}

template<typename L>
int bellmanFordCheckCycle(const AdjList<L>& g, const std::vector<int>& minDist) {
	for (int u = 0; u < g.size(); ++u) {
		if (minDist[u] == INF) // Pass if not visited
			continue;
		for (Edge<L> v : g.adj[u]) { // For each edge
			// If the edge can be relaxed, it is part of a negative cycle
			if (v.w + minDist[u] < minDist[v.to])
				return u;
		}
	}
	return -1;
}

std::vector<int> bellmanFordBacktrack(const std::vector<int>& pred, int start, int dest) {
	std::vector<int> path;
	int curr = dest;
	do {
		path.push_back(curr);
		curr = pred[curr];
	} while (curr != start && curr != dest); // Also check for cycle
	std::reverse(path.begin(), path.end());
	return path;
}