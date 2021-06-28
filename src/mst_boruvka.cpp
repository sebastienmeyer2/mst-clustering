#include "mst_boruvka.hpp"

BoruvkaAlgorithm::BoruvkaAlgorithm(Graph* graph) : MSTAlgorithm(graph) {}

void BoruvkaAlgorithm::compute_mst() 
{
    /*!
     * @brief Computes the Minimum Spanning Tree of the graph associated to 
     * this instance of MSTAlgorithm, using Boruvka's algorithm. More specifically,
     * initializes a Union-Find data structure with all the nodes of the graph, 
     * and unions connected components of the MST with respect to their edge of 
     * minimal weight, until there is only one component left.
     * 
     */

    this->mst_weight = 0.0;
    std::cout << "\nComputing MST using Boruvka's algorithm...";
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // initialization of the algorithm
    std::unordered_set<Edge*> edges = this->initial_graph->get_edges();
    std::vector<Node*> nodes = this->initial_graph->get_nodes();
    for(Node* n : nodes) {
        this->mst_graph.add_node(n);
    }

    UnionFind uf = UnionFind(nodes);

    // filling the MST with edges until there is only one component left
    int k = uf.get_num_classes();
    while (k > 1) {

        // for each component, we will keep track of the external edge with minimal weight
        std::unordered_set<Node*> representatives = uf.get_representatives();
        std::unordered_map<Node*, Edge*> min_edges;
        for (Node* rep : representatives) {
            min_edges.insert(std::pair<Node*, Edge*>(rep, NULL));
        }

        // getting all the edges with minimal weights according to components (not best complexity here)
        for (Edge*  e : edges) {

            Node* n_in = e->p1;
            Node* n_out = e->p2;

            Node* rep_in = uf.Find(n_in);
            Node* rep_out = uf.Find(n_out);

            if (rep_in == rep_out) {
                continue;
            }

            if (min_edges.at(rep_in) == NULL || min_edges.at(rep_in)->weight > e->weight) {
                min_edges.erase(rep_in);
                min_edges.insert(std::pair<Node*, Edge*>(rep_in, e));
            }

            if (min_edges.at(rep_out) == NULL || min_edges.at(rep_out)->weight > e->weight) {
                min_edges.erase(rep_out);
                min_edges.insert(std::pair<Node*, Edge*>(rep_out, e));
            }
        
        }

        // linking components
        for (Node* rep : representatives) {
            Edge* min_edge = min_edges.at(rep);

            Node* rep1 = uf.Find(min_edge->p1);
            Node* rep2 = uf.Find(min_edge->p2);

            if (rep1 != rep2) {
                this->mst_graph.add_edge(min_edge);
                this->mst_weight += min_edge->weight;
                uf.Union(rep1, rep2);
            }
        }

        k = uf.get_num_classes();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "[OK]" << std::endl;
    this->treatment_done();

    std::cout << "Total weight of the MST: " << this->mst_weight << std::endl;

    std::cout << "Time spent by the algorithm: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
}