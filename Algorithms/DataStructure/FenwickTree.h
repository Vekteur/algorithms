#pragma once

#include <vector>

template<typename T>
class FenwickTree {
private:
	std::vector<T> ft;
	int size;

	int lsOne(int i) {
		return i & (-i);
	}

public:
	FenwickTree(int size = 0) : size{ size } {
		ft.assign(size + 1, 0);
	}

	T rsqFromStart(int i) {
		T sum = 0;
		for (++i; i > 0; i -= lsOne(i))
			sum += ft[i];
		return sum;
	}

	T rsq(int i, int j) {
		return rsqFromStart(j) - rsqFromStart(i - 1);
	}

	void update(int i, T v) {
		for (++i; i < int(ft.size()); i += lsOne(i))
			ft[i] += v;
	}
};