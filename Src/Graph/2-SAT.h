#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Graph.h"
#include "SCC.h"

class TwoSAT {
private:
	AdjList<> g{ 0 };
public:
	struct Variable { int id; bool positive; };
	struct Disjunction { Variable var1, var2; };
private:
	int index(Variable var) {
		return 2 * var.id + 1 - static_cast<int>(var.positive);
	}

	int negIndex(Variable var) {
		return 2 * var.id + static_cast<int>(var.positive);
	}
public:
	void addDisjunction(Disjunction disj) {
		g.adj.resize(std::max({ int(g.adj.size()),
				2 * (disj.var1.id + 1), 2 * (disj.var2.id + 1) }));
		g.addEdge(negIndex(disj.var1), index(disj.var2));
		g.addEdge(negIndex(disj.var2), index(disj.var1));
	}

	std::vector<bool> findAssigment() {
		std::vector<int> sccIndices(g.size(), -1);
		int sccId = 0;
		for (const std::vector<int>& scc : stronglyConnectedComponents(g)) {
			for (int u : scc)
				sccIndices[u] = sccId;
			++sccId;
		}

		std::vector<bool> assignment(g.size() / 2, false);
		for (int i = 0; i < g.size(); i += 2) {
			if (sccIndices[i] == sccIndices[i + 1])
				return {};
			assignment[i / 2] = (sccIndices[i] > sccIndices[i + 1]);
		}
		return assignment;
	}
};