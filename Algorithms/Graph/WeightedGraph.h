#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <bitset>
#include <utility>
#include <algorithm>
#include <functional>

using namespace std;

using pii = pair<int, int>;

struct Edge { int to, weight; };

const int INF{ numeric_limits<int>::max() };
const int MAXN{ 10'000 };
std::vector<Edge> adj[MAXN];

// Matrices for Floyd-Warshall
int adjMat[MAXN][MAXN];
int nextMat[MAXN][MAXN];

int dijkstra(int a, int b) {
	int minDist[MAXN];
	for (int i = 0; i < MAXN; ++i) {
		minDist[i] = -1;
	}

	struct Comp {
		bool operator()(const Edge& a, const Edge& b) { return a.weight > b.weight; }
	};
	std::priority_queue < Edge, std::vector<Edge>, Comp > pq;
	pq.push({ a, 0 });

	while (!pq.empty()) {
		Edge top = pq.top(); pq.pop();
		int u = top.to;

		if (u == b)
			return top.weight;

		if (minDist[u] == -1) {
			minDist[u] = top.weight; // The unvisited nodes are at distance -1
			for (Edge v : adj[u]) {
				int dist = v.weight + top.weight;
				if (minDist[v.to] == -1) // If not already visited (optional but faster)
					pq.push({ v.to, dist });
			}
		}
	}
	return -1;
}

vector<pii> bellmanFordDistances(int n, int startNode) {
	vector<pii> minDist(n);

	for (int i = 0; i < n; i++) {
		minDist[i].first = numeric_limits<int>::max(); // Shortest distance
		minDist[i].second = -1; // Parent node
	}

	// Init
	minDist[startNode].first = 0;

	for (int i = 0; i < n - 1; i++) { // n - 1 times
		for (int j = 0; j < n; j++) { // For each edge
			for (Edge v : adj[j]) {
				// Pass if not visited
				if (minDist[j].first == numeric_limits<int>::max())
					continue;

				// Relaxation
				if (v.weight + minDist[j].first < minDist[v.to].first) {
					minDist[v.to].first = v.weight + minDist[j].first;
					minDist[v.to].second = j;
				}
			}
		}
	}
	return minDist;
}

int bellmanFordCheckCycle(int n, const std::vector<pii>& minDist) {
	for (int j = 0; j < n; j++) { // For each edge
		for (Edge v : adj[j]) {
			// Pass if not visited
			if (minDist[j].first == numeric_limits<int>::max())
				continue;

			// Return the node that creates a cycle
			if (v.weight + minDist[j].first < minDist[v.to].first)
				return j;
		}
	}
	return -1;
}

std::vector<int> bellmanFordBacktrack(int n, const std::vector<pii>& minDist, int endNode) {
	vector<int> nodes;

	int node = endNode;
	while (node != -1) // When returned to start node
	{
		node = minDist[node].second;
		nodes.push_back(node);

		if (node == endNode) // Backtrack cycle
			return nodes;
	}
	return nodes;
}

void floydWarshall(int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			adjMat[i][j] = (i == j) ? 0 : INF;
			nextMat[i][j] = j;
		}
	}

	for (int k = 0; k < n; ++k) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (adjMat[i][k] != INF && adjMat[k][j] != INF && adjMat[i][j] > adjMat[i][k] + adjMat[k][j]) {
					adjMat[i][j] = adjMat[i][k] + adjMat[k][j];
					nextMat[i][j] = nextMat[i][k];
				}
			}
		}
	}
}

vector<int> getFloydWarshallPath(int a, int b) {
	vector<int> path;

	int curr = a;
	while (curr != b) {
		curr = nextMat[curr][b];
		path.push_back(curr);
	}

	return path;
}