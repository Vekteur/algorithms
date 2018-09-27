#pragma once


#include <vector>
#include <functional>
#include <cmath>

#include "Graph.h"

class LcaBinaryLifting {
private:
	std::vector<std::vector<int>> up;
	std::vector<int> pre, post;
	int maxLog;

	bool isAncestor(int a, int b) {
		return pre[a] <= pre[b] && post[a] >= post[b];
	}

public:
	template<typename L>
	LcaBinaryLifting(const AdjList<L>& g, int root) : pre(g.size()), post(g.size()) {
		maxLog = int(log2(int(g.size())));
		up.assign(g.size(), std::vector<int>(maxLog + 1));

		int count = 0;
		std::function<void(int, int)> dfs = [&dfs, &count, &g, this](int u, int parent) {
			pre[u] = count++;
			up[u][0] = parent;
			for (int d = 1; d <= maxLog; ++d)
				up[u][d] = up[up[u][d - 1]][d - 1];
			for (Edge<L> v : g.adj[u])
				dfs(v.to, u);
			post[u] = count++;
		};
		dfs(root, root);
	}

	int query(int a, int b) {
		if (isAncestor(a, b))
			return a;
		if (isAncestor(b, a))
			return b;
		for (int d = maxLog; d >= 0; --d) {
			if (!isAncestor(up[a][d], b))
				a = up[a][d];
		}
		return up[a][0];
	}
};