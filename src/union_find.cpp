#include "union_find.hpp"

// Constructor(s) / Destructor(s)

UnionFind::UnionFind(std::vector<Node*> nodes) 
{
    /*!
     * @brief Builds the Union-Find data structure from a vector
     * of nodes
     * 
     * @param nodes Nodes of the graph
     * 
     */

    this->num_classes = nodes.size();
    for(Node* n : nodes) {
        this->parent.insert(std::pair<Node*, Node*>(n, n));
        this->rank.insert(std::pair<Node*, int>(n, 1));
        this->representatives.insert(n);
    }
}

UnionFind::~UnionFind() {} 

// Getters

Node* UnionFind::get_parent(Node* node) 
{
    /*!
     * @brief Returns the parent of \p node in the data structure, if it exists
     * 
     * @param node Node of which we want the parent
     * 
     * @return NULL if the node is not part of the data structure, otherwise 
     * its parent
     * 
     */

    if(this->parent.find(node) == this->parent.end()) {
        return NULL;
    }
    return this->parent.at(node);
}

int UnionFind::get_rank(Node* node)
{
    /*!
     * @brief Returns the rank of \p node in the data structure, if it exists
     * 
     * @param node Node of which we want the rank
     * 
     * @return 0 if the node is not part of the data structure, otherwise 
     * its rank
     * 
     */

    if(this->rank.find(node) == this->rank.end()) {
        return 0;
    }
    return this->rank.at(node);
}

std::unordered_set<Node*> UnionFind::get_representatives()
{
    /*!
     * @brief Returns the representatives of components
     * 
     * @return A unordered set containing all the representatives left
     * 
     */

    return this->representatives;
}

int UnionFind::get_num_classes()
{
    /*!
     * @brief Returns the number of classes of this data structure
     *
     * @return num_classes from the data structure
     * 
     */

    return this->num_classes;
}

// Union & Find functions with all ameliorations (path compression, num classes, ...)

Node* UnionFind::Find(Node* node)
{
    /*!
     * @brief For a \p node that belongs to the data structure, find its 
     * representative in the data structure using path compression
     * 
     * @param node Node of which we want the reprensentative
     * 
     * @return The representative of the initial node
     * 
     */

    assert(("This node is not part of the data structure", this->get_parent(node) != NULL));

    Node* parent = this->get_parent(node);

    if(node == parent) {
        return parent;
    }

    Node* r = this->Find(parent);
    this->parent[node] = r;
    return r;
}

void UnionFind::Union(Node* n1, Node* n2)
{
    /*!
     * @brief For two nodes \p n1 and \p n2 of the data structure, computes 
     * the union of their two classes 
     * 
     * @param n1 First node 
     * @param n2 Second node
     * 
     */

    Node* rep1 = this->Find(n1);
    Node* rep2 = this->Find(n2);

    if(rep1 != rep2) {
        this->num_classes -= 1; // proceeding with Union

        int r1 = this->get_rank(rep1);
        int r2 = this->get_rank(rep2);

        if(r1 > r2) {
            this->parent[rep2] = rep1;
            this->representatives.erase(rep2);
        } else if(r2 > r1) {
            this->parent[rep1] = rep2;
            this->representatives.erase(rep1);
        } else { // arbitrary choice
            this->parent[rep1] = rep2;
            this->representatives.erase(rep1);
            this->rank[rep2] += 1;
        }
    }
}