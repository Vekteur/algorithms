#pragma once

#include <vector>
#include <algorithm>

#include "Constants.h"

namespace {
int minimumCoins(const std::vector<int>& coins, int n) {
	std::vector<int> dp(n + 1, INF);
	dp[0] = 0;
	for (int x = 1; x <= n; ++x) {
		for (int coin : coins) {
			if (x - coin >= 0)
				dp[x] = std::min(dp[x], dp[x - coin] + 1);
		}
	}
	return dp[n];
}
}