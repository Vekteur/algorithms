#pragma once

#include "Geometry.h"
#include "Point.h"
#include "Line.h"

#include <algorithm>
#include <tuple>

namespace {

struct Segment {
	Point p1, p2;

	Segment(Point p1, Point p2) : p1{ p1 }, p2{ p2 } {}

	Line toLine() const {
		return Line(p1, p2);
	}

	std::tuple<bool, Point> inter(Segment s) const {
		Line l1 = toLine(), l2 = s.toLine();
		if (l1.isParallel(l2)) {
			if (l1 == l2) {
				if (p1.inBox(s.p1, s.p2)) return { true, p1 };
				if (p2.inBox(s.p1, s.p2)) return { true, p2 };
				if (s.p1.inBox(p1, p2)) return { true, s.p1 };
				if (s.p2.inBox(p1, p2)) return { true, s.p2 };
			}
			return { false, {} };
		}
		Point p = l1.inter(l2);
		return { p.inBox(p1, p2) && p.inBox(s.p1, s.p2), p };
	}

	std::tuple<bool, Point> inter(Line l2) const {
		Line l1 = toLine();
		if (l1.isParallel(l2)) {
			if (l1 == l2)
				return { true, p1 };
			return { false, {} };
		}
		Point p = l1.inter(l2);
		return { p.inBox(p1, p2), p };
	}

	double dist(Point p) const {
		Line l = toLine();
		Point dir = Point(l.a, l.b).perp();
		double d = dot(dir, p), d1 = dot(dir, p1), d2 = dot(dir, p2);
		if (min(d1, d2) < d && d < max(d1, d2))
			return l.dist(p);
		return min(p1.dist(p), p2.dist(p));
	}

	double dist(Line l) const {
		if (std::get<0>(inter(l)))
			return 0;
		return min(l.dist(p1), l.dist(p2));
	}

	double dist(Segment s) const {
		return std::min({ dist(s.p1), dist(s.p2), s.dist(p1), s.dist(p2) });
	}
};

ostream& operator<<(ostream& out, Segment s) {
	out << '(' << s.p1 << ' ' << s.p2 << ')';
	return out;
}

}