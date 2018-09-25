#pragma once

#include <iostream>

#include "Geometry.h"
#include "Constants.h"

struct Point;

double orient(Point, Point, Point);

struct Point {
	double x, y;

	Point(double x, double y) : x{ x }, y{ y } {}

	double dist(Point p) const {
		double dx = x - p.x;
		double dy = y - p.y;
		return sqrt(dx * dx + dy * dy);
	}

	bool inBox(Point a, Point b) const {
		return min(a.x, b.x) <= x && x <= max(a.x, b.x) && min(a.y, b.y) <= y && y <= max(a.y, b.y);
	}

	bool inAngle(Point p, Point q, Point r) const // angle pqr (q in center)
	{
		return orient(q, p, *this) < 0 && orient(q, *this, r) < 0;
	}

	double angle(Point a, Point b) const {
		Point da{ a.x - x, a.y - y };
		Point db{ b.x - x, b.y - y };

		double anga = atan2(da.y, da.x); // atan gives angle between [-pi; pi]
		double angb = atan2(db.y, db.x);

		double res = anga - angb;
		if (res > PI) res -= 2 * PI;
		if (res < -PI) res += 2 * PI;

		return res;
	}

	bool operator==(const Point& p) const {
		return eq(x, p.x) && eq(y, p.y);
	}
};

double orient(Point p, Point q, Point r) {
	return q.x * r.y - r.x * q.y - p.x * (r.y - q.y) + p.y * (r.x - q.x);
}

struct AngleComp {
	Point pivot;
	AngleComp(Point _pivot) : pivot{ _pivot } {}

	bool operator()(const Point& a, const Point& b) const {
		if (orient(pivot, a, b) == 0)
			return pivot.dist(a) < pivot.dist(b);

		Point da{ a.x - pivot.x, a.y - pivot.y };
		Point db{ b.x - pivot.x, b.y - pivot.y };
		return(atan2(a.y, a.x) < atan2(b.y, b.x));
	}
};

namespace {
ostream& operator<<(ostream& out, Point p) {
	out << p.x << ' ' << p.y;
	return out;
}
}