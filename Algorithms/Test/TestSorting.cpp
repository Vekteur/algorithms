#include "catch.hpp"

#include <iostream>
#include <algorithm>
#include <functional>

#include "Sorting/BinarySearch.h"
#include "Sorting/TernarySearch.h"
#include "Sorting/BasicSorting.h"
#include "Sorting/MergeSort.h"

using namespace std;

TEST_CASE("Sorting") {
	SECTION("Binary seach") {
		vector<int> arr{ 1, 2, 5, 5, 6, 9, 14 };
		REQUIRE(binarySearch(0, int(arr.size()) - 1, [&arr](int i) { return arr[i] <= 5; }) == 3);
		REQUIRE(binarySearch(0, int(arr.size()) - 1, [&arr](int i) { return arr[i] <= 8; }) == 4);
		REQUIRE(binarySearch(0., 100., [](double x) { return x * x < 9.; }) == Approx(3.));
	}
	SECTION("Ternary search") {
		auto maxF = [](double x) { return -abs(2 * (x - 3)) + 5; };
		REQUIRE(ternarySearch(-10., 10., maxF) == Approx(3.));
		auto minF = [](double x) { return (x - 2) * (x - 2) + 1; };
		REQUIRE(ternarySearch(-10., 10., minF, std::greater<double>{}) == Approx(2.));
		vector<int> v{ 1, 2, 5, 6, 7, 4, -1 };
		REQUIRE(ternarySearch(0, v.size() - 1, [&v](int x) { return v[x]; }) == 4);
	}
	std::vector<int> a{ 5, 8, 4, 4, 9, 6, 3 };
	std::vector<int> expected = a;
	std::sort(expected.begin(), expected.end());
	SECTION("Bubble sort") {
		bubbleSort(a);
		REQUIRE(a == expected);
	}
	SECTION("Selection sort") {
		selectionSort(a);
		REQUIRE(a == expected);
	}
	SECTION("Insertion sort") {
		insertionSort(a);
		REQUIRE(a == expected);
	}
	SECTION("Counting sort") {
		countingSort(a, *max_element(a.begin(), a.end()));
		REQUIRE(a == expected);
	}
	SECTION("Merge sort") {
		mergeSort(a);
		REQUIRE(a == expected);
	}
}