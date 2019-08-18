#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
class SparseTable {
private:
	int n;
	std::vector<int> logs;
	// st[i][j] stores the range query result of [i, i + 2^j[
	std::vector<std::vector<T>> st;
	std::function<T(T, T)> f;
public:
	SparseTable(const std::vector<T>& a, 
		std::function<T(T, T)> f = [](T a, T b) { return std::min(a, b); })
		: n(int(a.size())), logs(n + 1), f{ f } {

		logs[0] = -1;
		for (int k = 1; k <= n; ++k)
			logs[k] = logs[k >> 1] + 1;

		int maxLog = logs[n];
		st.assign(maxLog + 1, std::vector<T>(a.size()));

		for (int i = 0; i < n; ++i)
			st[0][i] = a[i];

		// The range [i, i + 2^d[ splits in [i, i + 2^(d - 1)[ and [i + 2^(d - 1), i + 2^d[
		for (int d = 1; d <= maxLog; ++d) {
			// i + 2^d - 1 (last index of the sequence) must always be valid
			for (int i = 0; i + (1 << d) - 1 < n; ++i)
				st[d][i] = f(st[d - 1][i], st[d - 1][i + (1 << (d - 1))]);
		}
	}

	T query(int l, int r) {
		int d = logs[r - l + 1];
		return f(st[d][l], st[d][r - (1 << d) + 1]);
	}

	void update(int k, T v) {
		for (int d = 0; d <= logs[n]; ++d)
			for (int i = max(0, k - (1 << d) + 1); i < min(n - (1 << d) + 1, k + 1); ++i)
				st[d][i] = f(st[d][i], v);
	}
};