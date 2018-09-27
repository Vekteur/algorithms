#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
class SegmentTree2D {
private:
	int n, m;
	std::function<T(T, T)> combine;
	std::vector<std::vector<T>> st;

	/*
	p? is the index of the current node
	s? is a search interval
	l? is the left of an interval
	r? is the right of an interval
	?x means that the interval is on the x axe
	?y means that the interval is on the y axe
	*/
	void rec_build_y(int px, int lx, int rx, int py, int ly, int ry, const std::vector<std::vector<T>>& a) {
		if (ly == ry) {
			if (lx == rx)
				st[px][py] = a[lx][ly];
			else
				st[px][py] = combine(st[2 * px + 1][py], st[2 * px + 2][py]);
		} else {
			int my = (ly + ry) / 2;
			rec_build_y(px, lx, rx, 2 * py + 1, ly, my, a);
			rec_build_y(px, lx, rx, 2 * py + 2, my + 1, ry, a);
			st[px][py] = combine(st[px][2 * py + 1], st[px][2 * py + 2]);
		}
	}

	void rec_build_x(int px, int lx, int rx, const std::vector<std::vector<T>>& a) {
		if (lx != rx) {
			int mx = (lx + rx) / 2;
			rec_build_x(2 * px + 1, lx, mx, a);
			rec_build_x(2 * px + 2, mx + 1, rx, a);
		}
		rec_build_y(px, lx, rx, 0, 0, m - 1, a);
	}

	T rec_query_y(int px, int py, int ly, int ry, int sly, int sry) {
		if (sly <= ly && ry <= sry) {
			return st[px][py];
		} else {
			int my = (ly + ry) / 2;
			if (sry <= my)
				return rec_query_y(px, 2 * py + 1, ly, my, sly, sry);
			else if (sly > my)
				return rec_query_y(px, 2 * py + 2, my + 1, ry, sly, sry);
			else
				return combine(rec_query_y(px, 2 * py + 1, ly, my, sly, sry),
					rec_query_y(px, 2 * py + 2, my + 1, ry, sly, sry));
		}
	}

	T rec_query_x(int px, int lx, int rx, int slx, int srx, int sly, int sry) {
		if (slx <= lx && rx <= srx) {
			return rec_query_y(px, 0, 0, m - 1, sly, sry);
		} else {
			int mx = (lx + rx) / 2;
			if (srx <= mx)
				return rec_query_x(2 * px + 1, lx, mx, slx, srx, sly, sry);
			else if (slx > mx)
				return rec_query_x(2 * px + 2, mx + 1, rx, slx, srx, sly, sry);
			else
				return combine(rec_query_x(2 * px + 1, lx, mx, slx, srx, sly, sry),
					rec_query_x(2 * px + 2, mx + 1, rx, slx, srx, sly, sry));
		}
	}

	void rec_update_y(int px, int lx, int rx, int py, int ly, int ry, int sy, int v) {
		if (ly == ry) {
			if (lx == rx)
				st[px][py] = v;
			else
				st[px][py] = combine(st[2 * px + 1][py], st[2 * px + 2][py]);
		} else {
			int my = (ly + ry) / 2;
			if (sy <= my)
				rec_update_y(px, lx, rx, 2 * py + 1, ly, my, sy, v);
			else
				rec_update_y(px, lx, rx, 2 * py + 2, my + 1, ry, sy, v);
			st[px][py] = combine(st[px][2 * py + 1], st[px][2 * py + 2]);
		}
	}

	void rec_update_x(int px, int lx, int rx, int sx, int sy, int v) {
		if (lx != rx) {
			int mx = (lx + rx) / 2;
			if (sx <= mx)
				rec_update_x(2 * px + 1, lx, mx, sx, sy, v);
			else
				rec_update_x(2 * px + 2, mx + 1, rx, sx, sy, v);
		}
		rec_update_y(px, lx, rx, 0, 0, m - 1, sy, v);
	}

public:
	SegmentTree2D() {}
	SegmentTree2D(const std::vector<std::vector<T>>& a,
		std::function<T(T, T)> combine = [](T c1, T c2) { return min(c1, c2); })
		: n{ int(a.size()) }, m{ int(a[0].size()) }, combine{ combine } {

		st.assign(n * 4, std::vector<T>(m * 4));
		rec_build_x(0, 0, n - 1, a);
	}

	T query(int slx, int srx, int sly, int sry) {
		return rec_query_x(0, 0, n - 1, slx, srx, sly, sry);
	}

	void update(int sx, int sy, int v) {
		rec_update_x(0, 0, n - 1, sx, sy, v);
	}
};