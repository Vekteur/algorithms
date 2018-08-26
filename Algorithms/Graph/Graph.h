#pragma once

#include <tuple>
#include <vector>
#include <map>

#include "Math/Mat.h"

struct DefaultLabel { };

struct SourceLabel {
	int from;
};

struct WeightLabel {
	int w;
};

template<typename L = DefaultLabel>
struct Edge : L {
	int to;
	Edge(int to = 0, L label = L{})
		: L{ label }, to{ to }
	{ }
};

template<typename L = DefaultLabel>
struct BiEdge : Edge<L> {
	int from;
	BiEdge(int from = 0, int to = 0, L label = L{})
		: Edge{ to, label }, from{ from }
	{ }
};

template<typename L = DefaultLabel>
struct AdjList {
	std::vector<std::vector<Edge<L>>> adj;

	AdjList(int n) : adj(n)  {}

	void addEdge(int from, int to, L label = L{}) {
		adj[from].push_back(Edge<L>{ to, label });
	}

	void removeEdge(int from, int to) {
		for (auto it = adj[from].begin(); it != adj[from].end(); ++it) {
			if (it->to == to) {
				adj[from].erase(it);
				break;
			}
		}
	}

	L operator()(int from, int to) {
		for (auto e : adj[from])
			if (e.to == to)
				return e;
		throw "Edge not found";
	}

	int size() const {
		return adj.size();
	}
};

template<typename L = DefaultLabel>
struct MappedAdjList {
	std::vector<std::map<int, L>> adj;

	MappedAdjList(int n) : adj(n) {}

	void addEdge(int from, int to, L label) {
		adj[from][to] = label;
	}

	void removeEdge(int from, int to) {
		for (auto it = adj[from].begin(); it != adj[from].end(); ++it) {
			if (it->first == to) {
				adj[from].erase(it);
				break;
			}
		}
	}

	int size() const {
		return adj.size();
	}
};

template<typename L = DefaultLabel>
struct AdjMat : Mat<L> {
	AdjMat(int n = 0, L defaultValue = L()) : Mat<L>(n, defaultValue) { }

	void addEdge(int row, int col, L label = L()) {
		(*this)(row, col) = label;
	}
};

template<>
struct AdjMat<DefaultLabel> : AdjMat<bool> { };

template<typename L = DefaultLabel>
struct EdgeList {
	std::vector<BiEdge<L>> list;

	void addEdge(int from, int to, L label = L{}) {
		list.push_back({ from, to, label });
	}

	int size() const {
		return list.size();
	}
};