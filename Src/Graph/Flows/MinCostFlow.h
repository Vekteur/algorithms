#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <tuple>

#include "Graph/Graph.h"
#include "Constants.h"

struct WeightCostLabel {
	int w, cost;
};

template<typename L>
std::tuple<int, int> minCostFlow(AdjMat<std::vector<L>> mat, int source, int sink, int requestedFlow = INF) {
	int n = mat.size();

	// Multiple edges between two nodes are allowed
	// The adjacency list keeps track of the index of the edge in the adjacency matrix
	struct IndexLabel {
		int index;
	};
	AdjList<IndexLabel> g(n);
	for (int u = 0; u < n; ++u) {
		for (int v = u; v < n; ++v) {
			int index = 0;
			std::vector<L> oppEdges = std::move(mat(v, u));
			for (L l : mat(u, v)) {
				mat(v, u).push_back({ 0, -l.cost });
				g.addEdge(u, v, { index });
				g.addEdge(v, u, { index });
				++index;
			}
			for (L l : oppEdges) {
				mat(u, v).push_back({ 0, -l.cost });
				mat(v, u).push_back(l);
				g.addEdge(u, v, { index });
				g.addEdge(v, u, { index });
				++index;
			}
		}
	}

	int currFlow = 0, minCost = 0;

	// SPFA is used to find min cost paths
	auto spfa = [&mat, &g, &source, &sink, &n, &requestedFlow, &currFlow, &minCost]() {
		std::vector<int> flows(n, 0);
		flows[source] = INF;
		std::vector<int> minDist(n, INF);
		minDist[source] = 0;
		std::vector<std::tuple<int, int>> pred(n, { -1, -1 }); // Contains tuples (predecessor, edge index)
		std::vector<bool> inQueue(g.size(), false);
		inQueue[source] = true;
		std::queue<int> q;
		q.push(source);

		while (!q.empty()) {
			int u = q.front(); q.pop();
			inQueue[u] = false;
			for (Edge<IndexLabel> v : g.adj[u]) {
				L label = mat(u, v.to)[v.index];
				if (label.cost + minDist[u] < minDist[v.to] && label.w != 0) {
					pred[v.to] = { u, v.index };
					flows[v.to] = std::min(flows[u], label.w);
					minDist[v.to] = label.cost + minDist[u];
					if (!inQueue[v.to]) {
						q.push(v.to);
						inQueue[v.to] = true;
					}
				}
			}
		}

		int flow = std::min(requestedFlow - currFlow, flows[sink]);
		if (flow == 0)
			return 0;

		// Update graph
		minCost += flow * minDist[sink];
		int curr = sink;
		while (curr != source) {
			int prev, index;
			std::tie(prev, index) = pred[curr];
			mat(curr, prev)[index].w += flow;
			mat(prev, curr)[index].w -= flow;
			curr = prev;
		}
		return flow;
	};

	int flow;
	while (currFlow != requestedFlow && (flow = spfa()) != 0)
		currFlow += flow;
	return { currFlow, minCost };
}