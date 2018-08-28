#pragma once

#include <vector>
#include <set>

#include "Graph.h"

template<typename L>
std::vector<int> pruferCode(const AdjList<L>& g) {
	std::vector<int> degree(g.size());
	std::vector<bool> killed(g.size(), false);
	std::set<int> leaves;
	for (int i = 0; i < g.size(); ++i) {
		degree[i] = g.adj[i].size();
		if (degree[i] == 1)
			leaves.insert(i);
	}
	std::vector<int> code;
	for (int k = 0; k < g.size() - 2; ++k) {
		int minLeave = *leaves.begin();
		leaves.erase(leaves.begin());
		killed[minLeave] = true;
		int parent;
		for (Edge<L> e : g.adj[minLeave]) {
			if (!killed[e.to]) {
				parent = e.to;
				break;
			}
		}
		if (--degree[parent] == 1)
			leaves.insert(parent);
		code.push_back(parent);
	}
	return code;
}