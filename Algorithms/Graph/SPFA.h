#pragma once

#include <vector>
#include <queue>

#include "Graph.h"
#include "Constants.h"

template<typename L>
std::tuple<std::vector<int>, std::vector<int>> spfaDistances(const AdjList<L>& g, int start) {
	std::vector<int> minDist(g.size(), INF);
	minDist[start] = 0;
	std::vector<int> pred(g.size(), -1);
	std::vector<bool> inQueue(g.size(), false);
	inQueue[start] = true;
	std::queue<int> q;
	q.push(start);

	while (!q.empty()) {
		int u = q.front(); q.pop();
		inQueue[u] = false;
		for (Edge<L> v : g.adj[u]) {
			if (v.w + minDist[u] < minDist[v.to]) { // Relaxation
				minDist[v.to] = v.w + minDist[u];
				pred[v.to] = u;
				if (!inQueue[v.to]) {
					q.push(v.to);
					inQueue[v.to] = true;
				}
			}
		}
	}
	return { minDist, pred };
}