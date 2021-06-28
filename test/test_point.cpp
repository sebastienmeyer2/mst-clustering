#include "catch.hpp"

#include "../src/point.hpp"

TEST_CASE("Simple point initialization", "[point:init]")
{
    Point::d = 2;

    SECTION("Empty point")
    {
        Point p;

        REQUIRE(p.label == 0);
        REQUIRE(p.d == 2);
    }

    SECTION("Complete point")
    {
        Point p;

        p.coords[0] = 9;

        REQUIRE(p.coords[0] == 9);
        REQUIRE(p.coords[1] == 0);
    }

    SECTION("Dimension getter")
    {
        Point p;

        int d = p.get_dim();

        REQUIRE(d == 2);
    }
}

TEST_CASE("Computation of distances between points", "[point:dist]")
{
    Point::d = 2;

    Point p1;
    p1.coords[0] = 0;
    p1.coords[1] = 2;

    Point p2;
    p2.coords[0] = 2;
    p2.coords[1] = 0;

    double d = p1.distance(&p2);

    REQUIRE((d - 2.828) < 0.001);
}