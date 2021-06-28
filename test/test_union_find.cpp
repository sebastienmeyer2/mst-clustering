#include "catch.hpp"

#include "../src/union_find.hpp"

TEST_CASE("Inizalition tests for Union Find data structure", "[uf:init]")
{
    Node absent_node = Node(11);

    // creating a vector of nodes
    std::vector<Node*> my_nodes;
    for(int i = 0; i < 10; i++) {
        my_nodes.push_back(new Node(i));
    }

    UnionFind uf = UnionFind(my_nodes);

    SECTION("Parent initialization")
    {
        REQUIRE(uf.get_parent(&absent_node) == NULL);

        for (int i = 0; i < 10; i ++) {
            REQUIRE(uf.get_parent(my_nodes[i]) == my_nodes[i]);
        }
    }

    SECTION("Rank initialization")
    {
        REQUIRE(uf.get_rank(&absent_node) == 0);

        for (int i = 0; i < 10; i ++) {
            REQUIRE(uf.get_rank(my_nodes[i]) == 1);
        }
    }

    SECTION("Representatives initialization")
    {
        std::unordered_set<Node*> representatives = uf.get_representatives();

        REQUIRE(representatives.find(&absent_node) == representatives.end());

        for (int i = 0; i < 10; i ++) {
            REQUIRE(representatives.find(my_nodes[i]) != representatives.end());
        }
    }

    SECTION("Number of classes initialization")
    {
        int nb_of_classes = uf.get_num_classes();

        REQUIRE(nb_of_classes == 10);
    }
}

TEST_CASE("Basic tests for Union Find data structure manipulation", "[uf:base]")
{
    // creating a vector of nodes
    std::vector<Node*> my_nodes;
    for(int i = 0; i < 10; i++){
        my_nodes.push_back(new Node(i));
    }

    UnionFind uf = UnionFind(my_nodes);

    SECTION("Find tests") 
    {
        REQUIRE(uf.Find(my_nodes[0]) == my_nodes[0]);
        REQUIRE(uf.Find(my_nodes[3]) != my_nodes[4]);
        REQUIRE(uf.Find(my_nodes[9]) == my_nodes[9]);
    }

    SECTION("Union tests")
    {
        uf.Union(my_nodes[0], my_nodes[1]);

        REQUIRE(uf.get_num_classes() == 9);

        // be careful: here, the union is arbitrary (second node is the parent)

        REQUIRE(uf.Find(my_nodes[0]) == my_nodes[1]);
        REQUIRE(uf.Find(my_nodes[1]) == my_nodes[1]);

        REQUIRE(uf.get_rank(my_nodes[1]) == 2);

        uf.Union(my_nodes[0], my_nodes[2]);

        REQUIRE(uf.get_num_classes() == 8);

        REQUIRE(uf.Find(my_nodes[2]) == my_nodes[1]);
        
        REQUIRE(uf.get_rank(my_nodes[1]) == 2);
        REQUIRE(uf.get_rank(my_nodes[2]) == 1);

        uf.Union(my_nodes[1], my_nodes[2]);

        REQUIRE(uf.get_num_classes() == 8);

        REQUIRE(uf.Find(my_nodes[1]) == my_nodes[1]);
        REQUIRE(uf.Find(my_nodes[2]) == my_nodes[1]);

        REQUIRE(uf.get_rank(my_nodes[1]) == 2);
        REQUIRE(uf.get_rank(my_nodes[2]) == 1);
    }
}