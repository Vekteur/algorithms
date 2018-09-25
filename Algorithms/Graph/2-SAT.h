#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Graph.h"
#include "ConnectedComponents.h"

class TwoSAT {
private:
	AdjList<> g{ 0 };
public:
	struct Variable { int id; bool pos; };
	struct Disjunction { Variable var1, var2; };
private:
	int index(Variable var) {
		return 2 * var.id + 1 - static_cast<int>(var.pos);
	}

	int negIndex(Variable var) {
		return 2 * var.id + static_cast<int>(var.pos);
	}
public:
	void addDisjunction(Disjunction disj) {
		g.adj.resize(std::max(int(g.adj.size()),
			std::max(2 * (disj.var1.id + 1), 2 * (disj.var2.id + 1))));

		g.addEdge(negIndex(disj.var1), index(disj.var2));
		g.addEdge(negIndex(disj.var2), index(disj.var1));
	}

	std::vector<bool> findAssigment() {
		AdjList<> gInv = invertAdjList(g);
		std::vector<int> SCCindices(g.size(), -1);

		int sccId = 0;
		for (const std::vector<int>& scc : stronglyConnectedComponents(g)) {
			for (int u : scc)
				SCCindices[u] = sccId;
			++sccId;
		}

		std::vector<bool> assignment(g.size() / 2, false);
		for (int i = 0; i < g.size(); i += 2) {
			if (SCCindices[i] == SCCindices[i + 1])
				return {};
			assignment[i / 2] = (SCCindices[i] > SCCindices[i + 1]);
		}
		return assignment;
	}
};