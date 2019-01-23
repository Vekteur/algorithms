#pragma once

#include "Geometry.h"
#include "Point.h"

struct Line {
	double a, b, c; // ax + by = c

	Line(double a, double b, double c) : a{ a }, b{ b }, c{ c } {
		normalize();
	}

	Line(Point p1, Point p2) {
		a = p2.y - p1.y;
		b = p1.x - p2.x;
		c = a * p1.x + b * p1.y;
		normalize();
	}

	bool contains(Point p) const {
		return eq(a * p.x + b * p.y, c);
	}

	double dist(Point p) const {
		return abs(a * p.x + b * p.y - c) / sqrt(a * a + b * b);
	}

	Point inter(Line l) const {
		double d = a * l.b - l.a * b;
		double x = (l.b * c - b * l.c) / d;
		double y = (a * l.c - l.a * c) / d;
		return Point(x, y);
	}

	Line perp(Point p) const {
		return Line(-b, a, a * p.y - b * p.x);
	}

	Point sym(Point p) const {
		Line l = perp(p);
		Point q = inter(l);
		return Point(2 * q.x - p.x, 2 * q.y - p.y);
	}

	static Line vertical(int c) {
		return Line(1., 0., c);
	}

	static Line horizontal(int c) {
		return Line(0., 1., c);
	}

	void normalize() {
		double norm = Point(a, b).norm();
		if (a < 0)
			norm *= -1;
		a /= norm;
		b /= norm;
		c /= norm;
	}

	bool operator==(const Line& l) const {
		return eq(a, l.a) && eq(b, l.b) && eq(c, l.c);
	}
};

namespace {
ostream& operator<<(ostream& out, Line l) {
	out << l.a << ' ' << l.b << ' ' << l.c;
	return out;
}
}