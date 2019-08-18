#pragma once

#include "Graph/CartesianTree.h"
#include "Graph/Lca/LcaRangeQuery.h"
#include "Graph/Tree.h"

#include <functional>

template<typename T>
class OptimalRMQ {
private:
	std::vector<T> a;
	LcaRangeQueryOptimized lca;
	OptimalRMQ(const std::vector<T>& a, const AdjList<>& cartesianTree) 
		: a(a), lca(cartesianTree, treeRoot(cartesianTree)) {}

public:
	OptimalRMQ(const std::vector<T>& a, std::function<bool(T, T)> comp = std::less<T>())
		: OptimalRMQ(a, buildCartesianTree(a, comp)) {}

	T query(int sl, int sr) {
		return a[lca.query(sl, sr)];
	}
};