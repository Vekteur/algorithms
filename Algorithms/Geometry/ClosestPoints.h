#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include "Point.h"

namespace {
double closestPoints(std::vector<Point> points) {
	int n = points.size();
	std::vector<Point> buf(n);
	double min_dist = INF_D;

	std::function<void(int, int)> closestPointsRec = [&closestPointsRec, &points, &buf, &min_dist](int l, int r) {
		if (r - l <= 1)
			return;
		// points is sorted by x
		int m = (l + r) / 2;
		double midx = points[m].x;
		closestPointsRec(l, m);
		closestPointsRec(m, r);

		std::merge(points.begin() + l, points.begin() + m, points.begin() + m, points.begin() + r, buf.begin() + l,
			[](Point p1, Point p2) { return p1.y < p2.y; });
		std::copy(buf.begin() + l, buf.begin() + r, points.begin() + l);
		// points is sorted by y
		int buf_i = l;
		for (int i = l; i < r; ++i) {
			if (abs(points[i].x - midx) < min_dist) {
				for (int j = buf_i - 1; j >= l && points[i].y - buf[j].y < min_dist; --j)
					min_dist = std::min(min_dist, points[i].dist(buf[j]));
				buf[buf_i++] = points[i];
			}
		}
	};

	std::sort(points.begin(), points.end());
	closestPointsRec(0, n);
	return min_dist;
}
}