#pragma once

#include "point.hpp"

#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <cfloat>

typedef int node_label_t;
typedef int node_id_t;

class Node{

    /*!
     * @class Describes a Node of the Graph
     * 
     */
    private:
        node_id_t id;

    protected:
        Point* p;

    public:
        node_label_t label;

        Node(node_label_t label);
        Node(node_label_t label, Point* p);

        Point* get_point();
        void set_point(Point* p);
        double dist(Node* other);

        void set_id(node_id_t id_);
        node_id_t get_id();
};

double intracluster_variance(std::unordered_map<Node*, Node*> clusters);
double silhouette(std::unordered_map<Node*, Node*> clusters);
