#include "mst_prim_kumar.hpp"

#include <iostream>
#include <mpi.h>


#define IS_NODE_ALLOCATED_TO_ME(nodeid) (nodeid >= node_id_min && nodeid < node_id_max)

PrimKumarAlgorithm::PrimKumarAlgorithm(Graph* graph, int rank_, int world_size_) : MSTAlgorithm(graph) 
{
    /*!
     * @brief Builds the MST "solver"
     * 
     * @param graph The graph to use
     * @param rank_ The rank of the processor (MPI-related)
     * @param world_size_ The world size (MPI-related)
     *
     */
     
    this->rank = rank_;
    this->world_size = world_size_;
}


void PrimKumarAlgorithm::compute_mst() 
{
    /*!
     * @brief Computes the Minimum Spanning Tree of the initial graph using 
     * Prim-Kumar's algorithm. More specifically, initialize a priority queue to 
     * sort the edges by their weight. At each iteration, it adds the edge with 
     * minimal weight to the MST and checks for new edges to discover. 
     * 
     */


    int n_slaves = this->world_size;
    int n_nodes = this->initial_graph->get_number_of_nodes();

    int min_nodes_per_proc = n_nodes/(n_slaves);
    int split_nodes = n_nodes % n_slaves;

    int nodes_interval[2];

    nodes_interval[0] = this->rank*min_nodes_per_proc + (this->rank < split_nodes ? this->rank : split_nodes);  
    nodes_interval[1] = nodes_interval[0] + min_nodes_per_proc + (this->rank < split_nodes);

    int node_id_min = nodes_interval[0];
    int node_id_max = nodes_interval[1]; 

    struct {double d; int edge_id;} iter_res;
    struct {double d; int edge_id;} reduction_res;


    this->mst_weight = 0.0;

    std::chrono::steady_clock::time_point begin;
    if(this->rank == 0){
        std::cout << "Computing the MST using Prim-Kumar's algorithm...";
        begin = std::chrono::steady_clock::now();
    }

    int n = this->initial_graph->get_number_of_nodes();
    //std::vector<Node*> nodes = this->initial_graph->get_nodes();

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


    Node* current_node = this->initial_graph->get_node(0);




    this->mst_graph.add_node(current_node); // just to have the source node in the MST

    std::unordered_set<Edge*> connected_edges = this->initial_graph->connected_edges(current_node);

    for (Edge* e : connected_edges) {

        Node* other_node = e->other_node(current_node);
        if(IS_NODE_ALLOCATED_TO_ME(other_node->get_id())){
            if (!this->mst_graph.has_node(other_node)) {
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

    //std::printf("Proc %d, %d remaning items in queue\n", this->rank, pq.size());

    // filling the MST with new nodes until it forms a tree 
    while (this->mst_graph.get_number_of_nodes() < n) {

        Edge* new_edge;

        if(!pq.empty()){
            // adding the new edge and node to the MST and the visited nodes
            Edge* new_edge = *pq.begin();

            // We shall now send this to the root processor, so that it decides which edge is the best
            iter_res.edge_id = new_edge->id;
            iter_res.d = new_edge->weight;            
        } else {
            iter_res.d = std::numeric_limits<double>::infinity();
        }


        //std::printf("Proc %d, %d remaning items in queue\n", this->rank, pq.size());
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&iter_res, &reduction_res, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);


        // This is the REAL new edge
        new_edge = this->initial_graph->get_edge(reduction_res.edge_id);

        if(reduction_res.d == std::numeric_limits<double>::infinity()){
            printf("ARRRHS\n");
        } else {
            //printf("Elected edge %d\n", reduction_res.edge_id);
        }

        Node* p1 = new_edge->p1;
        Node* p2 = new_edge->p2;
        Node* new_node = (this->mst_graph.has_node(p1)) ? p2 : p1;

        this->mst_graph.add_edge(new_edge);
        this->mst_weight += new_edge->weight;



        this->mst_graph.add_node(new_node);
        
        pq.erase(new_edge);

        if(IS_NODE_ALLOCATED_TO_ME(p1->get_id()) || IS_NODE_ALLOCATED_TO_ME(p2->get_id())){
            // we won't need the best edge for this node anymore
            min_edge.erase(new_node);

        }


        // updating the external nodes best edges
        connected_edges = this->initial_graph->connected_edges(new_node);
            for (Edge* e : connected_edges) {
                Node* other_node = e->other_node(new_node);
                if(IS_NODE_ALLOCATED_TO_ME(other_node->get_id())){
                    if (!this->mst_graph.has_node(other_node)) {
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
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    if(this->rank == 0){
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "[OK]" << std::endl;
        this->treatment_done();

        std::cout << "Total weight of the MST: " << this->mst_weight << std::endl;

        std::cout << "Time spent by the algorithm: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
    }
}
