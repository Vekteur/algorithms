#include "catch.hpp"

#include "Geometry/Point.h"
#include "Geometry/Line.h"
#include "Geometry/Polygon.h"

using namespace std;

TEST_CASE("Geometry") {
	Point p1(1, 2);
	Point p2(3, 5);
	Point q1(1, 4);
	Point q2(3, 3);
	Line l1(p1, p2);
	Line l2(q1, q2);
	SECTION("Angle") {
		Point p(1, 1), q(0, 0), r(1, -1);
		REQUIRE(!Point(1, 2).inAngle(p, q, r));
		REQUIRE(Point(2, 1).inAngle(p, q, r));
		REQUIRE(q.angle(p, r) == Approx(PI / 2));
		REQUIRE(Point(3, 1).angle(r, p) == Approx(PI / 4));
		REQUIRE(Point(3, 1).angle(p, r) == Approx(-PI / 4));
	}
	SECTION("Intersection") {
		REQUIRE(l2 == Line(0.5, 1., 4.5));
		REQUIRE(l1.inter(l2) == Point(2, 3.5));
	}
	SECTION("Distance") {
		REQUIRE(p1.dist(p2) == Approx(sqrt(13)));
		REQUIRE(l1.dist(q1) == Approx(l1.dist(q2)));
	}
	SECTION("Symetry") {
		Point r(-1, 0);
		REQUIRE(l2.perp(r) == Line(r, q1));
		REQUIRE(l2.sym(r) == Point(3, 8));
		REQUIRE(Line::horizontal(3).sym(r) == Point(-1, 6));
	}
	SECTION("Polygon") {
		Polygon p;
		p.add(q1).add(p1).add(q2).add(p2);
		REQUIRE(p.perimeter() == Approx(4 + 2 * sqrt(5)));
		REQUIRE(p.area() == Approx(4));
		Point r(l1.inter(l2));
		REQUIRE(p.contains(r));
		REQUIRE(!p.contains(Point(2, 2)));
		REQUIRE(p.isConvex());
		p.add(r);
		REQUIRE(!p.isConvex());
		REQUIRE(p.convexHull().size() == 4);
	}
}