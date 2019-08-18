#pragma once

#include "Graph.h"

#include <functional>

AdjList<> predecessorsToTree(const std::vector<int>& preds, bool undirected = false) {
	AdjList<> tree(preds.size());
	for (int i = 0; i < int(preds.size()); ++i) {
		if (preds[i] != -1) {
			tree.addEdge(preds[i], i);
			if (undirected)
				tree.addEdge(i, preds[i]);
		}
	}
	return tree;
}

template<typename L>
std::vector<int> treeToPredecessorsUndirected(const AdjList<L>& tree, int root) {
	std::vector<int> preds(tree.size());
	std::function<void(int)> dfs = [&dfs, &tree, &preds](int u, int parent) {
		preds[u] = parent;
		for (Edge<L> v : tree.adj[u]) {
			if (v.to != parent)
				dfs(v.to, u);
		}
	}
	dfs(root, -1);
	return preds;
}

template<typename L>
std::vector<int> treeToPredecessorsDirected(const AdjList<L>& tree) {
	std::vector<int> preds(tree.size(), -1);
	for (int u = 0; u < tree.size(); ++u) {
		for (Edge<L> v : tree.adj[u]) {
			preds[v.to] = u;
		}
	}
	return preds;
}

template<typename L>
int treeRoot(const AdjList<L>& tree) {
	std::vector<int> preds = treeToPredecessorsDirected(tree);
	for (int u = 0; u < int(preds.size()); ++u)
		if (preds[u] == -1)
			return u;
	return -1;
}