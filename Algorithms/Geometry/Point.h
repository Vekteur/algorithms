#pragma once

#include <iostream>

#include "Geometry.h"

double orient(Point p, Point q, Point r);

class Point {
public:
	double x, y;

	Point(double _x, double _y) : x{ _x }, y{ _y } {}

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
		if (res > pi) res -= 2 * pi;
		if (res < -pi) res += 2 * pi;

		//cout << anga*180/pi << ' ' << angb*180/pi << '\n';
		return res;
	}
};

ostream& operator<<(ostream& out, Point p) {
	out << p.x << ' ' << p.y;
	return out;
}

double orient(Point p, Point q, Point r) {
	return q.x * r.y - r.x * q.y - p.x * (r.y - q.y) + p.y * (r.x - q.x);
}