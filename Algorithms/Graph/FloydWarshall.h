#pragma once

#include <vector>

#include "Graph.h"
#include "Constants.h"

template<typename L>
std::tuple<AdjMat<L>, AdjMat<Edge<>>> floydWarshall(AdjMat<L> mat) {
	AdjMat<Edge<>> nextMat{ mat.size() };
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
					nextMat(i, j).to = nextMat(i, k).to;
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
		curr = nextMat(curr, end).to;
		path.push_back(curr);
	}

	return path;
}

template<typename L>
void floydWarshallInfiniteSorthestPaths(AdjMat<L>& mat) {
	for (int k = 0; k < mat.size(); ++k)
		for (int i = 0; i < mat.size(); ++i)
			for (int j = 0; j < mat.size(); ++j)
				if (mat(i, k).w != INF && mat(k, j).w != INF && mat(k, k).w < 0)
					mat(i, j).w = -INF;
}