#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

#include "Graph/Graph.h"
#include "Constants.h"

// See https://cp-algorithms.com/graph/push-relabel-faster.html
template<typename L>
int maxFlowPushRelabel(const AdjMat<L>& capacity, int source, int sink) {

	int n = capacity.size();
	AdjMat<int> flow(n, 0);
	std::vector<int> height(n, 0), excess(n, 0);

	auto push = [&excess, &capacity, &flow](int u, int v) {
		int diff = std::min(excess[u], capacity(u, v).w - flow(u, v));
		flow(u, v) += diff;
		flow(v, u) -= diff;
		excess[u] -= diff;
		excess[v] += diff;
	};

	auto relabel = [&capacity, &flow, &height, &n](int u) {
		int minHeight = INF;
		for (int v = 0; v < n; ++v) {
			if (flow(u, v) < capacity(u, v).w)
				minHeight = std::min(minHeight, height[v]);
		}
		if (minHeight != INF)
			height[u] = minHeight + 1;
	};

	auto findMaxHeightVertices = [&excess, &height, &source, &sink, &n]() {
		int maxHeight = -1;
		std::vector<int> maxHeights;
		for (int u = 0; u < n; ++u) {
			if (u != source && u != sink && excess[u] > 0) {
				if (height[u] >= maxHeight) {
					if (height[u] > maxHeight) {
						maxHeight = height[u];
						maxHeights.clear();
					}
					maxHeights.push_back(u);
				}
			}
		}
		return maxHeights;
	};

	auto discharge = [&push, &relabel, &capacity, &flow, &excess, &height, &n](int u) {
		bool pushed = false;
		for (int v = 0; v < n && excess[u] > 0; ++v) {
			if (flow(u, v) < capacity(u, v).w && height[u] == height[v] + 1) {
				push(u, v);
				pushed = true;
			}
		}
		if (!pushed)
			relabel(u);
		return pushed;
	};

	height[source] = n;
	excess[source] = INF;
	for (int v = 0; v < n; ++v) {
		if (v != source)
			push(source, v);
	}

	std::vector<int> current;
	while (!(current = findMaxHeightVertices()).empty()) {
		for (int u : current) {
			if (!discharge(u))
				break;
		}
	}

	int maxFlow = 0;
	for (int u = 0; u < n; ++u)
		maxFlow += flow(source, u);
	return maxFlow;
}