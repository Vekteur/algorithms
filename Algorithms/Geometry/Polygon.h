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

	vector<Point>& points() {
		return m_points;
	}

	const vector<Point>& points() const {
		return m_points;
	}

	double perimeter() const {
		int s = m_points.size();
		double p = 0;
		for (int i = 0; i < m_points.size(); ++i)
			p += m_points[i].dist(m_points[(i + 1) % s]);
		return p;
	}

	double area() {
		double result = 0;
		m_points.push_back(m_points[0]);
		for (int i = 0; i < m_points.size() - 1; ++i)
			result += m_points[i].x * m_points[i + 1].y - m_points[i + 1].x * m_points[i].y;
		m_points.pop_back();
		return result / 2;
	}

	bool isConvex() const // Suppose that the vertices are counter-clockwise
	{
		int s = m_points.size();
		for (int i = 0; i < s; ++i)
			if (orient(m_points[i], m_points[(i + 1) % s], m_points[(i + 2) % s]) <= 0)
				return false;
		return true;
	}

	bool contains(Point p) const {
		int s = m_points.size();
		double angle = 0;
		for (int i = 0; i < s; ++i) {
			angle += p.angle(m_points[i], m_points[(i + 1) % s]);
		}
		if (eq(angle, -2 * PI))
			return true;

		for (int i = 0; i < s; ++i) {
			Line seg{ m_points[i], m_points[(i + 1) % s] };
			if (seg.a * p.x + seg.b * p.y == seg.c && p.inBox(m_points[i], m_points[(i + 1) % s]))
				return true;
		}

		return false;
	}

	vector<Point> convexHull() {
		vector<Point> ch;
		vector<Point> sorted = m_points;

		Point lowest = m_points[0];
		for (Point p : m_points) {
			if (p.y < lowest.y || (p.y == lowest.y && p.x < lowest.x))
				lowest = p;
		}

		sort(sorted.begin(), sorted.end(), AngleComp{ lowest });

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
			} else {
				ch.pop_back();
			}
		}

		return ch;
	}
};