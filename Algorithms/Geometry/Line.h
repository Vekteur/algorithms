#pragma once

#include "Geometry.h"
#include "Point.h"

struct Line {
	double a, b, c; // ax + by + c = 0

	Line(double a, double b, double c) : a{ a }, b{ b }, c{ c } {}

	Line(Point s, Point t) {
		a = t.y - s.y;
		b = s.x - t.x;
		c = -(a * s.x + b * s.y);
	}

	bool contains(Point p) const {
		return eq(a * p.x + b * p.y, c);
	}

	double dist(Point p) const {
		return abs(a * p.x + b * p.y + c) / sqrt(a * a + b * b);
	}

	Point inter(Line l) const {
		double d = a * l.b - l.a * b;
		double x = (b * l.c - l.b * c) / d;
		double y = (l.a * c - a * l.c) / d;
		return Point(x, y);
	}

	Line perp(Point p) const {
		return Line(-b, a, b * p.x - a * p.y);
	}

	Point sym(Point p) const {
		Line l = perp(p);
		Point q = inter(l);
		return Point(2 * q.x - p.x, 2 * q.y - p.y);
	}

	static Line vertical(int c) {
		return Line(1., 0., -c);
	}

	static Line horizontal(int c) {
		return Line(0., 1., -c);
	}

	bool operator==(const Line& l) const {
		if (b == 0 || l.b == 0) {
			if (b == 0 && l.b == 0)
				return eq(a / c, l.a / l.c);
			return false;
		}
		return(eq(a / b, l.a / l.b) && eq(c / b, l.c / l.b));
	}
};

namespace {
ostream& operator<<(ostream& out, Line l) {
	out << l.a << ' ' << l.b << ' ' << l.c;
	return out;
}
}