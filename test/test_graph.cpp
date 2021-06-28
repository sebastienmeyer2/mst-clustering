#include "catch.hpp" 

#include "../src/graph.hpp"
#include <unordered_set>

TEST_CASE("Simple graph construction", "[graph:init]") 
{
    std::vector<Node> my_nodes;
    std::vector<Edge> my_edges;

    // Creating nodes "i"
    for(int i = 0; i < 10; i++){
        my_nodes.push_back(Node(i));
    }

    // Edges between "i" and "9-i" with weight 2*i
    for(int i = 0; i < 10; i++){
        my_edges.push_back(Edge(
            &my_nodes[i],
            &my_nodes[9-i],
            2*i
        ));
    }

    Graph my_graph;

    // Adding edges
    for(int i = 0; i < 10; i++){
        my_graph.add_edge(&my_edges[i]);
    }

    SECTION("Tests of nodes") 
    {

        Node not_in_graph = Node(10);
        Node duplicate = Node(5);

        REQUIRE(my_graph.has_node(&my_nodes[5]) == true);
        REQUIRE(my_graph.has_node(&not_in_graph) == false);
        REQUIRE(my_graph.has_node(&duplicate) == false); // Pointer test

    }

    SECTION("Tests of edges- Will not expand the iterator of the set but works") 
    {

        std::unordered_set<Edge*> set1 = my_graph.connected_edges(&my_nodes[5]);
        Edge not_in_graph = Edge(&my_nodes[1], &my_nodes[5]);
        Edge duplicate = Edge(&my_nodes[0], &my_nodes[9]);

        REQUIRE(set1.find(&my_edges[5]) != set1.end());
        REQUIRE(set1.find(&my_edges[0]) == set1.end());
        REQUIRE(set1.find(&not_in_graph) == set1.end());
        REQUIRE(set1.find(&duplicate) == set1.end()); // Pointer test

    }

    SECTION("Tests to see if all edges are in memory")
    {
        std::unordered_set<Edge*> all_edges = my_graph.get_edges();

        for (int i = 0; i < 10; i ++) {
            REQUIRE(all_edges.find(&my_edges[i]) != all_edges.end());
        }

        REQUIRE(all_edges.size() == 10);
    }
}

TEST_CASE("Tests of complex getters", "[graph:getters]")
{
    std::vector<Node> my_nodes;
    std::vector<Edge> my_edges;

    // Creating nodes "i"
    for(int i = 0; i < 10; i++){
        my_nodes.push_back(Node(i));
    }

    // Edges between "i" and "9-i" with weight 2*i
    for(int i = 0; i < 10; i++){
        my_edges.push_back(Edge(
            &my_nodes[i],
            &my_nodes[9-i],
            2*i
        ));
    }

    Graph my_graph;

    // Adding edges
    for(int i = 0; i < 10; i++){
        my_graph.add_edge(&my_edges[i]);
    }

    SECTION("Number of nodes getter") 
    {
        int n = my_graph.get_number_of_nodes();

        REQUIRE(n == 10);
    }

    SECTION("Getter of any node")
    {
        // We will just test whether the Node is in the graph

        Node* node = my_graph.get_any_node();

        bool node_in_graph = false; 

        for (int i = 0; i < 10; i ++) {
            if (node == &my_nodes[i]) {
                node_in_graph = true;
            }
        }

        REQUIRE(node_in_graph == true);
    }

    SECTION("Nodes getter")
    {
        std::vector<Node*> nodes = my_graph.get_nodes();

        REQUIRE(nodes.size() == 10);

        int nodes_in_graph = 0;

        for (int i = 0; i < 10; i ++) {
            for (int j = 0; j < 10; j ++) {
                if (nodes[i] == &my_nodes[j]) {
                    nodes_in_graph += 1;
                }
            }
        }

        REQUIRE(nodes_in_graph == 10);
    }
}

TEST_CASE("Tests for the total weight computation", "[graph:weight]")
{
    std::vector<Node> my_nodes;
    std::vector<Edge> my_edges;

    // Creating nodes "i"
    for(int i = 0; i < 10; i++){
        my_nodes.push_back(Node(i));
    }

    // Edges between "i" and "9-i" with weight 2*i
    for(int i = 0; i < 10; i++){
        my_edges.push_back(Edge(
            &my_nodes[i],
            &my_nodes[9-i],
            2*i
        ));
    }

    Graph my_graph;

    // Adding edges
    for(int i = 0; i < 10; i++){
        my_graph.add_edge(&my_edges[i]);
    }

    double weight = my_graph.total_weight();

    REQUIRE(std::abs(weight - 90) < 0.001);
}