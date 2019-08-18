#pragma once

#include <vector>

class UnionFind {
private:
	std::vector<int> parent;
	std::vector<int> rank; // Depth of the tree
	// The number of elements in the tree could be used instead

public:
	UnionFind(int size = 0) {
		rank.assign(size, 0);
		parent.resize(size);
		for (int i = 0; i < size; ++i)
			parent[i] = i;
	}

	int find(int a) {
		if (parent[a] == a)
			return a;
		return parent[a] = find(parent[a]); // Path compression
	}

	bool sameSet(int a, int b) {
		return find(a) == find(b);
	}

	void merge(int a, int b) {
		int par_a = find(a);
		int par_b = find(b);
		if (par_a == par_b) return;

		// Heuristic by rank
		if (rank[par_a] > rank[par_b]) {
			parent[par_b] = par_a;
		} else {
			parent[par_a] = par_b;
			if (rank[par_a] == rank[par_b])
				++rank[par_b];
		}
	}
};