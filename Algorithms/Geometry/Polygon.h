#pragma once

#include <vector>
#include <algorithm>

#include "Geometry.h"
#include "Point.h"
#include "Line.h"

struct Polygon {
	std::vector<Point> points;

	Polygon(std::vector<Point> points) : points(points) {}

	int size() const {
		return int(points.size());
	}

	double perimeter() const {
		double p = 0;
		for (int i = 0; i < size(); ++i)
			p += points[i].dist(points[(i + 1) % size()]);
		return p;
	}

	double area() const {
		double result = 0;
		for (int i = 0; i < size(); ++i)
			result += points[i].x * points[(i + 1) % size()].y - points[(i + 1) % size()].x * points[i].y;
		return result / 2;
	}

	bool isConvex() const {
		bool has_pos = false, has_neg = false;
		for (int i = 0; i < size(); ++i) {
			double o = orient(points[i], points[(i + 1) % size()], points[(i + 2) % size()]);
			if (o < 0) has_neg = true;
			if (o > 0) has_pos = true;
		}
		return !(has_pos && has_neg);
	}

	bool contains(Point p) const {
		double angle = 0;
		for (int i = 0; i < size(); ++i)
			angle += p.angle(points[i], points[(i + 1) % size()]);
		
		// Strictly in the polygon
		if (eq(angle, -2 * PI))
			return true;

		// On the bounds of the polygon
		for (int i = 0; i < size(); ++i) {
			Line seg{ points[i], points[(i + 1) % size()] };
			if (eq(seg.side(p), 0) && p.inBox(points[i], points[(i + 1) % size()]))
				return true;
		}
		return false;
	}

	std::vector<Point> convexHull() const {
		Point pivot = points[0];
		for (Point p : points) {
			if (p.x < pivot.x || (p.x == pivot.x && p.y < pivot.y))
				pivot = p;
		}

		// The pivot is always last in sorted order
		// because it is aligned with itself and is the closest of itself
		std::vector<Point> sorted = points;
		std::sort(sorted.begin(), sorted.end(), AngleComp{ pivot });

		if (int(sorted.size()) <= 3)
			return sorted;

		std::vector<Point> ch;
		for (int k = 0; k < 2; ++k)
			ch.push_back(sorted[k]);

		for (int k = 2; k < int(sorted.size()); ++k) {
			while (orient(ch[int(ch.size()) - 2], ch[int(ch.size()) - 1], sorted[k]) < 0)
				ch.pop_back();
			ch.push_back(sorted[k]);
		}

		return ch;
	}
};