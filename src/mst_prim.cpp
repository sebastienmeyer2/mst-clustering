#include "mst_prim.hpp"

#include <iostream>

PrimAlgorithm::PrimAlgorithm(Graph* graph) : MSTAlgorithm(graph) {}

void PrimAlgorithm::compute_mst() 
{
    /*!
     * @brief Computes the Minimum Spanning Tree of the initial graph using 
     * Prim's algorithm. More specifically, initialize a priority queue to 
     * sort the edges by their weight. At each iteration, it adds the edge with 
     * minimal weight to the MST and checks for new edges to discover. 
     * 
     */

    this->mst_weight = 0.0;
    std::cout << "\nComputing the MST using Prim's algorithm...";
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int n = this->initial_graph->get_number_of_nodes();
    std::vector<Node*> nodes = this->initial_graph->get_nodes();

    auto cmp = [](const Edge* e1, const Edge* e2) 
        { 
            double w1 = e1->weight;
            double w2 = e2->weight;

            if (w1 < w2) {
                return true;
            } else if (w1 > w2) {
                return false;
            } else { // Allows multiple keys for same weight values
                intptr_t p1 = reinterpret_cast<intptr_t>(e1);
                intptr_t p2 = reinterpret_cast<intptr_t>(e2);

                return p1 < p2;
            }
        };

    // initialization of the priority queue (ie set), best edge per vertex and beginning vertex
    std::unordered_map<Node*, Edge*> min_edge;
    std::set<Edge*, decltype(cmp)> pq(cmp);

    std::unordered_set<Node*> visited_nodes;

    Node* current_node = this->initial_graph->get_any_node();
    visited_nodes.insert(current_node);
    this->mst_graph.add_node(current_node); // just to have the source node in the MST

    std::unordered_set<Edge*> connected_edges = this->initial_graph->connected_edges(current_node);

    for (Edge* e : connected_edges) {
        Node* other_node = e->other_node(current_node);

        if (visited_nodes.find(other_node) == visited_nodes.end()) {
            if (min_edge.find(other_node) != min_edge.end()) 
            {
                if (*e < *min_edge.at(other_node)) {
                    pq.erase(min_edge.at(other_node));
                    min_edge.erase(other_node);

                    min_edge.insert(std::pair<Node*, Edge*>(other_node, e));
                    pq.insert(e);
                }
            }
            else 
            {
                min_edge.insert(std::pair<Node*, Edge*>(other_node, e));
                pq.insert(e);
            }
        }
    }

    // filling the MST with new nodes until it forms a tree 
    while (this->mst_graph.get_number_of_nodes() < n) {

        // testing whether if the set is empty
        if (pq.empty()) {
            throw std::invalid_argument("No MST can be built !");
        }

        // adding the new edge and node to the MST and the visited nodes
        Edge* new_edge = *pq.begin();

        Node* p1 = new_edge->p1;
        Node* p2 = new_edge->p2;
        Node* new_node = (visited_nodes.find(p1) != visited_nodes.end()) ? p2 : p1;

        visited_nodes.insert(new_node);

        this->mst_graph.add_edge(new_edge);
        this->mst_weight += new_edge->weight;

        // we won't need the best edge for this node anymore
        min_edge.erase(new_node);
        pq.erase(new_edge);

        // updating the external nodes best edges
        connected_edges = this->initial_graph->connected_edges(new_node);

        for (Edge* e : connected_edges) {
            Node* other_node = e->other_node(new_node);

            if (visited_nodes.find(other_node) == visited_nodes.end()) {
                if (min_edge.find(other_node) != min_edge.end()) 
                {
                    if (*e < *min_edge.at(other_node)) {
                        pq.erase(min_edge.at(other_node));
                        min_edge.erase(other_node);

                        min_edge.insert(std::pair<Node*, Edge*>(other_node, e));
                        pq.insert(e);
                    }
                }
                else 
                {
                    min_edge.insert(std::pair<Node*, Edge*>(other_node, e));
                    pq.insert(e);
                }
            }
        }
    }
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "[OK]" << std::endl;
    this->treatment_done();

    std::cout << "Total weight of the MST: " << this->mst_weight << std::endl;

    std::cout << "Time spent by the algorithm: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
}
