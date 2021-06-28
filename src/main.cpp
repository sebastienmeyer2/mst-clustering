#include "graph.hpp"
#include "mst_prim.hpp"
#include "mst_boruvka.hpp"
#include "mst_kruskal.hpp"
#include "mst_prim_kumar.hpp"
#include "compare_clustering.hpp"

#include <mpi.h>

#include <vector>
#include <iostream>
#include <sstream>

void exit_with_help()
{
    std::printf(
    "Usage: build/main [options]\n"
    "options:\n"
    "-a : run Prim, Boruvka & Kruskal algorithms on generated graphs\n"
    "-c : run a comparison between MST clustering and k-means\n"
    "-i : run a comparison between Inconsistency clustering and k-means\n"
    "-m : run Prim with MPI\n"
    );
	exit(1);
}

int main(int argc, char** argv){

    bool run_mst_algo = false;
    bool run_mst_clust = false;
    bool run_mst_inc = false;
    bool run_mpi_prim = false;

    if (argc <= 1) 
    {
        exit_with_help();
    }
    
	for (int i = 1; i<argc; i++)
	{
		if(argv[i][0] != '-') exit_with_help();
		++i;
		switch(argv[i-1][1])
		{
			case 'a': run_mst_algo = true; break;
			case 'c': run_mst_clust = true; break;
			case 'i': run_mst_inc = true; break;
            case 'm': run_mpi_prim = true; break;
			default:
				std::fprintf(stderr,"unknown option\n");
				exit_with_help();
		}
	}

    /*
     * Tasks 
     */

    // TASKS 1 & 2

    if (run_mst_algo) 
    {
        // Comparing MST algorithms on Erdos-Rényi graphs

        std::vector<int> ER_graph_sizes({4, 8, 100, 250, 500, 1000});

        for (int n : ER_graph_sizes) 
        {
            Graph gER = Graph("data/ERn" + std::to_string(n) + "p10.txt");

            std::cout << "\nComparing MST algorithms with Erdos-Rényi graphs of size " << n << "." << std::endl;
            std::cout << "The graph contains " << gER.get_number_of_edges() << " edges." << std::endl;

            PrimAlgorithm primER = PrimAlgorithm(&gER);
            primER.compute_mst();

            BoruvkaAlgorithm boruvkaER = BoruvkaAlgorithm(&gER);
            boruvkaER.compute_mst();

            KruskalAlgorithm kruskalER = KruskalAlgorithm(&gER);
            kruskalER.compute_mst();
        }

        // Comparing MST algorithms on Barabasi-Albert graphs

        std::vector<int> BA_graph_sizes({100, 250, 500, 1000});

        for (int n : BA_graph_sizes) 
        {
            Graph gBA = Graph("data/BAn" + std::to_string(n) + "mo20m2.txt");

            std::cout << "\nComparing MST algorithms with Barabasi-Albert graphs of size " << n << "." << std::endl;
            std::cout << "The graph contains " << gBA.get_number_of_edges() << " edges." << std::endl;

            PrimAlgorithm primBA = PrimAlgorithm(&gBA);
            primBA.compute_mst();

            BoruvkaAlgorithm boruvkaBA = BoruvkaAlgorithm(&gBA);
            boruvkaBA.compute_mst();

            KruskalAlgorithm kruskalBA = KruskalAlgorithm(&gBA);
            kruskalBA.compute_mst();
        }
    }

    if (run_mpi_prim) 
    {
        // Comparing MST algorithms on Erdos-Rényi graphs

        std::vector<int> ER_graph_sizes({4, 8, 100, 250, 500, 1000});
    
        MPI_Init(NULL, NULL);

        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        
        

        for (int n : ER_graph_sizes) 
        {
            Graph gER = Graph("data/ERn" + std::to_string(n) + "p10.txt");

            std::cout << "\nComparing MST algorithms with Erdos-Rényi graphs of size " << n << "." << std::endl;
            std::cout << "The graph contains " << gER.get_number_of_edges() << " edges." << std::endl;

            PrimKumarAlgorithm primER = PrimKumarAlgorithm(&gER, world_rank, world_size);
            primER.compute_mst();


        }

        // Comparing MST algorithms on Barabasi-Albert graphs

        std::vector<int> BA_graph_sizes({100, 250, 500, 1000});

        for (int n : BA_graph_sizes) 
        {
            Graph gBA = Graph("data/BAn" + std::to_string(n) + "mo20m2.txt");

            std::cout << "\nComparing MST algorithms with Barabasi-Albert graphs of size " << n << "." << std::endl;
            std::cout << "The graph contains " << gBA.get_number_of_edges() << " edges." << std::endl;

            PrimKumarAlgorithm primBA = PrimKumarAlgorithm(&gBA, world_rank, world_size);
            primBA.compute_mst();
        }
        
        MPI_Finalize();
    }

    
    // TASK 5

    if (run_mst_clust) 
    {
        // Comparing MST clustering and k-means on Walmart dataset 

        std::vector<int> walmart_graph_sizes({100, 250, 500, 1000});
        std::vector<int> nb_clusters({3, 4, 5, 6, 7, 8});

        for (int n : walmart_graph_sizes)
        {
            for (int k : nb_clusters)
            {
                std::cout << "\nComparing MST clustering and k-means on graph of size " << n;
                std::cout << " and with " << k << " clusters" << std::endl;

                compare_clustering("data/scaled_n" + std::to_string(n) + "_walmart_features.txt", k);
            }
        }
    }

    // TASK 6 

    if (run_mst_inc)
    {
        // with Erdos-Rényi graphs 

        std::cout << "\nComparing different cutoffs on Erdos-Rényi graphs" << std::endl;
        std::vector<double> ER_cutoffs({1, 5, 10, 50, 100, 500, 1000});

        for (double c : ER_cutoffs) {
            Graph g = Graph("data/ERn1000p10.txt");
            KruskalAlgorithm k = KruskalAlgorithm(&g);

            std::unordered_map<Node*, Node*> clusters = k.compute_clustering(c);

            std::unordered_map<Node*, int> clusters_size;
            for (auto kv : clusters) {
                if(clusters_size.find(kv.second) != clusters_size.end()) {
                    clusters_size.at(kv.second) += 1;
                } else {
                    clusters_size.insert(std::pair<Node*, int>(kv.second, 1));
                }
            }
            std::cout << "In total, there are : " << clusters_size.size() << " clusters when cutoff = " << c << std::endl;

            int big_clusters = 0;
            std::cout << "The cluster sizes above 1 are the following : ";
            for (auto kv : clusters_size) {
                if (kv.second > 1) {
                    std::cout << kv.second << " ";
                    big_clusters += 1;
                }
            }
            std::cout << std::endl;
            std::cout << "There are " << big_clusters << " clusters of size above 1." << std::endl;
        }

        // with Walmart dataset

        std::cout << "\nComparing different cutoffs on Walmart dataset" << std::endl;
        std::vector<double> walmart_cutoffs({1, 5, 10, 50, 100, 500, 1000});

        for (double c : walmart_cutoffs) {
            compare_inconsistency("data/scaled_n1000_walmart_features.txt", c);
        }
    }

    return 0;
}
