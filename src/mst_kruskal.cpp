#include "mst_kruskal.hpp"

KruskalAlgorithm::KruskalAlgorithm(Graph* graph) : MSTAlgorithm(graph) {}

void KruskalAlgorithm::compute_mst()
{
    /*!
     * @brief Computes the Minimum Spanning Tree of the graph associated to 
     * this instance of MSTAlgorithm, using Kruskal's algorithm. More specifically,
     * sorts all the edges by their weight, and uses an Union-Find data structure
     * to select which components to union at each iteration, until there is 
     * only one left, which is the MST.
     * 
     */

    this->mst_weight = 0.0;
    std::cout << "\nComputing the MST using Kruskal's algorithm...";
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int n = this->initial_graph->get_number_of_nodes();
    std::vector<Node*> nodes = this->initial_graph->get_nodes();
    std::unordered_set<Edge*> edges = this->initial_graph->get_edges();

    auto cmp = [](const Edge* e1, const Edge* e2) 
        { 
            double w1 = e1->weight;
            double w2 = e2->weight;

            if (w1 < w2) {
                return false; // reversed in order to follow priority queue's implementation
            } else if (w1 > w2) {
                return true;
            } else { // allows multiple keys for same weight values (arbitrary comp)
                intptr_t p1 = reinterpret_cast<intptr_t>(e1);
                intptr_t p2 = reinterpret_cast<intptr_t>(e2);

                return p1 < p2;
            }
        };

    // initialization of the algorithm
    std::priority_queue<Edge*, std::vector<Edge*>, decltype(cmp)> pq(cmp);
    for (Edge* e : edges) {
        pq.push(e);
    }

    UnionFind uf = UnionFind(nodes);

    // while there is more than one component, we add edges that are shared 
    int k = uf.get_num_classes();
    while (k > 1) {

        Edge* min_edge = pq.top();
        pq.pop();

        Node* rep1 = uf.Find(min_edge->p1);
        Node* rep2 = uf.Find(min_edge->p2);

        if (rep1 != rep2) {
            this->mst_graph.add_edge(min_edge);
            this->mst_weight += min_edge->weight;
            uf.Union(rep1, rep2);
        }

        k = uf.get_num_classes();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "[OK]" << std::endl;
    this->treatment_done();

    std::cout << "Total weight of the MST: " << this->mst_weight << std::endl;

    std::cout << "Time spent by the algorithm: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
}