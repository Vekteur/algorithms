#pragma once

#include <iostream>
#include <vector>
#include <bitset>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>

using vi = std::vector<int>;
using vvi = std::vector<std::vector<int>>;

// Adjacency list and vis array
const int MAXN{ 10'000 };
vi adj[MAXN];
vi adjT[MAXN];
std::bitset<MAXN> vis;

void dfsCC(int u, vi& cc, vi adj[]) {
	if (vis[u])
		return;
	vis[u] = true;
	cc.push_back(u);

	for (int v : adj[u])
		dfsCC(v, cc, adj);
}

void dfs_stack() {
	std::stack<int> st;
	st.push(0);

	while (!st.empty()) {
		int u = st.top(); st.pop();

		for (int v : adj[u]) {
			if (!vis[v]) {
				vis[v] = true;
				st.push(v);
			}
		}
	}
}

void bfs() {
	std::queue<int> q;
	q.push(0);

	while (!q.empty()) {
		int u = q.front(); q.pop();

		for (int v : adj[u]) {
			if (!vis[u]) {
				vis[v] = true;
				q.push(v);
			}
		}
	}
}

void findCC() {
	for (int i = 0; i < MAXN; ++i) {
		if (!vis[i]) {
			vi cc;
			dfsCC(i, cc, adj);

			for (int v : cc)
				std::cout << v << ' ';
			std::cout << '\n';
		}
	}
}

vi getToposortInDegree() {
	int inDegree[MAXN]; // Number of incoming edges for each node
	std::queue<int> zeroIn; // All nodes that have zero incoming edges
	vi toposort;

	// Compute inDegree and zeroIn
	for (int i = 0; i < MAXN; ++i)
		for (int v : adj[i])
			++inDegree[v];

	for (int i = 0; i < MAXN; ++i)
		if (inDegree[i] == 0)
			zeroIn.push(i);

	// Compute toposort
	while (!zeroIn.empty()) {
		int u = zeroIn.front(); zeroIn.pop();
		toposort.push_back(u);

		for (int v : adj[u]) {
			--inDegree[v];
			if (inDegree[v] == 0)
				zeroIn.push(v);
		}
	}

	return toposort; // Every node is before its children
}

void dfsToposort(int u, vi& toposort) {
	if (vis[u])
		return;
	vis[u] = true;

	for (int v : adj[u])
		dfsToposort(v, toposort);

	toposort.push_back(u);
}

vi getToposortDfs() {
	vi toposort;

	for (int i = 0; i < MAXN; ++i)
		dfsToposort(i, toposort);

	return toposort; // Every node is after its children
}

vvi getSCCKosoaraju() {
	vi toposort = getToposortDfs();

	for (int i = 0; i < MAXN; ++i)
		vis[i] = false;

	vvi SCCs;
	for (int i = MAXN - 1; i >= 0; --i) {
		if (!vis[toposort[i]]) {
			vi SCC;
			dfsCC(toposort[i], SCC, adjT);
			SCCs.push_back(SCC);
		}
	}

	return SCCs;
}
