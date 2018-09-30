#pragma once

#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
std::vector<int> dfsStack(const AdjList<L>& g) {
	std::vector<int> preOrder;
	std::vector<int> vis(g.size());
	std::stack<int> st;
	st.push(0);

	while (!st.empty()) {
		int u = st.top(); st.pop();
		preOrder.push_back(u);

		for (Edge<L> v : g.adj[u]) {
			if (!vis[v.to]) {
				vis[v.to] = true;
				st.push(v.to);
			}
		}
	}
	return preOrder;
}

template<typename L>
std::vector<int> bfs(const AdjList<L>& g) {
	std::vector<int> preOrder;
	std::vector<int> vis(g.size());
	std::queue<int> q;
	q.push(0);

	while (!q.empty()) {
		int u = q.front(); q.pop();
		preOrder.push_back(u);

		for (Edge<L> v : g.adj[u]) {
			if (!vis[v.to]) {
				vis[v.to] = true;
				q.push(v.to);
			}
		}
	}
	return preOrder;
}


