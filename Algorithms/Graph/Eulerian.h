#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <cstdio>

using namespace std;

using lit = list<int>::iterator;

const int MAXV = 51, MAXE = 1001;

struct Edge { int to, id; };

vector<Edge> adj[MAXV];
list<int> path;
bool vis_edge[MAXE];
bool vis_node[MAXV];
int n;

bool isEulerian() {
	for (int i = 1; i < MAXV; ++i) {
		if (adj[i].size() & 1 != 0)
			return false;
	}
	return true;
}

void dfs(int u, bool vis[]) {
	if (vis[u])
		return;
	vis[u] = true;

	for (Edge& v : adj[u])
		dfs(v.to, vis);
}

bool isConnected(int u) {
	for (int i = 1; i < MAXV; ++i)
		vis_node[i] = false;
	dfs(u, vis_node);

	for (int i = 1; i < MAXV; ++i) {
		if (adj[i].size() > 0 && !vis_node[i])
			return false;
	}

	return true;
}

void euler(lit it) {
	int u = *it;

	for (int i = 0; i < adj[u].size(); ++i) {
		if (!vis_edge[adj[u][i].id]) {
			vis_edge[adj[u][i].id] = true;

			lit new_it = path.insert(it, adj[u][i].to);
			euler(new_it);
		}
	}
}

void printEulerianCycle(int startNode) {
	if (!isEulerian() || !isConnected(startNode)) {
		printf("No eulerian cycle\n");
	} else {
		lit firstIt = path.insert(path.begin(), startNode);

		euler(firstIt);

		lit last = --path.end();
		for (lit it = path.begin(); it != last;) {
			printf("%d ", *it);
			printf("%d\n", *(++it));
		}
	}
}