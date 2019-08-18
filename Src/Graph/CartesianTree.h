#pragma once

#include <vector>
#include <stack>

#include "Graph.h"
#include "Tree.h"

#include <functional>

template<typename T>
AdjList<> buildCartesianTree(const std::vector<T>& a,
		std::function<bool(T, T)> comp = std::less<T>()) {

	std::vector<int> preds(a.size(), -1);
	std::stack<int> s;
	for (int i = 0; i < int(a.size()); ++i) {
		int last = -1;
		while (!s.empty() && comp(a[i], a[s.top()])) {
			last = s.top(); s.pop();
		}
		if (!s.empty())
			preds[i] = s.top();
		if (last != -1)
			preds[last] = i;
		s.push(i);
	}
	return predecessorsToTree(preds);
}