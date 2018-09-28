#include "catch.hpp"

#include <vector>
#include <functional>

#include "DataStructure/SumArray.h"
#include "DataStructure/DifferenceArray.h"
#include "DataStructure/SparseTable.h"
#include "DataStructure/FenwickTree.h"
#include "DataStructure/SegmentTree.h"
#include "DataStructure/LazySegmentTree.h"
#include "DataStructure/SegmentTree2D.h"
#include "DataStructure/UnionFind.h"
#include "DataStructure/OptimalRMQ.h"

using namespace std;

TEST_CASE("Data structures") {
	vector<int> arr{ 5, 1, 2, 9, 3, 1, 8, 3 };
	SECTION("Sum Array") {
		SumArray<int> sa(arr);
		REQUIRE(sa.query(2, 5) == 15);
		sa.update(2, 3);
		sa.update(3, -2);
		REQUIRE(sa.query(2, 5) == 16);
	}
	SECTION("Difference array") {
		DifferenceArray<int> da(arr);
		REQUIRE(da.query(4) == 3);
		da.update(2, 4, 4);
		da.update(4, 7, -3);
		da.update(6, 7, 11);
		REQUIRE(da.query(4) == 4);
	}
	SECTION("Sparse table") {
		SparseTable<int> st(arr);
		REQUIRE(st.query(2, 4) == 2);
		st.update(3, 0);
		REQUIRE(st.query(0, 7) == 0);
		REQUIRE(st.query(6, 7) == 3);
		SparseTable<int> maxSt(arr, [](int a, int b) { return std::max(a, b); });
		REQUIRE(maxSt.query(0, 7) == 9);
	}
	SECTION("Fenwick tree") {
		FenwickTree<int> ft(arr.size());
		for (int i = 0; i < int(arr.size()); ++i)
			ft.update(i, arr[i]);
		REQUIRE(ft.rsq(0, 3) == 17);
		REQUIRE(ft.rsq(2, 6) == 23);
	}
	SECTION("Segment tree") {
		SECTION("Min") {
			SegmentTree<int> st(arr);
			REQUIRE(st.query(2, 6) == 1);
			st.update(4, 0);
			st.update(2, 6);
			REQUIRE(st.query(2, 6) == 0);
		}
		SECTION("Max index") {
			vector<int> maxIndices(arr.size());
			std::iota(maxIndices.begin(), maxIndices.end(), 0);
			SegmentTree<int> st(maxIndices, [&arr](int i1, int i2) { return arr[i1] >= arr[i2] ? i1 : i2; });
			REQUIRE(st.query(2, 6) == 3);
		}
	}
	SECTION("Lazy Segment Tree") {
		SECTION("Sum and assignment") {
			struct Lazy { bool assigned = false; int assignment; int add = 0; };
			LazySegmentTree<int, Lazy> st(arr, [](int a, int b) { return a + b; },
				[](const Lazy& p, Lazy& c) {
					if (p.assigned) { c.assigned = true; c.assignment = p.assignment; }
					else { c.add += p.add; }
				},
				[](const int& t, const Lazy& l, int range) { 
					return l.assigned ? (l.assignment * range) : (t + l.add * range); 
				}
			);
			REQUIRE(st.query(1, 6) == 24);
			st.update(0, 4, Lazy{ false, 0, 3 });
			REQUIRE(st.query(1, 6) == 36);
			st.update(2, 6, Lazy{ false, 0, 2 });
			REQUIRE(st.query(1, 6) == 46);
			st.update(1, 3, Lazy{ true, -1, 0 });
			REQUIRE(st.query(1, 6) == 18);
		}

	}
	SECTION("2D Segment Tree") {
		std::vector<std::vector<int>> arr2D {
			{ 4, 2, 9, -3 },
			{ 2, 0, 1, 0 },
			{ 2, 6, 5, 4 },
			{ -2, 1, 8, 3 }
		};

		SegmentTree2D<int> st(arr2D, [](int a, int b) { return a + b; });
		REQUIRE(st.query(1, 3, 1, 2) == 21);
		st.update(1, 2, 5);
		REQUIRE(st.query(1, 3, 1, 2) == 25);
	}
	SECTION("Optimal range minimum query") {
		OptimalRMQ<int> rmq(arr);
		REQUIRE(rmq.query(2, 4) == 2);
		REQUIRE(rmq.query(0, 3) == 1);
		OptimalRMQ<int> rmaxq(arr, std::greater<int>());
		REQUIRE(rmaxq.query(0, 7) == 9);
	}
	SECTION("Union-find") {
		UnionFind uf(8);
		uf.merge(1, 3);
		uf.merge(4, 5);
		REQUIRE(!uf.sameSet(1, 5));
		uf.merge(3, 4);
		REQUIRE(uf.sameSet(1, 5));
		REQUIRE(!uf.sameSet(1, 6));
	}
}