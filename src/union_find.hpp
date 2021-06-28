#pragma once

#include "graph.hpp"

#include <cassert>

class UnionFind 
{

    /*!
     * @class Union-Find data structure, which implements 
     * an efficient way of working with connected components 
     *
     */

protected:
    std::unordered_map<Node*, Node*> parent;
    std::unordered_map<Node*, int> rank;
    std::unordered_set<Node*> representatives;
    int num_classes;
    
public:
    UnionFind(std::vector<Node*> nodes);
    ~UnionFind();

    Node* get_parent(Node* node);
    int get_rank(Node* node);
    std::unordered_set<Node*> get_representatives();
    int get_num_classes();

    Node* Find(Node* node);
    void Union(Node* n1, Node* n2);
};