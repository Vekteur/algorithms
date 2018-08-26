#pragma once

#include <utility>
#include <algorithm>

#include "Graph.h"
#include "DataStructure/UnionFind.h"

template<typename L>
EdgeList<> kruskal(const EdgeList<L>& g) {
	EdgeList<> mst;
	UnionFind uf(g.size());
	std::vector<BiEdge<L>> edges = g.list;
	std::sort(edges.begin(), edges.end(), 
		[](const BiEdge<L>& e1, const BiEdge<L>& e2) { return e1.w < e2.w; });

	for (int i = 0; i < g.size(); ++i) {
		int from = edges[i].from;
		int to = edges[i].to;
		if (!uf.sameSet(from, to)) {
			mst.addEdge(from, to);
			uf.merge(from, to);
		}
	}
	return mst;
}