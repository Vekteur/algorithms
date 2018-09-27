#pragma once

#include <vector>

#include "Graph.h"
#include "DataStructure/UnionFind.h"

struct TarjanQuery { int a, b; };

template<typename L>
std::vector<int> lcaTarjanOffline(const AdjList<L>& g, int root, const std::vector<TarjanQuery>& queries) {
	struct OtherNode { int node, query; };
	std::vector<std::vector<OtherNode>> otherInQuery(g.size());
	for (int i = 0; i < int(queries.size()); ++i) {
		TarjanQuery q = queries[i];
		otherInQuery[q.a].push_back({ q.b, i });
		otherInQuery[q.b].push_back({ q.a, i });
	}
	std::vector<int> vis(g.size(), false);
	std::vector<int> setAncestor(g.size()); // unionAncestor[uf.find(u)] = node with least depth in the set of u
	for (int i = 0; i < g.size(); ++i)
		setAncestor[i] = i;
	UnionFind uf(g.size());
	std::vector<int> ancestors(g.size());
	std::function<void(int)> dfs = [&dfs, &g, &otherInQuery, &setAncestor, &uf, &vis, &ancestors](int u) {
		vis[u] = true;
		for (Edge<L> v : g.adj[u]) {
			dfs(v.to);
			uf.merge(u, v.to);
			setAncestor[uf.find(u)] = u;
		}
		for (OtherNode other : otherInQuery[u]) {
			if (vis[other.node])
				ancestors[other.query] = setAncestor[uf.find(other.node)];
		}
	};
	dfs(root);

	return ancestors;
}