#pragma once

#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
int maxflowEK(MappedAdjList<L> g, int source, int sink) {
	int flow = 0;
	int pcap;

	std::function<int()> augmentBFS = [&g, &source, &sink]() {
		// Init BFS
		std::queue<int> q;
		std::vector<int> p(g.size(), -1); // Parents
		std::vector<int> pcap(g.size()); // Parents capacity
		pcap[source] = INF; // We want the maximum flow from the source
		q.push(source);
		// Compute path
		while (p[sink] == -1 && !q.empty()) {
			int u = q.front(); q.pop();
			for (std::pair<int, L> e : g.adj[u]) {
				int v = e.first;
				// If the weight is not 0 in the residual graph and v has no parent
				if (e.second.w > 0 && p[v] == -1) {
					p[v] = u; // Modify parent
					pcap[v] = min(pcap[u], e.second.w); // Modify the capacity of the parent
					q.push(v);
				}
			}
		}
		if (p[sink] == -1) return -1;
		//Update graph
		int cur = sink;
		while (cur != source) {
			int prev = p[cur];
			// g[prev][cur] always exists
			g.adj[prev][cur].w -= pcap[sink]; // Decrease the capacity of the augmenting path from source to sink
			// g[cur][prev] is 0 if it doesn't exist
			g.adj[cur][prev].w += pcap[sink]; // Increase the capacity of the augmenting path from sink to source
			cur = prev;
		}
		return pcap[sink];
	};

	while ((pcap = augmentBFS()) != -1)
		flow += pcap;
	return flow;
}