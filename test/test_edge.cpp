#include "catch.hpp"

#include "../src/edge.hpp"

TEST_CASE("Simple edge initialization", "[edge:init]") 
{
    Point::d = 2;

    Node n1 = Node(1);
    Node n2 = Node(5);

    SECTION("Without input weight and no points") 
    {
        Edge edge_no_weight = Edge(&n1, &n2);

        REQUIRE(edge_no_weight.p1->label == 1);
        REQUIRE(edge_no_weight.p2->label == 5);
        REQUIRE(std::abs(edge_no_weight.weight - 1) < 0.001);
    }

    SECTION("Without input weight and with points")
    {
        Point p1;
        p1.coords[0] = 0;
        p1.coords[1] = 2;

        Point p2;
        p2.coords[0] = 2;
        p2.coords[1] = 0;

        n1.set_point(&p1);
        n2.set_point(&p2); 

        Edge edge_with_points = Edge(&n1, &n2);

        REQUIRE(edge_with_points.p1->label == 1);
        REQUIRE(edge_with_points.p2->label == 5);
        REQUIRE(std::abs(edge_with_points.weight - 2.828) < 0.001);
    }

    SECTION("With input weight") 
    {
        Edge edge_weight = Edge(&n1, &n2, 10);

        REQUIRE(edge_weight.p1->label == 1);
        REQUIRE(edge_weight.p2->label == 5);
        REQUIRE(std::abs(edge_weight.weight - 10) < 0.001);
    }
}

TEST_CASE("Getter of other node", "[edge:other_node]")
{
    Node p1 = Node(1);
    Node p2 = Node(2);
    Node p3 = Node(3);
    Node* p0 = NULL;

    Edge e1 = Edge(&p1, &p2);

    REQUIRE(e1.other_node(&p1) == &p2);
    REQUIRE(e1.other_node(&p3) == NULL);
    REQUIRE(e1.other_node(p0) == NULL);
}

TEST_CASE("Edge operator for <", "[edge:<]")
{
    Point::d = 2;

    Node n1 = Node(1);
    Node n2 = Node(2);

    Edge edge_no_weight = Edge(&n1, &n2);

    Node n3 = Node(3);
    Node n4 = Node(4);

    Point p1;
    p1.coords[0] = 0;
    p1.coords[1] = 2;

    Point p2;
    p2.coords[0] = 2;
    p2.coords[1] = 0;

    n3.set_point(&p1);
    n4.set_point(&p2);

    Edge edge_with_points = Edge(&n3, &n4);

    REQUIRE(edge_no_weight < edge_with_points);
}