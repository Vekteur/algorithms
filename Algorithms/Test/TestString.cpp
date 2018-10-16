#include "catch.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "String/Z_Algorithm.h"
#include "String/KMP.h"
#include "String/SuffixArray.h"
#include "String/StringHashing.h"

#include "TestUtil.h"

using namespace std;

TEST_CASE("String") {
	string s = "abacaba";
	SECTION("Z algorithm") {
		REQUIRE(zAlgorithmBuild(s) == vector<int>{ 7, 0, 1, 0, 3, 0, 1 });
		REQUIRE(zAlgorithmMatch(s, "ba") == vector<int>{ 1, 5 });
		REQUIRE(zAlgorithmDistinctSubstrings(s) == 21);
	}
	SECTION("Knuth-Morris-Pratt algorithm") {
		REQUIRE(kmpBuild(s) == vector<int>{ 0, 0, 1, 0, 1, 2, 3 });
		REQUIRE(kmpMatch(s, "ba") == vector<int>{ 1, 5 });
		REQUIRE(kmpMatchEachPrefix(s) == vector<int>{ 8, 4, 2, 2, 1, 1, 1, 1 });
		REQUIRE(kmpDistinctSubstrings(s) == 21);
	}
	SECTION("Suffix array") {
		SuffixArray sa(s);
		REQUIRE(sa.p == vector<int>{ 6, 4, 0, 2, 5, 1, 3 });
		REQUIRE(sa.match("ba") == vector<int>{ 5, 1 });
		REQUIRE(sa.compare(0, 4, 3) == 0);
		REQUIRE(sa.compare(2, 4, 3) == 1);
		REQUIRE(sa.compare(2, 3, 4) == -1);
		REQUIRE(sa.longestCommonPrefix(0, 4) == 3);
		REQUIRE(sa.longestCommonPrefix(2, 4) == 1);
		REQUIRE(sa.distinctSubstrings() == 21);
	}
	SECTION("String hashing") {
		REQUIRE(rabinKarp(s, "ba", 1e9 + 9) == vector<int>{ 1, 5 });
	}
}