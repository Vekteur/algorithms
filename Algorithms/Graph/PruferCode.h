#pragma once

#include <vector>
#include <set>
#include <functional>

#include "Graph.h"

template<typename L>
std::vector<int> pruferCode(const AdjList<L>& g) {
	int n = g.size();
	// Build the parent tree with n - 1 as the root
	std::vector<int> parent(g.size(), -1);
	std::function<void(int)> dfs = [&dfs, &g, &parent](int u) {
		for (Edge<L> v : g.adj[u]) {
			if (parent[v.to] == -1) {
				parent[v.to] = u;
				dfs(v.to);
			}
		}
	};
	dfs(n - 1);

	std::vector<int> degree(n);
	for (int i = 0; i < n; ++i)
		degree[i] = g.adj[i].size();

	int ptr = -1;
	for (int i = 0; i < n; ++i) {
		if (degree[i] == 1) {
			ptr = i;
			break;
		}
	}

	int leaf = ptr;
	// Invariant : there exists at most one leaf that is <= ptr
	std::vector<int> code;
	for (int k = 0; k < n - 2; ++k) {
		int next = parent[leaf];
		code.push_back(next);
		--degree[next];
		if (degree[next] == 1 && next < ptr) {
			leaf = next;
		} else {
			while (degree[++ptr] != 1);
			leaf = ptr;
		}
	}
	return code;
}

namespace {
AdjList<> pruferTree(const std::vector<int>& code) {

	int n = code.size() + 2;
	std::vector<int> degree(n, 1);
	for (int u : code)
		++degree[u];

	int ptr = -1;
	while (degree[++ptr] != 1);

	int leaf = ptr;
	// Invariant : there exists at most one leaf that is <= ptr
	AdjList<> g{ n };
	for (int u : code) {
		g.addEdge(leaf, u);
		g.addEdge(u, leaf);
		if (--degree[u] == 1 && u < ptr) {
			leaf = u;
		} else {
			while (degree[++ptr] != 1);
			leaf = ptr;
		}
	}
	g.addEdge(leaf, n - 1);
	g.addEdge(n - 1, leaf);
	return g;
}
}