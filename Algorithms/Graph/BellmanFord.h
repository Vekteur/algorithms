#pragma once

#include <vector>

#include "Graph.h"
#include "Constants.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<int>> bellmanFordDistances(const AdjList<L>& g, int start) {
	std::vector<int> minDist(g.size(), INF);
	std::vector<int> pred(g.size(), -1);

	minDist[start] = 0;
	for (int k = 0; k < g.size() - 1; ++k) { // n - 1 times
		for (int u = 0; u < g.size(); ++u) { // For each edge
			for (Edge<L> v : g.adj[u]) {
				// Pass if not visited
				if (minDist[u] == INF)
					continue;

				// Relaxation
				if (v.w + minDist[u] < minDist[v.to]) {
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
	for (int j = 0; j < g.size(); ++j) { // For each edge
		for (Edge<L> v : g.adj[j]) {
			// Pass if not visited
			if (minDist[j] == INF)
				continue;

			// Return the node that creates a cycle
			if (v.w + minDist[j] < minDist[v.to])
				return j;
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