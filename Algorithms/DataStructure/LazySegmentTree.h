#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T, typename L>
class LazySegmentTree {
private:
	int n;
	std::function<T(const T&, const T&)> combine;
	std::function<void(const L&, L&)> propagate;
	std::function<T(const T&, const L&, int)> combineLazy;
	L defaultLazy;
	std::vector<T> st;
	std::vector<L> lazy;

	void propagateHelper(int p, int l, int r) {
		st[p] = combineLazyHelper(p, l, r);
		if (l != r) {
			propagate(lazy[p], lazy[2 * p + 1]);
			propagate(lazy[p], lazy[2 * p + 2]);
		}
		lazy[p] = defaultLazy;
	}

	T combineLazyHelper(int p, int l, int r) {
		return combineLazy(st[p], lazy[p], r - l + 1);
	}

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

	T rec_query(int p, int l, int r, int sl, int sr) {
		propagateHelper(p, l, r);
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

	void rec_update(int p, int l, int r, int sl, int sr, L v) {
		propagateHelper(p, l, r);
		if (sl <= l && r <= sr) {
			lazy[p] = v;
			propagateHelper(p, l, r);
		} else {
			int m = (l + r) / 2;
			if (sl <= m)
				rec_update(2 * p + 1, l, m, sl, sr, v);
			if (sr > m)
				rec_update(2 * p + 2, m + 1, r, sl, sr, v);
			st[p] = combine(st[2 * p + 1], st[2 * p + 2]);
		}
	}

public:
	LazySegmentTree() {}
	LazySegmentTree(const std::vector<T>& a,
		std::function<T(const T&, const T&)> combine = [](const T& c1, const T& c2) { return min(c1, c2); },
		std::function<void(const L&, L&)> propagate = [](const L& p, L& c) {},
		std::function<T(const T&, const L&, int)> combineLazy = [](const T& t, const L& l, int range) { return t; },
		L defaultLazy = L())
		: n{ int(a.size()) }, combine{ combine }, propagate{ propagate }, combineLazy{ combineLazy }, defaultLazy{ defaultLazy } {

		st.assign(n * 4, T());
		lazy.assign(n * 4, defaultLazy);
		rec_build(0, 0, n - 1, a);
	}

	T query(int sl, int sr) {
		return rec_query(0, 0, n - 1, sl, sr);
	}

	void update(int sl, int sr, L v) {
		rec_update(0, 0, n - 1, sl, sr, v);
	}
};