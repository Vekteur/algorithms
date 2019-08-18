#pragma once

#include <vector>

#include "Graph.h"
#include "Constants.h"

template<typename L>
int directedMstEdmond(const EdgeList<L>& g, int root) {

	std::vector<BiEdge<WeightLabel>> minInEdge(g.size(), { -1, -1, {INF} });
	for (auto e : g.list) { // Greedily choose the minimum vertex pointing to a node
		minInEdge[e.to] = std::min(minInEdge[e.to], e, 
			[](auto e1, auto e2) { return e1.w < e2.w; });
	}
	minInEdge[root] = { -1, root, {0} };

	std::vector<int> scc(g.size(), -1);
	std::vector<bool> vis(g.size(), false), cycleInScc(g.size(), false);
	int sccNb = 0;
	for (int u = 0; u < g.size(); ++u) {
		if (!vis[u]) {
			int v = u;
			std::vector<int> path;
			for (; v != -1 && !vis[v]; v = minInEdge[v].from) {
				vis[v] = true;
				path.push_back(v);
			}

			int i = 0, s = int(path.size());
			for (; i < s && path[i] != v; ++i)
				scc[path[i]] = sccNb++;
			if (i < s) { // v is the origin of a cycle
				for (; i < s; ++i)
					scc[path[i]] = sccNb;
				cycleInScc[sccNb++] = true;
			}
		}
	}

	if (sccNb == g.size()) {
		int sum = 0;
		for (auto e : minInEdge)
			sum += e.w;
		return sum;
	}

	EdgeList<L> gScc(sccNb);
	for (auto e : g.list) {
		int fromScc = scc[e.from], toScc = scc[e.to];
		if (fromScc != toScc) {
			// If e.to is in a cycle, the minimum edge pointing to e.to must not be taken
			int w = cycleInScc[toScc] ? e.w - minInEdge[e.to].w : e.w;
			gScc.addEdge(fromScc, toScc, { w });
		}
	}

	int sum = 0;
	for (auto e : minInEdge) {
		if (cycleInScc[scc[e.to]])
			sum += e.w;
	}

	return sum + directedMstEdmond(gScc, scc[root]);
}