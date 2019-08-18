#pragma once

#include <vector>
#include <queue>

namespace {
std::vector<int> slidingWindowMinimum(std::vector<int> a, int size) {
	int n = a.size();
	std::vector<int> slidingMin(n - size + 1);
	std::deque<int> q;
	for (int i = 0; i < n; ++i) {
		if (!q.empty() && q.front() <= i - size)
			q.pop_front();
		while (!q.empty() && a[q.back()] >= a[i])
			q.pop_back();
		q.push_back(i);
		if (i + 1 >= size)
			slidingMin[i + 1 - size] = q.front();
	}
	return slidingMin;
}
}