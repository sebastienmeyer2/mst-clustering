#include "catch.hpp"

#include "../src/mst_algorithm.hpp"
#include "../src/mst_prim.hpp"
#include "../src/mst_boruvka.hpp"
#include "../src/mst_kruskal.hpp"

TEST_CASE("First test for MST construction")
{
    std::vector<Node*> my_nodes;
    for (int i = 1; i <= 7; i ++) {
        my_nodes.push_back(new Node(i));
    }

    std::vector<Edge*> my_edges;
    my_edges.push_back(new Edge(my_nodes[0], my_nodes[2], 6)); // Edge n°0
    my_edges.push_back(new Edge(my_nodes[0], my_nodes[3], 1)); // Edge n°1
    my_edges.push_back(new Edge(my_nodes[1], my_nodes[2], 6)); // Edge n°2
    my_edges.push_back(new Edge(my_nodes[1], my_nodes[4], 8)); // Edge n°3
    my_edges.push_back(new Edge(my_nodes[1], my_nodes[5], 4)); // Edge n°4
    my_edges.push_back(new Edge(my_nodes[2], my_nodes[3], 9)); // Edge n°5
    my_edges.push_back(new Edge(my_nodes[2], my_nodes[5], 2)); // Edge n°6
    my_edges.push_back(new Edge(my_nodes[3], my_nodes[4], 3)); // Edge n°7
    my_edges.push_back(new Edge(my_nodes[3], my_nodes[5], 3)); // Edge n°8
    my_edges.push_back(new Edge(my_nodes[3], my_nodes[6], 3)); // Edge n°9
    my_edges.push_back(new Edge(my_nodes[4], my_nodes[5], 5)); // Edge n°10
    my_edges.push_back(new Edge(my_nodes[4], my_nodes[6], 2)); // Edge n°11
    int E = my_edges.size();

    Graph g;
    // we add all edges, since every node has an edge they will all be in the graph
    for (int i = 0; i < E; i ++) {
        g.add_edge(my_edges[i]); 
    }

    SECTION("Prim's algorithm")
    {
        PrimAlgorithm prim = PrimAlgorithm(&g);
        prim.compute_mst();

        REQUIRE(std::abs(prim.get_mst_graph()->total_weight() - 15) < 0.001);

        std::unordered_set<Edge*> mst_edges = prim.get_mst_graph()->get_edges();

        REQUIRE(mst_edges.find(my_edges[0]) == mst_edges.end());  // Edge 1<-6->2
        REQUIRE(mst_edges.find(my_edges[1]) != mst_edges.end());  // Edge 1<-1->3
        REQUIRE(mst_edges.find(my_edges[2]) == mst_edges.end());  // Edge 2<-6->3
        REQUIRE(mst_edges.find(my_edges[3]) == mst_edges.end());  // Edge 2<-8->5
        REQUIRE(mst_edges.find(my_edges[4]) != mst_edges.end());  // Edge 2<-4->6
        REQUIRE(mst_edges.find(my_edges[5]) == mst_edges.end());  // Edge 3<-9->4
        REQUIRE(mst_edges.find(my_edges[6]) != mst_edges.end());  // Edge 3<-2->6
        // REQUIRE(mst_edges.find(my_edges[7]) == mst_edges.end());  
        REQUIRE(mst_edges.find(my_edges[8]) != mst_edges.end());  // Edge 4<-3->6
        // REQUIRE(mst_edges.find(my_edges[9]) != mst_edges.end());  
        REQUIRE(mst_edges.find(my_edges[10]) == mst_edges.end()); // Edge 5<-5->6
        REQUIRE(mst_edges.find(my_edges[11]) != mst_edges.end()); // Edge 5<-2->7

        // For Prim on this graph, there are two possibilities, given that the graph 
        // is initialized with a random Node, either the fourth Node will be connected 
        // to 5th or 7th, but 5th and 7th are always connected

        bool needed_edge = mst_edges.find(my_edges[7]) != mst_edges.end() || mst_edges.find(my_edges[9]) != mst_edges.end();
        bool limit_edge = mst_edges.find(my_edges[7]) == mst_edges.end() || mst_edges.find(my_edges[9]) == mst_edges.end();
        REQUIRE(limit_edge == true); 
        REQUIRE(needed_edge == true); 
    }

    SECTION("Boruvka's algorithm")
    {
        BoruvkaAlgorithm boruvka = BoruvkaAlgorithm(&g);
        boruvka.compute_mst();

        REQUIRE(std::abs(boruvka.get_mst_graph()->total_weight() - 15) < 0.001);

        std::unordered_set<Edge*> mst_edges = boruvka.get_mst_graph()->get_edges();

        REQUIRE(mst_edges.find(my_edges[0]) == mst_edges.end());  // Edge 1<-6->2
        REQUIRE(mst_edges.find(my_edges[1]) != mst_edges.end());  // Edge 1<-1->3
        REQUIRE(mst_edges.find(my_edges[2]) == mst_edges.end());  // Edge 2<-6->3
        REQUIRE(mst_edges.find(my_edges[3]) == mst_edges.end());  // Edge 2<-8->5
        REQUIRE(mst_edges.find(my_edges[4]) != mst_edges.end());  // Edge 2<-4->6
        REQUIRE(mst_edges.find(my_edges[5]) == mst_edges.end());  // Edge 3<-9->4
        REQUIRE(mst_edges.find(my_edges[6]) != mst_edges.end());  // Edge 3<-2->6
        // REQUIRE(mst_edges.find(my_edges[7]) == mst_edges.end());  
        REQUIRE(mst_edges.find(my_edges[8]) != mst_edges.end());  // Edge 4<-3->6
        // REQUIRE(mst_edges.find(my_edges[9]) != mst_edges.end());  
        REQUIRE(mst_edges.find(my_edges[10]) == mst_edges.end()); // Edge 5<-5->6
        REQUIRE(mst_edges.find(my_edges[11]) != mst_edges.end()); // Edge 5<-2->7

        // For Boruvka on this graph, there are two possibilities, given that the connected
        // components will at one step have two edges with weight 3 that link them, either 
        // the fourth Node will be connected to 5th or 7th, but 5th and 7th are always connected

        bool needed_edge = mst_edges.find(my_edges[7]) != mst_edges.end() || mst_edges.find(my_edges[9]) != mst_edges.end();
        bool limit_edge = mst_edges.find(my_edges[7]) == mst_edges.end() || mst_edges.find(my_edges[9]) == mst_edges.end();
        REQUIRE(limit_edge == true); 
        REQUIRE(needed_edge == true); 
    }

    SECTION("Kruskal's algorithm")
    {
        KruskalAlgorithm kruskal = KruskalAlgorithm(&g);
        kruskal.compute_mst();

        SECTION("MST computed with Kruskal's algorithm")
        {
            REQUIRE(std::abs(kruskal.get_mst_graph()->total_weight() - 15) < 0.001);

            std::unordered_set<Edge*> mst_edges = kruskal.get_mst_graph()->get_edges();

            REQUIRE(mst_edges.find(my_edges[0]) == mst_edges.end());  // Edge 1<-6->2
            REQUIRE(mst_edges.find(my_edges[1]) != mst_edges.end());  // Edge 1<-1->3
            REQUIRE(mst_edges.find(my_edges[2]) == mst_edges.end());  // Edge 2<-6->3
            REQUIRE(mst_edges.find(my_edges[3]) == mst_edges.end());  // Edge 2<-8->5
            REQUIRE(mst_edges.find(my_edges[4]) != mst_edges.end());  // Edge 2<-4->6
            REQUIRE(mst_edges.find(my_edges[5]) == mst_edges.end());  // Edge 3<-9->4
            REQUIRE(mst_edges.find(my_edges[6]) != mst_edges.end());  // Edge 3<-2->6
            // REQUIRE(mst_edges.find(my_edges[7]) == mst_edges.end());  
            REQUIRE(mst_edges.find(my_edges[8]) != mst_edges.end());  // Edge 4<-3->6
            // REQUIRE(mst_edges.find(my_edges[9]) != mst_edges.end());  
            REQUIRE(mst_edges.find(my_edges[10]) == mst_edges.end()); // Edge 5<-5->6
            REQUIRE(mst_edges.find(my_edges[11]) != mst_edges.end()); // Edge 5<-2->7

            // For Kruskal on this graph, there are two possibilities, because two edges
            // have the same weight and can lead to two different MST, in which the fourth 
            // node will be connected to either the 5th or 7th, but 5th and 7th are always connected

            bool needed_edge = mst_edges.find(my_edges[7]) != mst_edges.end() || mst_edges.find(my_edges[9]) != mst_edges.end();
            bool limit_edge = mst_edges.find(my_edges[7]) == mst_edges.end() || mst_edges.find(my_edges[9]) == mst_edges.end();
            REQUIRE(limit_edge == true); 
            REQUIRE(needed_edge == true); 
        }

        SECTION("Clustering using Kruskal's MST")
        {
            int k = 2;
            std::unordered_map<Node*, Node*> clustering_k2 = kruskal.compute_clustering(k);

            // The heaviest weight in the MST has to be 2<-4->6, thus we will have two groups of nodes
            REQUIRE(clustering_k2.at(my_nodes[1]) == my_nodes[1]);

            REQUIRE(clustering_k2.at(my_nodes[0]) != my_nodes[1]);
            REQUIRE(clustering_k2.at(my_nodes[5]) != my_nodes[1]);
            REQUIRE(clustering_k2.at(my_nodes[4]) != my_nodes[1]);

            k = 4;
            std::unordered_map<Node*, Node*> clustering_k4 = kruskal.compute_clustering(k);

            // The two heaviest edges in the MST are 2<-4->6 & 4<-4->6
            REQUIRE(clustering_k4.at(my_nodes[1]) == my_nodes[1]);

            REQUIRE(clustering_k4.at(my_nodes[2]) == clustering_k4.at(my_nodes[5]));
            REQUIRE(clustering_k4.at(my_nodes[2]) != my_nodes[0]);

            REQUIRE(clustering_k4.at(my_nodes[0]) == clustering_k4.at(my_nodes[3]));

            REQUIRE(clustering_k4.at(my_nodes[4]) == clustering_k4.at(my_nodes[6]));
        }
    }
}

