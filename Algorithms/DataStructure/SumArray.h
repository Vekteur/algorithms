#pragma once

#include <vector>
#include <numeric>

template<typename T>
class SumArray {
private:
	std::vector<T> acc;
	std::function<T(T, T)> op;
	std::function<T(T, T)> invOp;
public:
	SumArray(const std::vector<T>& a, 
		std::function<T(T, T)> op = [](T a, T b) { return a + b; }, 
		std::function<T(T, T)> invOp = [](T a, T b) { return a - b; }) 
		: acc(a.size()), op(op), invOp(invOp) {

		std::partial_sum(a.begin(), a.end(), acc.begin(), op);
	}

	void update(int k, T v) {
		for (int i = k; i < int(acc.size()); ++i)
			acc[i] = op(acc[i], v);
	}

	T query(int l, int r) {
		if (l == 0)
			return acc[r];
		return invOp(acc[r], acc[l - 1]);
	}
};