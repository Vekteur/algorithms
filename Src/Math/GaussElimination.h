#pragma once

#include <vector>
#include <tuple>
#include <bitset>

#include "Constants.h"

namespace {
// See http://e-maxx.ru/algo/linear_systems_gauss
std::tuple<Solutions, std::vector<double>> gauss(std::vector<std::vector<double>> a, const std::vector<double>& b) {
	const double epsilon = 1e-7;
	int n = int(a.size());
	int m = int(a[0].size());
	for (int i = 0; i < n; ++i)
		a[i].push_back(b[i]);

	std::vector<int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; ++col) {
		// Set the row sel with maximum absolute value a[sel][col] as current row
		int sel = row;
		for (int i = row; i < n; ++i)
			if (abs(a[i][col]) > abs(a[sel][col]))
				sel = i;
		if (abs(a[sel][col]) < epsilon)
			continue;
		std::swap(a[sel], a[row]);
		where[col] = row;

		for (int i = 0; i < n; ++i)
			if (i != row) {
				// Add row row to row i, setting a[i][col] to 0
				double c = a[i][col] / a[row][col];
				for (int j = col; j <= m; ++j)
					a[i][j] -= a[row][j] * c;
			}
		++row;
	}

	// Compute answer
	std::vector<double> ans(m, 0);
	for (int i = 0; i < m; ++i)
		if (where[i] != -1)
			ans[i] = a[where[i]][m] / a[where[i]][i];

	// Check if there is a unique solution
	for (int i = 0; i < n; ++i) {
		double sum = 0;
		for (int j = 0; j < m; ++j)
			sum += ans[j] * a[i][j];
		if (abs(sum - a[i][m]) > epsilon)
			return { Solutions::ZERO, ans };
	}
	// Check if there is infinite solutions
	for (int i = 0; i < m; ++i)
		if (where[i] == -1)
			return { Solutions::INF, ans };
	// Else there is one solution
	return { Solutions::ONE, ans };
}
}

// See http://e-maxx.ru/algo/linear_systems_gauss
template<int M>
std::tuple<Solutions, std::bitset<M>> gaussMod2(std::vector<std::bitset<M + 1>> a) {
	int n = int(a.size());
	int m = int(a[0].size()) - 1;

	std::vector<int> where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; ++col) {
		// Set a row i with maximum absolute value a[i][col] (true) as current row
		for (int i = row; i < n; ++i) {
			if (a[i][col]) {
				std::swap(a[i], a[row]);
				break;
			}
		}
		if (!a[row][col])
			continue;
		where[col] = row;

		// xor row row to row i if needed in order to set a[i][col] to false
		for (int i = 0; i < n; ++i)
			if (i != row && a[i][col])
				a[i] ^= a[row];
		++row;
	}

	std::bitset<M> ans;
	for (int i = 0; i < m; ++i)
		if (where[i] != -1)
			ans[i] = a[where[i]][m];
	for (int i = 0; i < n; ++i) {
		bool sum = 0;
		for (int j = 0; j < m; ++j)
			sum ^= bool(ans[j] * a[i][j]);
		if (sum ^ a[i][m])
			return { Solutions::ZERO, ans };
	}

	for (int i = 0; i < m; ++i)
		if (where[i] == -1)
			return { Solutions::INF, ans };
	return { Solutions::ONE, ans };
}

// See http://e-maxx.ru/algo/determinant_gauss
double determinant(std::vector<std::vector<double>> a) {
	const double epsilon = 1e-7;
	int n = a.size();

	double det = 1.;
	for (int diag = 0; diag < n; ++diag) {
		int sel = diag;
		for (int j = diag + 1; j < n; ++j) {
			if (abs(a[j][diag] > abs(a[sel][diag])))
				sel = j;
		}
		if (abs(a[sel][diag]) < epsilon)
			return 0.;
		swap(a[diag], a[sel]);
		if (diag != sel)
			det = -det;
		det *= a[diag][diag];
		for (int col = diag + 1; col < n; ++col)
			a[diag][col] /= a[diag][diag];
		for (int row = 0; row < n; ++row) {
			if (row != diag && abs(a[row][diag]) > epsilon) {
				for (int col = diag + 1; col < n; ++col)
					a[row][col] -= a[diag][col] * a[row][diag];
			}
		}
	}
	return det;
}