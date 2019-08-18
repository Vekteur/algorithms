#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Graph.h"

template<typename L>
bool hasCycle(const AdjList<L>& g) {
	enum State { UNVISITED, OPENED, CLOSED };
	std::vector<State> states(g.size());

	bool foundCycle = false;
	std::function<void(int)> dfs = [&foundCycle, &states, &g, &dfs](int u) {
		if (states[u] == OPENED)
			foundCycle = true;
		if (states[u] != UNVISITED)
			return;
		states[u] = OPENED;
		for (Edge<L> v : g.adj[u])
			dfs(v.to);
		states[u] = CLOSED;
	};

	for (int u = 0; u < g.size(); ++u)
		if (states[u] == UNVISITED)
			dfs(u);

	return foundCycle;
}