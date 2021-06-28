#include "mst_algorithm.hpp"

MSTAlgorithm::MSTAlgorithm(Graph* graph)
{
    /*!
     * @brief Constructor of a MST Algorithm
     * 
     * @param graph A pointer towards the graph of which we want to compute the MST
     * 
     */

    this->initial_graph = graph;
    this->mst_weight = 0.0;
    this->treated = false;
}

Graph* MSTAlgorithm::get_initial_graph()
{
    /*!
     * @brief Gets the (protected) pointer of the initial graph
     * 
     * @return Initial graph's pointer
     *  
     */

    return this->initial_graph;
}

Graph* MSTAlgorithm::get_mst_graph()
{
    /*!
     * @brief Gets the (protected) pointer of the MST, if it has been computed
     * 
     * @return MST's pointer
     *  
     */

    return (this->is_treated()) ? &this->mst_graph : NULL;
}

double MSTAlgorithm::get_mst_weight()
{
    /*!
     * @brief Gets the (protected) pointer of the MST weight, if it has been computed
     * 
     * @return 0.0 if the MST has not been computed, otherwise its total weight
     *  
     */

    return this->mst_weight;
}

bool MSTAlgorithm::is_treated() 
{
    /*!
     * @brief Gets the boolean value checking whether if the MST has been computed
     * 
     * @return Value of treated
     *  
     */

    return this->treated;
}

void MSTAlgorithm::treatment_done()
{
    /*!
     * @brief Sets the boolean value of treated to true
     * 
     */

    this->treated = true;
}

std::unordered_map<Node*, Node*> MSTAlgorithm::compute_clustering(int k)
{
    /*!
     * @brief Creates a mapping between nodes of the MST (and initial) graph 
     * after it is built, and their representatives where we have eliminated 
     * the k-1 most expansive edges from the MST graph. 
     * 
     * @param k The number of clusters to build 
     * 
     * @return A mapping between nodes of the graph(s) and their representatives 
     * after removing k-1 edges
     * 
     */

    if (!this->treated) {
        this->compute_mst();
    }

    int n = this->mst_graph.get_number_of_nodes();
    assert(("There are less points than clusters!", k <= n));

    auto cmp = [](const Edge* e1, const Edge* e2) 
        { 
            double w1 = e1->weight;
            double w2 = e2->weight;

            if (w1 < w2) {
                return false;
            } else if (w1 > w2) {
                return true;
            } else { // Allows multiple keys for same weight values
                intptr_t p1 = reinterpret_cast<intptr_t>(e1);
                intptr_t p2 = reinterpret_cast<intptr_t>(e2);

                return p1 < p2;
            }
        };

    // builds an ordered queue of the edges of the MST graph
    std::unordered_set<Edge*> mst_edges = this->mst_graph.get_edges();

    std::priority_queue<Edge*, std::vector<Edge*>, decltype(cmp)> pq(cmp);

    for (Edge* e : mst_edges) {
        pq.push(e);
    }

    // selects only the E-(k-1) lightest edges of the MST graph
    std::vector<Node*> nodes = this->mst_graph.get_nodes();
    UnionFind uf = UnionFind(nodes);

    int nb_edges = (n-1) - (k-1); // MST is a tree 

    for (int i = 0; i < nb_edges; i ++) {
        Edge* min_edge = pq.top();
        uf.Union(min_edge->p1, min_edge->p2);
        pq.pop();
    }

    // builds a map from all the nodes to their representatives 
    std::unordered_map<Node*, Node*> clusters;

    for (Node* node : nodes) {
        Node* rep = uf.Find(node);

        clusters.insert(std::pair<Node*, Node*>(node, rep));
    }

    return clusters;
}

std::unordered_map<Node*, Node*> MSTAlgorithm::compute_clustering(double cutoff)
{
    /*!
     * @brief Creates a mapping between nodes of the MST (and initial) graph 
     * after it is built, and their representatives where we have eliminated 
     * the edges whose weights have a standard deviation larger than cutoff 
     * times the average weights of the other edges connected to its nodes
     * 
     * @param cutoff The scale to eliminate edges
     * 
     * @return A mapping between nodes of the graph(s) and their representatives 
     * after removing inconsistent edges
     * 
     */

    if (!this->treated) {
        this->compute_mst();
    }

    auto cmp = [](const Edge* e1, const Edge* e2) 
        { 
            double w1 = e1->weight;
            double w2 = e2->weight;

            if (w1 < w2) {
                return false;
            } else if (w1 > w2) {
                return true;
            } else { // Allows multiple keys for same weight values
                intptr_t p1 = reinterpret_cast<intptr_t>(e1);
                intptr_t p2 = reinterpret_cast<intptr_t>(e2);

                return p1 < p2;
            }
        };

    // builds an union-find ds and initializes the edges
    std::unordered_set<Edge*> mst_edges = this->mst_graph.get_edges();

    std::vector<Node*> nodes = this->mst_graph.get_nodes();
    UnionFind uf = UnionFind(nodes);

    // passes through the MST edges and keeps the consistent edges
    
    for (Edge* e : mst_edges) {
        bool add_edge = true;

        Node* p1 = e->p1;
        Node* p2 = e->p2;

        double avg_weight = 0.0;
        double std_dev = 0.0;
        double cnt = 0.0;

        double edge_std_dev = 0.0;

        // computes mean weight and std deviation for edges on both sides
        for (Edge* e1 : this->mst_graph.connected_edges(p1)) {
            if (e1 != e) {
                double edge_weight = e1->weight;
                avg_weight += edge_weight;
                std_dev += edge_weight*edge_weight;
                cnt += 1;
            }
        }

        for (Edge* e2 : this->mst_graph.connected_edges(p2)) {
            if (e2 != e) {
                double edge_weight = e2->weight;
                avg_weight += edge_weight;
                std_dev += edge_weight*edge_weight;
                cnt += 1;
            }
        }

        avg_weight /= cnt;
        std_dev = std::sqrt((std_dev/cnt) - avg_weight*avg_weight);

        // discards the considered edge if its std deviation is too large 
        edge_std_dev = std::abs(e->weight - avg_weight);
        if (edge_std_dev >= cutoff*std_dev) {
            add_edge = false;
        }

        // adds the edge if it is consistent
        if (add_edge == true) {
            uf.Union(p1, p2);
        }
    }

    // builds a map from all the nodes to their representatives 
    std::unordered_map<Node*, Node*> clusters;

    for (Node* node : nodes) {
        Node* rep = uf.Find(node);

        clusters.insert(std::pair<Node*, Node*>(node, rep));
    }

    return clusters;
}
