#pragma once

#include <vector>
#include <bitset>
#include <tuple>

// Choose N sufficiently large such that N >= sum + 1
// Might be implemented using a dynamic bitset
template<int N>
bool subsetSum(const std::vector<int>& objs, int sum) {
	std::bitset<N> bs(1);
	for (int v : objs)
		bs |= (bs << v);
	return bs[sum];
}

struct SubsetObjects {
	int v; // Value
	int m; // Multiplicity
};

// Choose N sufficiently large such that N >= sum + 1
template<int N>
bool multiplicitySubsetSum(const std::vector<SubsetObjects>& objs, int sum) {
	std::bitset<N> bs(1);
	for (auto obj : objs) {
		for (int x = 1; x <= obj.m; x *= 2) {
			bs |= (bs << (obj.v * x));
			obj.m -= x;
		}
		bs |= (bs << (obj.v * obj.m));
	}
	return bs[sum];
}