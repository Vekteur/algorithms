#pragma once

#include <vector>
#include <algorithm>

#include "Geometry.h"
#include "Point.h"
#include "Line.h"

class Polygon {
public:
	vector<Point> points;

	Polygon& add(Point p) {
		points.push_back(p);
		return *this;
	}

	vector<Point> getPoints() const {
		return points;
	}

	double getPerimeter() const {
		int s = points.size();
		double p = 0;
		for (int i = 0; i < points.size(); ++i)
			p += points[i].dist(points[(i + 1) % s]);
		return p;
	}

	double getArea() {
		double result = 0;
		points.push_back(points[0]);
		for (int i = 0; i < points.size() - 1; ++i)
			result += points[i].x * points[i + 1].y - points[i + 1].x * points[i].y;
		points.pop_back();
		return result / 2;
	}

	bool isConvex() const // Suppose that the vertices are counter-clockwise
	{
		int s = points.size();
		for (int i = 0; i < s; ++i)
			if (orient(points[i], points[(i + 1) % s], points[(i + 2) % s]) <= 0)
				return false;
		return true;
	}

	bool contains(Point p) const {
		int s = points.size();
		double angle = 0;
		for (int i = 0; i < s; ++i) {
			angle += p.angle(points[i], points[(i + 1) % s]);
			//cout << "Point " << i << " : " << angle * 180 / pi << '\n';
		}
		if (eq(angle, -2 * pi))
			return true;

		for (int i = 0; i < s; ++i) {
			Line seg{ points[i], points[(i + 1) % s] };
			if (seg.a * p.x + seg.b * p.y == seg.c && p.inBox(points[i], points[(i + 1) % s]))
				return true;
		}

		return false;
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

	vector<Point> getConvexHull() {
		vector<Point> ch;
		vector<Point> sorted = points;

		Point* lowest = &points[0];
		for (Point p : points) {
			if (p.y < lowest->y)
				lowest = &p;
			if (p.y == lowest->y)
				if (p.x < lowest->x)
					lowest = &p;
		}

		sort(sorted.begin(), sorted.end(), Polygon::AngleComp{ *lowest });

		if (sorted.size() <= 3)
			return sorted;

		int k;
		for (k = 0; k < 2; ++k)
			ch.push_back(sorted[k]);

		for (; k < sorted.size();) {
			int s = ch.size();
			if (orient(ch[s - 2], ch[s - 1], sorted[k]) >= 0) {
				ch.push_back(sorted[k]);
				++k;
			} else
				ch.pop_back();
		}

		return ch;
	}
};