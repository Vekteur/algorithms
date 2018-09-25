#pragma once

#include <vector>
#include <algorithm>

#include "Geometry.h"
#include "Point.h"
#include "Line.h"

struct Polygon {
	vector<Point> m_points;

	Polygon& add(Point p) {
		m_points.push_back(p);
		return *this;
	}

	int size() const {
		return int(m_points.size());
	}

	double perimeter() const {
		double p = 0;
		for (int i = 0; i < size(); ++i)
			p += m_points[i].dist(m_points[(i + 1) % size()]);
		return p;
	}

	double area() const {
		double result = 0;
		for (int i = 0; i < size(); ++i)
			result += m_points[i].x * m_points[(i + 1) % size()].y - m_points[(i + 1) % size()].x * m_points[i].y;
		return result / 2;
	}

	bool isConvex() const { // Suppose that the vertices are counter-clockwise
		for (int i = 0; i < size(); ++i)
			if (orient(m_points[i], m_points[(i + 1) % size()], m_points[(i + 2) % size()]) <= 0)
				return false;
		return true;
	}

	bool contains(Point p) const {
		double angle = 0;
		for (int i = 0; i < size(); ++i) {
			angle += p.angle(m_points[i], m_points[(i + 1) % size()]);
		}
		// Strictly in the polygon
		if (eq(angle, -2 * PI))
			return true;

		for (int i = 0; i < size(); ++i) {
			Line seg{ m_points[i], m_points[(i + 1) % size()] };
			// On the bounds of the polygon
			if (seg.contains(p) && p.inBox(m_points[i], m_points[(i + 1) % size()]))
				return true;
		}
		return false;
	}

	vector<Point> convexHull() const {
		vector<Point> ch;
		vector<Point> sorted = m_points;

		Point lowest = m_points[0];
		for (Point p : m_points) {
			if (p.y < lowest.y || (p.y == lowest.y && p.x < lowest.x))
				lowest = p;
		}

		sort(sorted.begin(), sorted.end(), AngleComp{ lowest });

		if (int(sorted.size()) <= 3)
			return sorted;

		for (int k = 0; k < 2; ++k)
			ch.push_back(sorted[k]);

		for (int k = 2; k < int(sorted.size());) {
			int s = ch.size();
			if (orient(ch[s - 2], ch[s - 1], sorted[k]) >= 0) {
				ch.push_back(sorted[k]);
				++k;
			} else {
				ch.pop_back();
			}
		}

		return ch;
	}
};