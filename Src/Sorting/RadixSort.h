#pragma once

#include <vector>
#include <array>

template<int MaskSize>
void radixSort(std::vector<int>& a) {
	int n = a.size();
	const int intSize = 32;
	const int mask = (1 << MaskSize) - 1;
	std::array<int, 1 << MaskSize> cnt;
	std::vector<int> b(n);
	for (int shift = 0; shift < intSize; shift += MaskSize) {
		std::fill(cnt.begin(), cnt.end(), 0);
		for (int i = 0; i < n; ++i) {
			int bits = (a[i] >> shift) & mask;
			++cnt[bits];
		}
		for (int i = 1; i < int(cnt.size()); ++i) {
			cnt[i] += cnt[i - 1];
		}
		for (int i = n - 1; i >= 0; --i) {
			int bits = (a[i] >> shift) & mask;
			b[--cnt[bits]] = a[i];
		}
		a = b;
	}
}