#pragma once

#include <vector>
#include <numeric>
#include <functional>
#include <cmath>

template<typename T, typename M>
class SqrtArray {
private:
	std::vector<T> a;
	std::vector<T> blocks; // Value associated to all elements in a block
	std::vector<M> multBlocks; // Value associated to each individual element in a block
	std::function<void(const T&, T&)> combine; // Operation between two values in a
	std::function<void(const M&, M&)> mergeMult; // Merge two values in multBlocks
	std::function<void(const M&, int, T&)> combineMult; // Modify the T value in function of the number of M values 
	int blockSize, blockCount;

	void updateElements(int l, int r, int block, const M& v) {
		for (int i = l; i <= r; ++i)
			combineMult(v, 1, a[i]);
		combineMult(v, r - l + 1, blocks[block]);
	}

	void updateBlock(int block, const M& v) {
		mergeMult(v, multBlocks[block]);
	}

	void queryElements(int l, int r, int block, T& value) {
		for (int i = l; i <= r; ++i)
			combine(a[i], value);
		combineMult(multBlocks[block], r - l + 1, value);
	}

	void queryBlock(int block, T& value) {
		combine(blocks[block], value);
		combineMult(multBlocks[block], blockSize, value);
	}

public:
	SqrtArray(const std::vector<T>& a,
		std::function<void(const T&, T&)> combine = [](const T& t1, T& t2) { return t2 += t1; },
		std::function<void(const M&, M&)> mergeMult = [](const M& l1, M& l2) { l2 += l1; },
		std::function<void(const M&, int, T&)> combineMult = [](const M& l, int range, T& t) { t += range * l; },
		M defaultM = M())
		: a(a), combine(combine), mergeMult(mergeMult), combineMult(combineMult) {

		blockSize = int(sqrt(int(a.size())));
		blockCount = (int(a.size()) - 1) / blockSize + 1;
		multBlocks = std::vector<M>(blockCount, defaultM);
		blocks = std::vector<T>(blockCount);

		for (int block = 0; block < blockCount; ++block) {
			int blockStart = block * blockSize;
			blocks[block] = a[blockStart];
			for (int i = 1; i < blockSize && blockStart + i < int(a.size()); ++i) {
				combine(a[blockStart + i], blocks[block]);
			}
		}
	}

	void update(int l, int r, const M& v) {
		int lBlock = l / blockSize, rBlock = r / blockSize;
		if (lBlock == rBlock) {
			updateElements(l, r, lBlock, v);
		} else {
			updateElements(l, (lBlock + 1) * blockSize - 1, lBlock, v);
			updateElements(rBlock * blockSize, r, rBlock, v);
			for (int block = lBlock + 1; block < rBlock; ++block)
				updateBlock(block, v);
		}
	}

	T query(int l, int r) {
		int lBlock = l / blockSize, rBlock = r / blockSize;
		T value = a[l];
		combineMult(multBlocks[lBlock], 1, value);
		if (lBlock == rBlock) {
			queryElements(l + 1, r, lBlock, value);
		} else {
			queryElements(l + 1, (lBlock + 1) * blockSize - 1, lBlock, value);
			queryElements(rBlock * blockSize, r, rBlock, value);
			for (int block = lBlock + 1; block < rBlock; ++block)
				queryBlock(block, value);
		}
		return value;
	}
};