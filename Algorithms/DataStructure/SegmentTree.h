#pragma once

#include <vector>
#include <algorithm>
#include <functional>
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
	// [l..r] is the current segment in the segment tree array
	// [sl..sr] is the search interval in the original array
	T rec_query(int p, int l, int r, int sl, int sr) {
		if (sl <= l && r <= sr) {
			return st[p];
		} else {
			int m = (l + r) / 2;
			if (sr <= m)
				return rec_query(2 * p + 1, l, m, sl, sr);
			else if (sl > m)
				return rec_query(2 * p + 2, m + 1, r, sl, sr);
			else
				return combine(rec_query(2 * p + 1, l, m, sl, sr),
					rec_query(2 * p + 2, m + 1, r, sl, sr));
		}
	}

	// s is the index in the original array of the value that will be updated
	void rec_update(int p, int l, int r, int s, T v) {
		if (l == r) {
			st[p] = v;
		} else if (l <= s && s <= r) {
			int m = (l + r) / 2;
			if (s <= m)
				rec_update(2 * p + 1, l, m, s, v);
			else
				rec_update(2 * p + 2, m + 1, r, s, v);
			// A node that is not a leaf has always two valid children
			st[p] = combine(st[2 * p + 1], st[2 * p + 2]);
		}
	}

public:
	SegmentTree() {}
	SegmentTree(const std::vector<T>& a, std::function<T(T, T)> combine = [](T c1, T c2) { return min(c1, c2); })
		: n{ int(a.size()) }, combine{ combine } {

		st.assign(n * 4, 0); // Could be reduced to 2 * n ?
		rec_build(0, 0, n - 1, a);
	}

	T query(int sl, int sr) {
		return rec_query(0, 0, n - 1, sl, sr);
	}

	void update(int s, T v) {
		rec_update(0, 0, n - 1, s, v);
	}

	friend std::ostream& operator<<(std::ostream& out, const SegmentTree& seg) {
		int index = 0;
		for (int iter = 1; index + iter - 1 < seg.st.size(); iter <<= 1) {
			for (int i = 0; i < iter; ++i)
				out << seg.st[index + i] << ' ';
			out << '\n';
			index += iter;
		}
		out << '\n';
		return out;
	}
};