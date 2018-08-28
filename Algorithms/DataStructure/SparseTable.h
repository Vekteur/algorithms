#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
class SparseTable {
private:
	std::vector<int> logs;
	// st[i][j] stores the range query result of [i, i + 2^j[
	std::vector<std::vector<T>> st;
	std::function<T(T, T)> f;
public:
	SparseTable(const std::vector<T>& a, 
		std::function<T(T, T)> f = [](T a, T b) { return std::min(a, b); })
		: logs(a.size() + 1), f{ f } {

		logs[1] = 0;
		for (int i = 2; i <= a.size(); ++i)
			logs[i] = logs[i / 2] + 1;

		double maxLog = logs[a.size()];
		st = std::vector<std::vector<T>>(a.size(), std::vector<T>(maxLog + 1));

		for (int i = 0; i < st.size(); ++i)
			st[i][0] = a[i];

		// The range [i, i + 2^j[ splits in [i, i + 2^(j - 1)[ and [i + 2^(j - 1), i + 2^j[
		for (int j = 1; j <= maxLog; ++j)
			for (int i = 0; i + (1 << j) <= st.size(); ++i)
				st[i][j] = f(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
	}

	T query(int l, int r) {
		int j = logs[r - l + 1];
		return f(st[l][j], st[r - (1 << j) + 1][j]);
	}
};