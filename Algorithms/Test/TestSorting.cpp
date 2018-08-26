#include "catch.hpp"

#include <iostream>

#include "Sorting/BinarySearch.h"

using namespace std;

TEST_CASE("Search") {
	vector<int> v{ 1, 2, 5, 5, 6, 9, 14 };
	REQUIRE(binary_search(v, 5) == 2);
	REQUIRE(binary_search(v, 4) == -1);
}