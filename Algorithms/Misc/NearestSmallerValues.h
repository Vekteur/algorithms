#pragma once

#include <vector>
#include <stack>

std::vector<int> nearestSmallerValues(const std::vector<int>& a) {
	int n = a.size();
	std::vector<int> nearest(n, -1);
	std::stack<int> st;
	for (int i = 0; i < n; ++i) {
		while (!st.empty() && a[st.top()] >= a[i])
			st.pop();
		if (!st.empty())
			nearest[i] = st.top();
		st.push(i);
	}
	return nearest;
}