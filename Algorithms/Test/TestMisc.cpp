#include "catch.hpp"

#include <iostream>
#include <algorithm>

#include "Misc/Subsets.h"
#include "Misc/Permutations.h"

#include "Math/BasicMath.h"
#include "DataStructure/UnionFind.h"

using namespace std;

TEST_CASE("Miscellaneous algorithms") {
	std::vector<int> a{ 4, 2, 3, 7, 4, 3, 1 };
	SECTION("Subsets") {
		REQUIRE(subsets(a).size() == (1 << a.size()));
		REQUIRE(subsets2(a).size() == (1 << a.size()));
		REQUIRE(subsets3(a).size() == (1 << a.size()));
	}
	SECTION("Permutations") {
		REQUIRE(permutations(a).size() == fact(a.size()));
		REQUIRE(permutations(a).size() == 5040);
	}
}