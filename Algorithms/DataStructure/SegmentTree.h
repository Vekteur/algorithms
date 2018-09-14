#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T>
class SegmentTree {
private:
	int n;
	std::function<T(T, T)> combine;
	std::vector<T> st;

	void rec_build(int p, int l, int r, const std::vector<T>& a) {
		if (l == r) {
			st[p] = a[l];
		} else {
			int m = (l + r) / 2;
			rec_build(2 * p + 1, l, m, a);
			rec_build(2 * p + 2, m + 1, r, a);
			st[p] = combine(st[2 * p + 1], st[2 * p + 2]);
		}
	}

	// p is the index of the current node
	// [l..r] is the current segment
	// [i..j] is the search interval
	T rec_query(int p, int l, int r, int i, int j) {
		if (i <= l && r <= j)
			return st[p];
		int m = (l + r) / 2;
		T c1, c2;
		if (i <= m)
			c1 = rec_query(2 * p + 1, l, m, i, j);
		if (j > m)
			c2 = rec_query(2 * p + 2, m + 1, r, i, j);
		if (j <= m)
			return c1;
		else if (i > m)
			return c2;
		else
			return combine(c1, c2);
	}

	// i is the node that will be updated
	void rec_update(int p, int l, int r, int i, int v) {
		if (l == r) {
			st[p] = v;
		} else if (l <= i && i <= r) {
			int m = (l + r) / 2;
			if (i <= m)
				rec_update(2 * p + 1, l, m, i, v);
			else
				rec_update(2 * p + 2, m + 1, r, i, v);
			st[p] = combine(st[2 * p + 1], st[2 * p + 2]);
		}
	}

public:
	SegmentTree(const std::vector<T>& a, std::function<T(T, T)> combine = [](T c1, T c2) { return min(c1, c2); })
		: n{ int(a.size()) }, combine{ combine } {

		st.assign(n * 4, 0);
		rec_build(0, 0, n - 1, a);
	}

	T query(int i, int j) {
		return rec_query(0, 0, n - 1, i, j);
	}

	void update(int i, int v) {
		rec_update(0, 0, n - 1, i, v);
	}

	friend std::ostream& operator<<(std::ostream& out, const SegmentTree& s) {
		int index = 0;
		for (int iter = 1; index + iter - 1 < s.st.size(); iter <<= 1) {
			for (int i = 0; i < iter; ++i)
				out << s.st[index + i] << ' ';
			out << '\n';
			index += iter;
		}
		out << '\n';
		return out;
	}
};