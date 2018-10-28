#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

#include "Graph.h"
#include "Constants.h"

template<typename L>
int maxflowEdmondsKarp(AdjMat<L> cap, int source, int sink) {
	int n = cap.size();
	AdjList<> g(n);
	for (int u = 0; u < n; ++u)
		for (int v = 0; v < n; ++v)
			if (cap(u, v).w != 0) {
				g.addEdge(u, v);
				g.addEdge(v, u);
			}

	auto bfs = [&cap, &g, &source, &sink, &n]() {
		std::vector<int> pred(n, -1);
		std::vector<int> flows(n, 0);
		flows[source] = INF;
		pred[source] = n;
		std::queue<int> q;
		q.push(source);

		while (!q.empty()) {
			int u = q.front(); q.pop();
			for (Edge<> v : g.adj[u]) {
				if (pred[v.to] == -1 && cap(u, v.to).w != 0) {
					pred[v.to] = u;
					flows[v.to] = std::min(flows[u], cap(u, v.to).w);
					q.push(v.to);
				}
			}
		}

		int flow = flows[sink];
		if (flow == 0)
			return 0;

		// Update graph
		int curr = sink;
		while (curr != source) {
			int prev = pred[curr];
			cap(curr, prev).w += flow;
			cap(prev, curr).w -= flow;
			curr = prev;
		}
		return flow;
	};

	for (int u = 0; u < n; ++u) {
		for (int v = 0; v < n; ++v) {
			if (cap(u, v).w != 0)
				std::cout << '(' << u << ", " << v << ')' << " : " << cap(u, v).w << std::endl;
		}
	}
	std::cout << std::endl;

	int maxFlow = 0, flow;
	while ((flow = bfs()) != 0) {
		maxFlow += flow;
		std::cout << flow << '\n';
		for (int u = 0; u < n; ++u) {
			for (int v = 0; v < n; ++v) {
				if (cap(u, v).w != 0)
					std::cout << '(' << u << ", " << v << ')' << " : " << cap(u, v).w << std::endl;
			}
		}
		std::cout << std::endl;
	}
	return maxFlow;
}