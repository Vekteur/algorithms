#pragma once

#include <vector>

#include "Graph.h"
#include "Constants.h"

struct NextLabel {
	int next;
};

template<typename L>
std::tuple<AdjMat<L>, AdjMat<int>> floydWarshall(AdjMat<L> mat) {
	AdjMat<int> nextMat{ mat.size() };
	for (int i = 0; i < mat.size(); ++i)
		for (int j = 0; j < mat.size(); ++j)
			nextMat(i, j) = j;
	for (int i = 0; i < mat.size(); ++i)
		mat(i, i).w = 0;

	for (int k = 0; k < mat.size(); ++k) {
		for (int i = 0; i < mat.size(); ++i) {
			for (int j = 0; j < mat.size(); ++j) {
				if (mat(i, k).w != INF && mat(k, j).w != INF && mat(i, k).w + mat(k, j).w < mat(i, j).w) {
					mat(i, j).w = mat(i, k).w + mat(k, j).w;
					nextMat(i, j) = nextMat(i, k);
				}
			}
		}
	}
	return { mat, nextMat };
}

template<typename L>
std::vector<int> floydWarshallPath(const AdjMat<L>& nextMat, int start, int end) {
	std::vector<int> path;

	int curr = start;
	while (curr != end) {
		curr = nextMat(curr, end);
		path.push_back(curr);
	}

	return path;
}