#pragma once

#include "node.hpp"

#include <string>

typedef double edge_weight_t;
typedef int edge_id_t;

class Edge{

    /*!
     * @class Décrit une arrête du graphe
     *
     */

    public:
        Node* p1;
        Node* p2;
        edge_weight_t weight;

        node_id_t id;

        Edge(Node* p1, Node* p2, edge_weight_t weight);
        Edge(Node* p1, Node* p2);

        Node* other_node(Node* p);

        bool operator<(Edge const& other) const;
 
};