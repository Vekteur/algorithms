#include "catch.hpp"

#include <iostream>

#include "Dynamic/Knapsack.h"

using namespace std;

TEST_CASE("Dynamic") {
	SECTION("Knapsack") {
		vector<Obj> objs{ {3, 5}, {2, 3}, {2, 3}, {1, 2} };
		REQUIRE(knapsack(objs, 5) == 8);
		REQUIRE(knapsack(objs, 6) == 10);
	}
}