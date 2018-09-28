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

		logs[0] = -1;
		for (int k = 1; k <= int(a.size()); ++k)
			logs[k] = logs[k >> 1] + 1;

		int maxLog = logs[int(a.size())];
		st = std::vector<std::vector<T>>(a.size(), std::vector<T>(maxLog + 1));

		for (int i = 0; i < int(st.size()); ++i)
			st[i][0] = a[i];

		// The range [i, i + 2^d[ splits in [i, i + 2^(d - 1)[ and [i + 2^(d - 1), i + 2^d[
		for (int d = 1; d <= maxLog; ++d) {
			// i + 2^d - 1 (last index of the sequence) must always be valid
			for (int i = 0; i + (1 << d) - 1 < int(st.size()); ++i)
				st[i][d] = f(st[i][d - 1], st[i + (1 << (d - 1))][d - 1]);
		}
	}

	T query(int l, int r) {
		int d = logs[r - l + 1];
		return f(st[l][d], st[r - (1 << d) + 1][d]);
	}

	void update(int k, T v) {
		for (int d = 0; d <= logs[int(st.size())]; ++d)
			for (int i = max(0, k - (1 << d) + 1); i < min(int(st.size()) - (1 << d) + 1, k + 1); ++i)
				st[i][d] = f(st[i][d], v);
	}
};