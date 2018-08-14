#pragma once

#include <utility>
#include <algorithm>

#include "../DataStructure/UnionFind.h"

using pii = std::pair<int, int>;
using ipii = std::pair<int, pii >;

int n;
std::vector<ipii> edges;
std::vector<ipii> st;

int kruskal() {
	int length = 0;
	UnionFind uf(n);
	std::sort(edges.begin(), edges.end());

	for (int i = 0; i < n; ++i) {
		int x = edges[i].second.first;
		int y = edges[i].second.second;
		if (!uf.sameSet(x, y)) {
			st.push_back(edges[i]);
			length += edges[i].first;
			uf.merge(x, y);
		}
	}
	return length;
}