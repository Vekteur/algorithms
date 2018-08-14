#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <queue>

using pii = std::pair<int, int>;
using ipii = std::pair<int, pii >;

const int MAXN{ 10'000 };
std::vector<pii> adj[MAXN]; // Contains the adjacent nodes and their respective distance
int taken[MAXN];
std::priority_queue<pii, std::vector<pii>, std::greater<pii>() > pq; // Contains the smallest distances of the nodes that can be reached and their respective node
// Minimax of a and b : compute MST and traverse it from a to b

void process(int u) {
	taken[u] = true;
	for (pii v : adj[u]) {
		if (!taken[v.first]) // Don't add edges to already processed nodes (optional but faster)
			pq.push(std::make_pair(v.second, v.first));
	}
}

int prim() {
	int length = 0;

	pq.push(std::make_pair(0, 0));
	while (!pq.empty()) {
		pii top = pq.top(); pq.pop();
		int u = top.second;

		// The priority queue can contain multiple edges to the same node but only the smallest accessible will be taken
		if (!taken[u]) {
			process(u);
			length += top.first;
		}
	}
	return length;
}