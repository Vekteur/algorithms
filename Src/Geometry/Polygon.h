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
		vector<Point> sorted = points;
		std::sort(sorted.begin(), sorted.end());
		vector<Point> ch;
		for (int phase = 0; phase < 2; ++phase) {
			int start = int(ch.size());
			for (Point p : sorted) {
				while (int(ch.size()) >= start + 2 && 
						orient(ch[int(ch.size()) - 2], ch[int(ch.size()) - 1], p) <= 0)
					ch.pop_back();
				ch.push_back(p);
			}
			ch.pop_back();
			std::reverse(sorted.begin(), sorted.end());
		}
		if (ch.size() == 2 && ch[0] == ch[1])
			ch.pop_back();
		return ch;
	}
};