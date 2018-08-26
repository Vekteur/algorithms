#pragma once

#include <vector>

template<typename T>
class FenwickTree {
private:
	std::vector<T> ft;
	int m_size;

	int LSOne(int i) {
		return i & (-i);
	}

public:
	FenwickTree(int size = 0) : m_size{ size } {
		ft.assign(m_size + 1, 0);
	}

	T rsqFromStart(int i) {
		T sum = 0;
		for (++i; i > 0; i -= LSOne(i))
			sum += ft[i];
		return sum;
	}

	T rsq(int i, int j) {
		return rsqFromStart(j) - rsqFromStart(i - 1);
	}

	void update(int i, T v) {
		for (++i; i < (int)ft.size(); i += LSOne(i))
			ft[i] += v;
	}
};