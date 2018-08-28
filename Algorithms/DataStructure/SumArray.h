#pragma once

#include <vector>
#include <numeric>

template<typename T>
class SumArray {
private: 
	std::vector<T> acc;
public:
	SumArray(const std::vector<T>& a) : acc(a.size()) {
		std::partial_sum(a.begin(), a.end(), acc.begin());
	}

	T query(int l, int r) {
		return acc[r] - (l == 0 ? 0 : acc[l - 1]);
	}
};