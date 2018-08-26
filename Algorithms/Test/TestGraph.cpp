#include "catch.hpp"

#include <algorithm>
#include <functional>

#include "Graph/Graph.h"
#include "TestUtil.h"

#include "Graph/BasicGraph.h"
#include "Graph/BellmanFord.h"
#include "Graph/BridgeAndArticulation.h"
#include "Graph/ConnectedComponents.h"
#include "Graph/CycleDetection.h"
#include "Graph/Dijkstra.h"
#include "Graph/Eulerian.h"
#include "Graph/FloydWarshall.h"
#include "Graph/Kruskal.h"
#include "Graph/MaxFlow.h"
#include "Graph/Prim.h"
#include "Graph/Toposort.h"

using namespace std;

TEST_CASE("Graph") {
	AdjList<WeightLabel> g(4);
	vector<BiEdge<WeightLabel>> edges{ {0, 1, {1}}, {0, 2, {3}}, {1, 2, {1}}, {2, 3, {2}}, {3, 1, {1}} };
	SECTION("Directed") {
		for (auto edge : edges) {
			g.addEdge(edge.from, edge.to, { edge.w });
		}
		SECTION("Basic") {
			REQUIRE(dfs_stack(g).size() == 4);
			REQUIRE(bfs(g).size() == 4);
		}
		SECTION("Cycle detection") {
			REQUIRE(hasCycle(g));
			g.adj[1].clear();
			REQUIRE(!hasCycle(g));
		}
		SECTION("Dijkstra") {
			vector<int> minDist, pred;
			tie(minDist, pred) = dijkstra(g, 0);
			REQUIRE(minDist[3] == 4);
			REQUIRE(pred[3] == 2);
			REQUIRE(dijkstraBacktrack(pred, 0, 3).size() == 3);
		}
		SECTION("Bellman-Ford") {
			vector<int> minDist, pred;
			tie(minDist, pred) = bellmanFordDistances(g, 0);
			REQUIRE(minDist[3] == 4);
			REQUIRE(pred[3] == 2);
			REQUIRE(bellmanFordBacktrack(pred, 0, 3).size() == 3);
			REQUIRE(bellmanFordCheckCycle(g, minDist) == -1);
			g.adj[1].clear();
			g.addEdge(1, 2, { -4 });
			tie(minDist, pred) = bellmanFordDistances(g, 0);
			REQUIRE(bellmanFordCheckCycle(g, minDist) != -1);
		}
		SECTION("Floyd-Warshall") {
			AdjMat<WeightLabel> mat(4, { INF });
			for (auto edge : edges) {
				mat.addEdge(edge.from, edge.to, { edge.w });
			}
			AdjMat<WeightLabel> minDist;
			AdjMat<int> next;
			tie(minDist, next) = floydWarshall(mat);
			REQUIRE(minDist(0, 3).w == 4);
			REQUIRE(floydWarshallPath(next, 0, 3).size() == 3);
		}
		SECTION("Eulerian") {
			REQUIRE(!containsEulerianPath(g));
			g.removeEdge(0, 2);
			g.addEdge(2, 0);
			REQUIRE(containsEulerianPath(g));
			REQUIRE(!containsEulerianCycle(g));
			struct VisLabel {
				bool vis = false;
			};
			AdjList<VisLabel> gVis(g.size());
			for (int u = 0; u < g.size(); ++u)
				for (auto v : g.adj[u])
					gVis.addEdge(u, v.to, { false });
			REQUIRE(findEulerianPath(gVis, 2).size() == 6);
		}
		SECTION("Toposort") {
			g.removeEdge(1, 2);
			g.addEdge(2, 1, { 0 }); // Make acyclic graph
			REQUIRE(toposortInDegree(g).size() == 4);
			REQUIRE(toposortDfs(g).size() == 4);
		}
		SECTION("Strongly connected components") {
			REQUIRE(SCCKosaraju(g).size() == 2);
		}
		SECTION("Max flow") {
			MappedAdjList<WeightLabel> mappedG(edges.size());
			for (auto edge : edges)
				mappedG.addEdge(edge.from, edge.to, { edge.w });
			REQUIRE(maxflowEK(mappedG, 0, 3) == 2);
			mappedG.removeEdge(2, 3);
			mappedG.addEdge(2, 3, { 5 });
			REQUIRE(maxflowEK(mappedG, 0, 3) == 4);
		}
	}

	SECTION("Undirected") {
		for (auto edge : edges) {
			g.addEdge(edge.from, edge.to, { edge.w });
			g.addEdge(edge.to, edge.from, { edge.w });
		}
		SECTION("Prim") {
			vector<int> pred = prim(g);
			vector<int> weights;
			for (int u = 0; u < (int) pred.size(); ++u)
				if (pred[u] != -1)
					weights.push_back(g(pred[u], u).w);
			REQUIRE(*max_element(weights.begin(), weights.end()) == 1);
		}
		SECTION("Kruskal") {
			EdgeList<WeightLabel> list;
			for (auto edge : edges) {
				list.addEdge(edge.from, edge.to, { edge.w });
			}
			REQUIRE(kruskal(list).size() == 3);
		}
		SECTION("Connected components") {
			REQUIRE(isConnected(g));
			g.removeEdge(0, 1);
			g.removeEdge(1, 0);
			g.removeEdge(0, 2);
			g.removeEdge(2, 0); // Isolate 0
			REQUIRE(!isConnected(g));
			REQUIRE(connectedComponents(g).size() == 2);
		}
		SECTION("Bridges and articulations") {
			g.removeEdge(0, 1);
			g.removeEdge(1, 0);
			std::vector<int> articulationPoints;
			std::vector<BiEdge<>> bridges;
			tie(articulationPoints, bridges) = bridgesAndArticulationPoints(g, 0);
			REQUIRE(articulationPoints.size() == 1);
			REQUIRE(articulationPoints[0] == 2);
			REQUIRE(bridges.size() == 1);
			REQUIRE(bridges[0].from == 0);
			REQUIRE(bridges[0].to == 2);
		}
	}
}