#pragma once

#include <vector>
#include <numeric>

template<typename T>
class DifferenceArray {
private:
	std::vector<T> diff;
public:
	DifferenceArray(const std::vector<T>& a) : diff(a.size()) {
		std::adjacent_difference(a.begin(), a.end(), diff.begin());
	}

	void update(int i, int j, T v) {
		diff[i] += v;
		if (j + 1 != int(diff.size()))
			diff[j + 1] -= v;
	}

	T query(int i) {
		return std::accumulate(diff.begin(), diff.begin() + i + 1, 0);
	}
};