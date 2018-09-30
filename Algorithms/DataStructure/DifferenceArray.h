#pragma once

#include <vector>
#include <numeric>

template<typename T>
class DifferenceArray {
private:
	std::vector<T> diff;
	std::function<T(T, T)> op;
	std::function<T(T, T)> invOp;
public:
	DifferenceArray(const std::vector<T>& a,
		std::function<T(T, T)> op = [](T a, T b) { return a + b; },
		std::function<T(T, T)> invOp = [](T a, T b) { return a - b; })
		: diff(a.size()), op(op), invOp(invOp) {

		std::adjacent_difference(a.begin(), a.end(), diff.begin(), invOp);
	}

	void update(int i, int j, T v) {
		diff[i] = op(diff[i], v);
		if (j + 1 != int(diff.size()))
			diff[j + 1] = invOp(diff[j + 1], v);
	}

	T query(int i) {
		return std::accumulate(diff.begin() + 1, diff.begin() + i + 1, *diff.begin(), op);
	}
};