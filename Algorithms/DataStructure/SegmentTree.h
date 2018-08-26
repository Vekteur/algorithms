#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T, typename Comp = std::less<T>>
class SegmentTree {
public:
	SegmentTree(std::vector<T> arr, Comp comp = Comp{});
	int query(int i, int j);
	void update(int i, int v);
	void print();

private:
	void rec_build(int p, int L, int R);
	int rec_query(int p, int L, int R, int i, int j);
	void rec_update(int p, int L, int R, int i);

	std::vector<T> a;
	std::vector<int> st;
	Comp comp;
};

template <typename T, typename Comp>
SegmentTree<T, Comp>::SegmentTree(std::vector<T> arr, Comp comp) : a{ arr }, comp{ comp } {
	st.assign(a.size() * 4, 0);
	rec_build(0, 0, a.size() - 1);
}

template <typename T, typename Comp>
int SegmentTree<T, Comp>::query(int i, int j) {
	return rec_query(0, 0, a.size() - 1, i, j);
}

template <typename T, typename Comp>
void SegmentTree<T, Comp>::update(int i, int v) {
	a[i] = v;
	rec_update(0, 0, a.size() - 1, i);
}

template <typename T, typename Comp>
void SegmentTree<T, Comp>::rec_build(int p, int L, int R) {
	if (L == R)
		// leaf node
		st[p] = L;
	else {
		// build children
		rec_build(2 * p + 1, L, (L + R) / 2);
		rec_build(2 * p + 2, (L + R) / 2 + 1, R);
		int c1 = st[2 * p + 1];
		int c2 = st[2 * p + 2];
		st[p] = comp(a[c1], a[c2]) ? c1 : c2;
	}
}

// p is the index of the current node,
// [L..R] is the current segment,
// [i..j] is the search interval
template <typename T, typename Comp>
int SegmentTree<T, Comp>::rec_query(int p, int L, int R, int i, int j) {
	// inside query range
	if (i <= L && R <= j) return st[p];
	// outside query range
	if (j < L || R < i) return -1;

	// compute the min position in the left
	// and right part of the interval
	int c1 = rec_query(2 * p + 1, L, (L + R) / 2, i, j);
	int c2 = rec_query(2 * p + 2, (L + R) / 2 + 1, R, i, j);

	// if we try to access a segment outside of the query
	if (c1 == -1) return c2;
	if (c2 == -1) return c1;
	return comp(a[c1], a[c2]) ? c1 : c2;
}

// i is the node that has to be updated
template <typename T, typename Comp>
void SegmentTree<T, Comp>::rec_update(int p, int L, int R, int i) {
	// if leaf node
	if (L == R) return;

	// if i is in the segment
	if (L <= i && i <= R) {
		rec_update(2 * p + 1, L, (L + R) / 2, i);
		rec_update(2 * p + 2, (L + R) / 2 + 1, R, i);
		int c1 = st[2 * p + 1];
		int c2 = st[2 * p + 2];
		st[p] = comp(a[c1], a[c2]) ? c1 : c2;
	}
}

template <typename T, typename Comp>
void SegmentTree<T, Comp>::print() {
	cout << "-----\n";
	int index = 0;
	for (int iter = 1; index + iter - 1 < st.size(); iter <<= 1) {
		for (int i = 0; i < iter; ++i)
			cout << st[index + i] << ' ';
		cout << '\n';
		index += iter;
	}
	cout << "-----\n";
}