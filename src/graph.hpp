#pragma once

#include "edge.hpp"
#include "node.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <random>

class Graph{

    /*!
     * @class Décrit le graphe
     *
     */

    private:
        std::unordered_map<Node*, std::unordered_set<Edge*> > adjacency_lists;
        std::unordered_set<Edge*> all_edges;

        std::unordered_map<node_id_t, Node*> node_id_mapper;
        std::unordered_map<edge_id_t, Edge*> edge_id_mapper;

        node_id_t node_counter;
        edge_id_t edge_counter;
    
    public:
        Graph();
        Graph(std::string file_path);
        Graph(std::vector<Point*> points);

        void add_node(Node* n);
        void add_edge(Edge* e);
        bool has_node(Node* n);

        const std::unordered_set<Edge*>& connected_edges(Node* n);

        int get_number_of_nodes();
        std::vector<Node*> get_nodes();
        int get_number_of_edges();
        std::unordered_set<Edge*> get_edges();
        Node* get_any_node();

        Node* get_node(node_id_t id);
        Edge* get_edge(edge_id_t id);

        double total_weight();

};