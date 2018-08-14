#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <bitset>
#include <algorithm>
#include <functional>

using namespace std;

using pii = pair<int, int>;

const int MAXN{ 1'000 };
map<int, int> g[MAXN];

int n;

int augmentBFS(map<int, int> g[], int source, int sink) {
	// Init BFS
	queue<int> q;
	vector<int> p(n, -1); // Parents
	vector<int> pcap(n); // Parents capacity
	pcap[source] = numeric_limits<int>::max(); // We want the maximum flow from the source
	q.push(source);
	// Compute path
	while (p[sink] == -1 && !q.empty()) {
		int u = q.front(); q.pop();
		for (pair<int, int> e : g[u]) {
			int v = e.first;
			// If the weight is not 0 in the residual graph and v has no parent
			if (e.second > 0 && p[v] == -1) {
				p[v] = u; // Modify parent
				pcap[v] = min(pcap[u], e.second); // Modify the capacity of the parent
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
		g[prev][cur] -= pcap[sink]; // Decrease the capacity of the augmenting path from source to sink
									// g[cur][prev] is 0 if it doesn't exist
		g[cur][prev] += pcap[sink]; // Increase the capacity of the augmenting path from sink to source
		cur = prev;
	}
	return pcap[sink];
}

int maxflowEK(map<int, int> g[], int source, int sink) {
	int flow = 0;
	int pcap;
	while ((pcap = augmentBFS(g, source, sink)) != -1)
		flow += pcap;

	return flow;
}