TEST_CASE("Second test for MST construction")
{
    std::vector<Node*> my_nodes;
    for (int i = 0; i <= 8; i ++) {
        my_nodes.push_back(new Node(i));
    }

    std::vector<Edge*> my_edges;
    my_edges.push_back(new Edge(my_nodes[0], my_nodes[1], 4));  // Edge n°0
    my_edges.push_back(new Edge(my_nodes[0], my_nodes[7], 9));  // Edge n°1
    my_edges.push_back(new Edge(my_nodes[1], my_nodes[2], 8));  // Edge n°2
    my_edges.push_back(new Edge(my_nodes[1], my_nodes[7], 11)); // Edge n°3
    my_edges.push_back(new Edge(my_nodes[2], my_nodes[3], 7));  // Edge n°4
    my_edges.push_back(new Edge(my_nodes[2], my_nodes[5], 4));  // Edge n°5
    my_edges.push_back(new Edge(my_nodes[2], my_nodes[8], 2));  // Edge n°6
    my_edges.push_back(new Edge(my_nodes[3], my_nodes[4], 9));  // Edge n°7
    my_edges.push_back(new Edge(my_nodes[3], my_nodes[5], 14)); // Edge n°8
    my_edges.push_back(new Edge(my_nodes[4], my_nodes[5], 10)); // Edge n°9
    my_edges.push_back(new Edge(my_nodes[5], my_nodes[6], 2));  // Edge n°10
    my_edges.push_back(new Edge(my_nodes[6], my_nodes[7], 1));  // Edge n°11
    my_edges.push_back(new Edge(my_nodes[6], my_nodes[8], 6));  // Edge n°12
    my_edges.push_back(new Edge(my_nodes[7], my_nodes[8], 7));  // Edge n°13
    int E = my_edges.size();

    Graph g;
    // we add all edges, since every node has an edge they will all be in the graph
    for (int i = 0; i < E; i ++) {
        g.add_edge(my_edges[i]);
    }

    SECTION("Prim's algorithm")
    {
        PrimAlgorithm prim = PrimAlgorithm(&g);
        prim.compute_mst();

        REQUIRE(std::abs(prim.get_mst_graph()->total_weight() - 37) < 0.001);

        std::unordered_set<Edge*> mst_edges = prim.get_mst_graph()->get_edges();

        REQUIRE(mst_edges.find(my_edges[0]) != mst_edges.end()); // Edge 0<-4->1
        REQUIRE(mst_edges.find(my_edges[1]) == mst_edges.end()); // Edge 0<-9->7
        REQUIRE(mst_edges.find(my_edges[2]) != mst_edges.end()); // Edge 1<-8->2
        REQUIRE(mst_edges.find(my_edges[3]) == mst_edges.end()); // Edge 1<-11->7
        REQUIRE(mst_edges.find(my_edges[4]) != mst_edges.end()); // Edge 2<-7->3
        REQUIRE(mst_edges.find(my_edges[5]) != mst_edges.end()); // Edge 2<-4->5
        REQUIRE(mst_edges.find(my_edges[6]) != mst_edges.end()); // Edge 2<-2->8
        REQUIRE(mst_edges.find(my_edges[7]) != mst_edges.end()); // Edge 3<-9->4
        REQUIRE(mst_edges.find(my_edges[8]) == mst_edges.end()); // Edge 3<-14->5
        REQUIRE(mst_edges.find(my_edges[9]) == mst_edges.end()); // Edge 4<-10->5
        REQUIRE(mst_edges.find(my_edges[10]) != mst_edges.end()); // Edge 5<-2->6
        REQUIRE(mst_edges.find(my_edges[11]) != mst_edges.end()); // Edge 6<-1->7
        REQUIRE(mst_edges.find(my_edges[12]) == mst_edges.end()); // Edge 6<-6->8
        REQUIRE(mst_edges.find(my_edges[13]) == mst_edges.end()); // Edge 7<-7->8
    }

    SECTION("Boruvka's algorithm")
    {
        BoruvkaAlgorithm boruvka = BoruvkaAlgorithm(&g);
        boruvka.compute_mst();

        REQUIRE(std::abs(boruvka.get_mst_graph()->total_weight() - 37) < 0.001);

        std::unordered_set<Edge*> mst_edges = boruvka.get_mst_graph()->get_edges();

        REQUIRE(mst_edges.find(my_edges[0]) != mst_edges.end()); // Edge 0<-4->1
        REQUIRE(mst_edges.find(my_edges[1]) == mst_edges.end()); // Edge 0<-9->7
        REQUIRE(mst_edges.find(my_edges[2]) != mst_edges.end()); // Edge 1<-8->2
        REQUIRE(mst_edges.find(my_edges[3]) == mst_edges.end()); // Edge 1<-11->7
        REQUIRE(mst_edges.find(my_edges[4]) != mst_edges.end()); // Edge 2<-7->3
        REQUIRE(mst_edges.find(my_edges[5]) != mst_edges.end()); // Edge 2<-4->5
        REQUIRE(mst_edges.find(my_edges[6]) != mst_edges.end()); // Edge 2<-2->8
        REQUIRE(mst_edges.find(my_edges[7]) != mst_edges.end()); // Edge 3<-9->4
        REQUIRE(mst_edges.find(my_edges[8]) == mst_edges.end()); // Edge 3<-14->5
        REQUIRE(mst_edges.find(my_edges[9]) == mst_edges.end()); // Edge 4<-10->5
        REQUIRE(mst_edges.find(my_edges[10]) != mst_edges.end()); // Edge 5<-2->6
        REQUIRE(mst_edges.find(my_edges[11]) != mst_edges.end()); // Edge 6<-1->7
        REQUIRE(mst_edges.find(my_edges[12]) == mst_edges.end()); // Edge 6<-6->8
        REQUIRE(mst_edges.find(my_edges[13]) == mst_edges.end()); // Edge 7<-7->8
    }

    SECTION("Kruskal's algorithm")
    {
        KruskalAlgorithm kruskal = KruskalAlgorithm(&g);
        kruskal.compute_mst();

        SECTION("MST computed with Kruskal's algorithm")
        {
            REQUIRE(std::abs(kruskal.get_mst_graph()->total_weight() - 37) < 0.001);

            std::unordered_set<Edge*> mst_edges = kruskal.get_mst_graph()->get_edges();

            REQUIRE(mst_edges.find(my_edges[0]) != mst_edges.end()); // Edge 0<-4->1
            REQUIRE(mst_edges.find(my_edges[1]) == mst_edges.end()); // Edge 0<-9->7
            REQUIRE(mst_edges.find(my_edges[2]) != mst_edges.end()); // Edge 1<-8->2
            REQUIRE(mst_edges.find(my_edges[3]) == mst_edges.end()); // Edge 1<-11->7
            REQUIRE(mst_edges.find(my_edges[4]) != mst_edges.end()); // Edge 2<-7->3
            REQUIRE(mst_edges.find(my_edges[5]) != mst_edges.end()); // Edge 2<-4->5
            REQUIRE(mst_edges.find(my_edges[6]) != mst_edges.end()); // Edge 2<-2->8
            REQUIRE(mst_edges.find(my_edges[7]) != mst_edges.end()); // Edge 3<-9->4
            REQUIRE(mst_edges.find(my_edges[8]) == mst_edges.end()); // Edge 3<-14->5
            REQUIRE(mst_edges.find(my_edges[9]) == mst_edges.end()); // Edge 4<-10->5
            REQUIRE(mst_edges.find(my_edges[10]) != mst_edges.end()); // Edge 5<-2->6
            REQUIRE(mst_edges.find(my_edges[11]) != mst_edges.end()); // Edge 6<-1->7
            REQUIRE(mst_edges.find(my_edges[12]) == mst_edges.end()); // Edge 6<-6->8
            REQUIRE(mst_edges.find(my_edges[13]) == mst_edges.end()); // Edge 7<-7->8
        }

        SECTION("Clustering using Kruskal's MST")
        {
            int k = 2;
            std::unordered_map<Node*, Node*> clustering_k2 = kruskal.compute_clustering(k);

            // The heaviest edge in this MST is 3<-9->4
            REQUIRE(clustering_k2.at(my_nodes[4]) == my_nodes[4]);
            REQUIRE(clustering_k2.at(my_nodes[4]) != my_nodes[7]);

            REQUIRE(clustering_k2.at(my_nodes[0]) == my_nodes[7]);
            REQUIRE(clustering_k2.at(my_nodes[3]) == my_nodes[7]);
            REQUIRE(clustering_k2.at(my_nodes[7]) == my_nodes[7]);

            k = 3;
            std::unordered_map<Node*, Node*> clustering_k3 = kruskal.compute_clustering(k);

            // The two heaviest edges in this MST are 3<-9->4 & 1<-8->2
            REQUIRE(clustering_k3.at(my_nodes[4]) == my_nodes[4]);
            REQUIRE(clustering_k3.at(my_nodes[4]) != my_nodes[7]);

            REQUIRE(clustering_k3.at(my_nodes[0]) == clustering_k3.at(my_nodes[1]));
            REQUIRE(clustering_k3.at(my_nodes[0]) != my_nodes[7]);
            
            REQUIRE(clustering_k3.at(my_nodes[3]) == my_nodes[7]);
            REQUIRE(clustering_k3.at(my_nodes[7]) == my_nodes[7]);
        }
    }
}