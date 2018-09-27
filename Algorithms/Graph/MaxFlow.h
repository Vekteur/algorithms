#pragma once

#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
int maxflowEK(MappedAdjList<L> g, int source, int sink) {
	std::function<int()> augmentBFS = [&g, &source, &sink]() {
		std::vector<int> pred(g.size(), -1);
		std::vector<int> cap(g.size());
		cap[source] = INF;
		std::queue<int> q;
		q.push(source);
		// Compute path
		while (!q.empty() && pred[sink] == -1) {
			int u = q.front(); q.pop();
			for (std::pair<int, L> e : g.adj[u]) {
				int v = e.first;
				if (e.second.w != 0 && pred[v] == -1) {
					pred[v] = u;
					cap[v] = min(cap[u], e.second.w);
					q.push(v);
				}
			}
		}
		if (pred[sink] == -1)
			return -1;
		// Update graph
		int curr = sink;
		while (curr != source) {
			int prev = pred[curr];
			// adj[prev][cur] is an edge of the path
			g.adj[prev][curr].w -= cap[sink];
			// adj[cur][prev] is initialized to 0 if it doesn't exist
			g.adj[curr][prev].w += cap[sink];
			curr = prev;
		}
		return cap[sink];
	};

	int cap;
	int maxFlow = 0;
	while ((cap = augmentBFS()) != -1)
		maxFlow += cap;
	return maxFlow;
}