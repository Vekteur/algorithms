#pragma once

#include <vector>
#include <functional>
#include <cmath>

#include "Graph.h"
#include "DataStructure/SparseTable.h"

class LcaRangeQuery {
private:
	std::vector<int> heights;
	std::vector<int> pre;
	SparseTable<int> st;
	std::vector<int> euler;

public:
	template<typename L>
	LcaRangeQuery(const AdjList<L>& g, int root) : st({}) {
		heights.resize(g.size());
		pre.resize(g.size());
		euler.resize(2 * g.size() - 1);
		int index = 0;
		std::function<void(int, int)> dfs = [&dfs, &g, &index, this](int u, int height) {
			heights[u] = height;
			pre[u] = index;
			euler[index++] = u;
			for (Edge<L> v : g.adj[u]) {
				dfs(v.to, height + 1);
				euler[index++] = u;
			}
		};
		dfs(root, 0);

		st = SparseTable<int>(euler, [this](int a, int b) { return heights[a] < heights[b] ? a : b; });
	}

	int query(int a, int b) {
		if (pre[a] > pre[b])
			std::swap(a, b);
		return st.query(pre[a], pre[b]);
	}
};

// Farach-Colton and Bender Algorithm
class LcaRangeQueryOptimized {
private:
	std::vector<int> heights;
	std::vector<int> pre;
	std::vector<int> euler;
	SparseTable<int> st;
	int blockSize, blockCount;
	std::vector<int> blockMask;
	std::vector<std::vector<std::vector<int>>> blocks;

	int minHeight(int a, int b) {
		return heights[a] < heights[b] ? a : b;
	}

	void buildSparseTable() {
		std::vector<int> eulerBlocks(blockCount);
		for (int block = 0; block < blockCount; ++block) {
			int blockStart = block * blockSize;
			eulerBlocks[block] = euler[blockStart];
			for (int i = 1; i < blockSize && blockStart + i < int(euler.size()); ++i) {
				eulerBlocks[block] = minHeight(eulerBlocks[block], euler[blockStart + i]);
			}
		}
		st = SparseTable<int>(eulerBlocks, [this](int a, int b) { return minHeight(a, b); });
	}

	void buildMasks() {

		blockMask.resize(blockCount);
		for (int block = 0; block < blockCount; ++block) {
			int mask = 0;
			int blockStart = block * blockSize;
			for (int i = 0; i < blockSize - 1; ++i) {
				if (blockStart + i >= blockCount || // Last block
					heights[euler[blockStart + i + 1]] - heights[euler[blockStart + i]] == 1) {
					
					mask += (1 << i);
				}
			}
			blockMask[block] = mask;
		}

		blocks.assign(1 << (blockSize - 1), std::vector<std::vector<int>>(blockSize, std::vector<int>(blockSize)));
		for (int mask = 0; mask < (1 << (blockSize - 1)); ++mask) {
			for (int l = 0; l < blockSize; ++l) {
				int value = 0;
				int minValue = value;
				int minIndex = l;
				blocks[mask][l][l] = minIndex;
				for (int r = l + 1; r < blockSize; ++r) {
					value += (mask & (1 << (r - 1))) ? 1 : -1;
					if (value < minValue) {
						minValue = value;
						minIndex = r;
					}
					blocks[mask][l][r] = minIndex;
				}
			}
		}
	}

	int minQueryInBlock(int block, int l, int r) {
		return blocks[blockMask[block]][l][r] + block * blockSize;
	}

public:
	template<typename L>
	LcaRangeQueryOptimized(const AdjList<L>& g, int root) : st({}) {
		heights.resize(g.size());
		pre.resize(g.size());
		euler.resize(2 * g.size() - 1);
		int index = 0;
		std::function<void(int, int)> dfs = [&dfs, &g, &index, this](int u, int height) {
			heights[u] = height;
			pre[u] = index;
			euler[index++] = u;
			for (Edge<L> v : g.adj[u]) {
				dfs(v.to, height + 1);
				euler[index++] = u;
			}
		};
		dfs(root, 0);
		blockSize = std::max(1, int(log2(euler.size()) / 2));
		blockCount = (int(euler.size()) - 1) / blockSize + 1;

		buildSparseTable();
		buildMasks();
	}

	int query(int a, int b) {
		if (pre[a] > pre[b])
			std::swap(a, b);
		int l = pre[a], r = pre[b];
		int lBlock = l / blockSize, rBlock = r / blockSize;
		int lInBlock = l % blockSize, rInBlock = r % blockSize;
		if (lBlock == rBlock) {
			return euler[minQueryInBlock(lBlock, lInBlock, rInBlock)];
		} else {
			int prefixMin = euler[minQueryInBlock(lBlock, lInBlock, blockSize - 1)];
			int suffixMin = euler[minQueryInBlock(rBlock, 0, rInBlock)];
			int lca = minHeight(prefixMin, suffixMin);
			if (lBlock + 1 < rBlock)
				lca = minHeight(lca, st.query(lBlock + 1, rBlock - 1));
			return lca;
		}
	}
};