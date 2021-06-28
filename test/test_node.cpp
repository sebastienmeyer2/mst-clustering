#include "catch.hpp"

#include "../src/point.hpp"
#include "../src/node.hpp"

TEST_CASE("Simple node initialization", "[node:init]") 
{
    Point::d = 2;

    SECTION("Node with no assigned point")
    {
        Node n = Node(3);

        REQUIRE(n.label == 3);
        REQUIRE(n.get_point() == NULL);
    }


    SECTION("Nodes with assigned point")
    {
        Point p;
        p.coords[0] = 2;
        p.coords[1] = 3;

        Node n = Node(1);

        n.set_point(&p);
        REQUIRE(n.get_point()->coords[0] == 2);
        
        Node n2 = Node(1, &p);
        REQUIRE(n2.get_point()->coords[0] == 2);
    }
}

TEST_CASE("Distance between nodes", "[node:dist]")
{
    Point::d = 2;

    SECTION("Both nodes are empty")
    {
        Node n1 = Node(1);
        Node n2 = Node(2);

        REQUIRE(n1.dist(&n2) == 1); // arbitrary choice in node.cpp
    }

    SECTION("At least one node is empty")
    {
        Node n1 = Node(1);

        Point p2;
        p2.coords[0] = 1;
        p2.coords[1] = 2;

        Node n2 = Node(2, &p2);

        REQUIRE(n1.dist(&n2) == 1);
    }

    SECTION("Both nodes have assigned points")
    {
        Point p1;
        p1.coords[0] = 0;
        p1.coords[1] = 2;

        Point p2;
        p2.coords[0] = 2;
        p2.coords[1] = 0;

        Node n1 = Node(1, &p1);
        Node n2 = Node(2, &p2);

        REQUIRE((n1.dist(&n2) - 2.828) < 0.001);
    }
}