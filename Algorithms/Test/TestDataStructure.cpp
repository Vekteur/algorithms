#include "catch.hpp"

#include <vector>
#include <functional>

#include "DataStructure/SumArray.h"
#include "DataStructure/DifferenceArray.h"
#include "DataStructure/SparseTable.h"
#include "DataStructure/FenwickTree.h"
#include "DataStructure/SegmentTree.h"
#include "DataStructure/UnionFind.h"

using namespace std;

TEST_CASE("Data structures") {
	vector<int> arr{ 5, 1, 2, 9, 3, 1, 8, 3 };
	SECTION("Sum Array") {
		REQUIRE(SumArray<int>(arr).query(2, 5) == 15);
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
		REQUIRE(st.query(0, 7) == 1);
		REQUIRE(st.query(6, 7) == 3);
		SparseTable<int> maxSt(arr, [](int a, int b) { return std::max(a, b); });
		REQUIRE(maxSt.query(0, 7) == 9);
	}
	SECTION("Fenwick tree") {
		FenwickTree<int> ft(arr.size());
		for (int i = 0; i < arr.size(); ++i)
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
		SECTION("Max") {
			vector<int> maxIndices(arr.size());
			std::iota(maxIndices.begin(), maxIndices.end(), 0);
			SegmentTree<int> st(maxIndices, [&arr](int i1, int i2) { return arr[i1] >= arr[i2] ? i1 : i2; });
			REQUIRE(st.query(2, 6) == 3);
		}
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