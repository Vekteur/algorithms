#pragma once

#include <cstdio>
#include <vector>

#include "Constants.h"

class Simplex {
private:
	const double epsilon = 1e-7;

	int m, n;
	double z = 0.;
	std::vector<std::vector<double>> a;
	std::vector<double> b, c, sol;
	std::vector<int> base;

	bool equals(double a, double b) {
		return abs(a - b) < epsilon;
	}

	// First negative in c
	int firstNegInC() {
		for (int col = 0; col < n; ++col) {
			if (c[col] < 0)
				return col;
		}
		return -1;
	}

	// Row with the smallest ratio in column col
	int smallestRatio(int col) {
		double minRatio = INF_D;
		int minRow = -1;
		for (int row = 0; row < m; ++row) {
			if (a[row][col] > 0) {
				double ratio = b[row] / a[row][col];
				if (ratio < minRatio) {
					minRatio = ratio;
					minRow = row;
				}
			}
		}
		return minRow;
	}

	void multRowOfA(int row, double ratio) {
		for (int col = 0; col < n; ++col)
			a[row][col] *= ratio;
		b[row] *= ratio;
	}

	void addRowToA(int fromRow, int toRow, double ratio) {
		for (int col = 0; col < n; ++col)
			a[toRow][col] += a[fromRow][col] * ratio;
		b[toRow] += b[fromRow] * ratio;
	}

	void addRowToC(int fromRow, double ratio) {
		for (int col = 0; col < n; ++col)
			c[col] += a[fromRow][col] * ratio;
		z += b[fromRow] * ratio;
	}

	// The only base column in the given row
	int baseInRow(int row) {
		for (int col = 0; col < n; ++col) {
			if (base[col] && equals(a[row][col], 1.))
				return col;
		}
		return -1;
	}

	void printTab() {
		for (int i = 0; i < n; ++i)
			printf("%8d ", base[i]);
		printf("\n");
		for (int i = 0; i < n; ++i)
			printf("%8.2g ", c[i]);
		printf(" | %8.2g\n", z);
		for (int i = 0; i < n; ++i)
			printf("---------");
		printf("-+-");
		printf("---------");
		printf("\n");
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j)
				printf("%8.2g ", a[i][j]);
			printf(" | %8.2g\n", b[i]);
		}
		printf("\n");
	}

	void computeSolution() {
		sol.assign(n, 0);
		for (int row = 0; row < m; ++row)
			sol[baseInRow(row)] = b[row];
	}

	void computeSimplex() {
		//printTab();
		int colIn;
		while ((colIn = firstNegInC()) != -1) {
			int pivot = smallestRatio(colIn);
			if (pivot == -1) {
				z = INF_D; // Unbounded
				break;
			}
			int colOut = baseInRow(pivot);
			multRowOfA(pivot, 1. / a[pivot][colIn]);
			for (int row = 0; row < m; ++row) {
				if (row != pivot)
					addRowToA(pivot, row, -a[row][colIn]);
			}
			addRowToC(pivot, -c[colIn]);
			base[colOut] = false;
			base[colIn] = true;
			//printTab();
		}
		computeSolution();
	}

	// Resolve the extended simplex problem with additional variables
	Simplex(const std::vector<std::vector<double>>& a_, const std::vector<double>& b_)
		: a(a_), b(b_) {

		m = int(b.size());
		n = int(a[0].size());

		base.assign(n, false);
		c.assign(n, 0.);
		for (int col = 0; col < m; ++col) {
			base.push_back(true);
			c.push_back(1.);
			for (int row = 0; row < m; ++row)
				a[row].push_back(row == col ? 1. : 0.);
		}
		n += m;
		for (int row = 0; row < m; ++row) {
			addRowToC(row, -1.);
		}
		computeSimplex();
	}

public:
	// Standard form simplex
	Simplex(const std::vector<std::vector<double>>& a_, const std::vector<double>& b_,
		const std::vector<double>& c_)
		: a(a_), b(b_), c(c_) {

		m = int(b.size());
		n = int(a[0].size());

		for (int row = 0; row < m; ++row) {
			if (b[row] < 0.)
				multRowOfA(row, -1.);
		}

		Simplex extended{ a, b };
		if (!equals(extended.z, 0.)) {
			z = -INF_D; // No solution
			return;
		}

		base = extended.base;
		base.resize(n);
		a = extended.a;
		for (int row = 0; row < m; ++row)
			a[row].resize(n);
		b = extended.b;
		computeSimplex();
	}

	// Geometric form simplex
	Simplex(const std::vector<std::vector<double>>& a_, const std::vector<double>& b_,
		const std::vector<double>& c_, bool geometric)
		: a(a_), b(b_), c(c_) {

		m = int(b.size()), n = int(a[0].size());
		// Make the variables positive
		// by adding another negative variable for each variable
		for (int var = 0; var < n; ++var) {
			c.push_back(-c[var]);
			for (int row = 0; row < m; ++row) {
				a[row].push_back(-a[row][var]);
			}
		}
		// Make the inequalities equalities
		// by substracting a positive variable in each inequality
		for (int var = 0; var < m; ++var) {
			c.push_back(0.);
			for (int row = 0; row < m; ++row) {
				a[row].push_back(row == var ? -1. : 0.);
			}
		}

		Simplex standard{ a, b, c };
		sol.resize(n);
		for (int var = 0; var < n; ++var)
			sol[var] = standard.sol[var] - standard.sol[var + n];
		z = standard.z;
	}

	double objective() {
		return -z;
	}

	std::vector<double> solution() {
		return sol;
	}
};