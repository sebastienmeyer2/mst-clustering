#pragma once 

#include "graph.hpp"
#include "union_find.hpp"

#include <queue>
#include <set>
#include <chrono>

class MSTAlgorithm 
{

    /*!
     * @class Abstract class for all the algorithms computing MST
     * 
     */

protected: 
    Graph* initial_graph;
    Graph mst_graph;
    double mst_weight;
    bool treated;

    void treatment_done();

public:
    MSTAlgorithm(Graph* graph);

    Graph* get_initial_graph();
    Graph* get_mst_graph();
    double get_mst_weight();
    bool is_treated();

    virtual void compute_mst() = 0;

    std::unordered_map<Node*, Node*> compute_clustering(int k);
    std::unordered_map<Node*, Node*> compute_clustering(double cutoff);
};