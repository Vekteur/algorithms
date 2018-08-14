#pragma once

#include <iostream>
#include <vector>
#include <bitset>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>

using vi = std::vector<int>;
using vvi = std::vector<std::vector<int>>;
using pii = std::pair<int, int>;

enum State {
	UNVISITED,
	OPENED,
	CLOSED
};

// Adjacency list and vis array
const int MAXN{ 10'000 };
vi adj[MAXN];
vi adjT[MAXN];
std::bitset<MAXN> vis;
State states[MAXN];

bool foundCycle = false;

// Cycle and bridge detection
int num[MAXN]; // Timestamp
int low[MAXN]; // Lowest timestamp to which any node in the subtree of i has a direct link
int root;
int rootChildren; // Number of children of the root
int cnt = 0;
vi articulationPoints;
std::vector<pii> bridges;

void dfsCycleDetection(int u) {
	if (states[u] == OPENED)
		foundCycle = true;
	if (states[u] != UNVISITED)
		return;
	states[u] = OPENED;

	for (int v : adj[u])
		dfsCycleDetection(v);

	states[u] = CLOSED;
}

bool hasCycle() {
	for (int i = 0; i < MAXN; ++i)
		dfsCycleDetection(i);

	return foundCycle;
}

void dfsFindBridges(int u, int parent) {
	// Property used : num[i] < num[j] <=> i is parent of j
	num[u] = low[u] = cnt++; // Increment cnt
	vis[u] = true;
	for (int v : adj[u]) {
		if (!vis[v]) // If not visited
		{
			dfsFindBridges(v, u); // Recursion
			if (u == root)
				++rootChildren;
			// u is an articulation point iff no node in the sub-tree of v has a link to a strict ancestor of u
			if (num[u] <= low[v] && u != root)
				articulationPoints.push_back(u);
			// (u, v) is a bridge iff no node in the sub-tree of v has a link to u or one of its ancestors other than (u, v)
			if (num[u] < low[v])
				bridges.push_back(std::make_pair(u, v));
			low[u] = std::min(low[u], low[v]);
		} else if (v != parent) // If already visited and not the parent -> cycle
			low[u] = std::min(low[u], num[v]);
	}
}

vi getArticulationPoints(int u) {
	cnt = 0;
	root = u;
	rootChildren = 0;
	dfsFindBridges(root, -1);
	if (rootChildren > 1)
		articulationPoints.push_back(u);
	return articulationPoints;
}

std::vector<pii> getBridges(int u) {
	getArticulationPoints(u);
	return bridges;
}