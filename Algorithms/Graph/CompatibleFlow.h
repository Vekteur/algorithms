#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "MaxFlowEdmondsKarp.h"
#include "Constants.h"

struct WeightDemandLabel {
	int w, demand;
};

template<typename L>
AdjMat<WeightLabel> buildCompatibleFlowGraph(AdjMat<L> cap, int source, int sink, int requestedFlow = INF) {
	int n = cap.size();
	AdjMat<WeightLabel> g(n + 2, { 0 }); // n is the new source, n + 1 is the new sink
	for (int u = 0; u < n; ++u) {
		for (int v = 0; v < n; ++v) {
			g(n, v).w += cap(u, v).demand;
			g(u, n + 1).w += cap(u, v).demand;
			g(u, v).w = cap(u, v).w - cap(u, v).demand;
		}
	}
	g(sink, source).w = requestedFlow;
	return g;
}

// TODO : compatible flow algorithms