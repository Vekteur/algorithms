#pragma once

#include <vector>
#include <queue>
#include <algorithm>

#include "Constants.h"

namespace {
struct KnapsackObject { 
	int w; // Weight
	int v; // Value
};

int knapsack(const std::vector<KnapsackObject>& objs, int cap) {
	std::vector<int> values(cap + 1, -INF);
	values[0] = 0;
	for (auto obj : objs)
		for (int c = cap; c - obj.w >= 0; --c)
			if (values[c - obj.w] != -INF)
				values[c] = std::max(values[c], values[c - obj.w] + obj.v);
	return *std::max_element(values.begin(), values.end());
}

struct KnapsackObjects : KnapsackObject {
	int m = 1; // Multiplicity
	KnapsackObjects(int w, int v, int m) : KnapsackObject{w, v}, m(m) {}
};

int multiplicityKnapsack(const std::vector<KnapsackObjects>& objs, int cap) {
	std::vector<int> values(cap + 1, -INF), newValues(cap + 1);
	values[0] = 0;
	for (auto obj : objs) {
		for (int i = 0; i < obj.w; ++i) {
			auto idx = [&obj, &i](int k) {
				return k * obj.w + i;
			};
			std::deque<int> maxValueIds;
			for (int k = 0; idx(k) <= cap; ++k) {
				int c = idx(k);
				if (!maxValueIds.empty() && maxValueIds.front() < k - obj.m)
					maxValueIds.pop_front();
				while (!maxValueIds.empty() && values[idx(maxValueIds.back())] +
						(k - maxValueIds.back()) * obj.v <= values[c])
					maxValueIds.pop_back();
				maxValueIds.push_back(k);
				bool unreachable = values[idx(maxValueIds.front())] == -INF;
				newValues[c] = unreachable ? -INF : values[idx(maxValueIds.front())] +
						(k - maxValueIds.front()) * obj.v;
			}
		}
		values = newValues;
	}
	return *std::max_element(values.begin(), values.end());
}
}