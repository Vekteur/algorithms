#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T, typename Func = std::less<T>>
class SegmentTree {
public:
	SegmentTree(std::vector<T> arr, Func func = Func{});
	int query(int i, int j);
	void update(int i, int v);
	void print();

private:
	void rec_build(int p, int L, int R);
	int rec_query(int p, int L, int R, int i, int j);
	void rec_update(int p, int L, int R, int i);

	std::vector<T> A;
	std::vector<int> st;
	Func functor;
};

template <typename T, typename Func>
SegmentTree<T, Func>::SegmentTree(std::vector<T> arr, Func func) : A{ arr }, functor{ func } {
	st.assign(A.size() * 4, 0);
	rec_build(0, 0, A.size() - 1);
}

template <typename T, typename Func>
int SegmentTree<T, Func>::query(int i, int j) {
	return rec_query(0, 0, A.size() - 1, i, j);
}

template <typename T, typename Func>
void SegmentTree<T, Func>::update(int i, int v) {
	A[i] = v;
	rec_update(0, 0, A.size() - 1, i);
}

template <typename T, typename Func>
void SegmentTree<T, Func>::rec_build(int p, int L, int R) {
	if (L == R)
		// leaf node
		st[p] = L;
	else {
		// build children
		rec_build(2 * p + 1, L, (L + R) / 2);
		rec_build(2 * p + 2, (L + R) / 2 + 1, R);
		int a1 = st[2 * p + 1];
		int a2 = st[2 * p + 2];
		st[p] = functor(A[a1], A[a2]) ? a1 : a2;
	}
}

// p is the index of the current node,
// [L..R] is the current segment,
// [i..j] is the search interval
template <typename T, typename Func>
int SegmentTree<T, Func>::rec_query(int p, int L, int R, int i, int j) {
	// inside query range
	if (i <= L && R <= j) return st[p];
	// outside query range
	if (j < L || R < i) return -1;

	// compute the min position in the left
	// and right part of the interval
	int a1 = rec_query(2 * p + 1, L, (L + R) / 2, i, j);
	int a2 = rec_query(2 * p + 2, (L + R) / 2 + 1, R, i, j);

	// if we try to access a segment outside of the query
	if (a1 == -1) return a2;
	if (a2 == -1) return a1;
	return functor(A[a1], A[a2]) ? a1 : a2;
}

// i is the node that has to be updated
template <typename T, typename Func>
void SegmentTree<T, Func>::rec_update(int p, int L, int R, int i) {
	// if leaf node
	if (L == R) return;

	// if i is in the segment
	if (L <= i && i <= R) {
		rec_update(2 * p + 1, L, (L + R) / 2, i);
		rec_update(2 * p + 2, (L + R) / 2 + 1, R, i);
		int a1 = st[2 * p + 1];
		int a2 = st[2 * p + 2];
		st[p] = functor(A[a1], A[a2]) ? a1 : a2;
	}
}

template <typename T, typename Func>
void SegmentTree<T, Func>::print() {
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