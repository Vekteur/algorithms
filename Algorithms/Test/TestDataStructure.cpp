#include "catch.hpp"

#include <vector>
#include <functional>

#include "DataStructure/FenwickTree.h"
#include "DataStructure/SegmentTree.h"
#include "DataStructure/UnionFind.h"

using namespace std;

TEST_CASE("Data structures") {
	vector<int> arr{ 5, 1, 2, 9, 3, 1, 8, 3 };
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
			REQUIRE(st.query(2, 6) == 5);
			st.update(4, 0);
			st.update(2, 6);
			REQUIRE(st.query(2, 6) == 4);
		}
		SECTION("Max") {
			SegmentTree<int, greater<int>> st(arr);
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