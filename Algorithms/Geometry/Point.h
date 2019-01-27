#pragma once

#include <iostream>

#include "Geometry.h"
#include "Constants.h"

namespace {

struct Point;

double orient(Point, Point, Point);

struct Point {
	double x, y;

	Point(double x = 0., double y = 0.) : x{ x }, y{ y } {}

	double norm() const {
		return sqrt(x * x + y * y);
	}

	double dist(Point p) const {
		return Point(p.x - x, p.y - y).norm();
	}

	bool inBox(Point a, Point b) const {
		return between(x, a.x, b.x) && between(y, a.y, b.y);
	}

	bool inAngle(Point p, Point q, Point r) const { // angle pqr (q is in the corner)
		return orient(q, p, *this) < 0 && orient(q, *this, r) < 0;
	}

	double angle(Point p) const {
		return atan2(p.y - y, p.x - x); // atan2 gives angle between ]-pi; pi]
	}

	double angle(Point a, Point b) const {
		double ang_a = angle(a), ang_b = angle(b);
		double ang = ang_a - ang_b;
		if (ang > PI) ang -= 2 * PI;
		if (ang <= -PI) ang += 2 * PI;
		return ang;
	}

	Point perp() const {
		return { -y, x };
	}

	bool operator==(const Point & p) const {
		return eq(x, p.x) && eq(y, p.y);
	}

	Point operator+(Point p) {
		return { x + p.x, y + p.y };
	}

	Point operator-() {
		return { -x, -y };
	}

	Point operator-(Point p) {
		return (*this) + -p;
	}
};

ostream& operator<<(ostream & out, Point p) {
	out << '(' << p.x << ' ' << p.y << ')';
	return out;
}

double dot(Point a, Point b) { 
	return a.x * b.x + a.y * b.y;
}

double cross(Point a, Point b) {
	return a.x * b.y - a.y * b.x;
}

double orient(Point p, Point q, Point r) {
	return cross(q - p, r - p);
}

struct AngleComp {
	Point pivot;
	AngleComp(Point _pivot) : pivot{ _pivot } {}

	bool operator()(const Point& a, const Point& b) const {
		if (eq(orient(pivot, a, b), 0))
			return pivot.dist(a) > pivot.dist(b);
		return pivot.angle(a) < pivot.angle(b);
	}
};

}