#pragma once

int nthPower(int b, int n) {
	if (n == 0)
		return 1;

	int power = nthPower(b, n / 2);
	power *= power;
	if (n & 1 == 1)
		power *= b;
	return power;
}