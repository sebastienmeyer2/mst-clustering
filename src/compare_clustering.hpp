#pragma once 

#include "mst_kruskal.hpp"
#include "kmeans.hpp"

int nb_columns(const std::string &line);
void compare_clustering(std::string file_to_read, int k);
void compare_inconsistency(std::string file_to_read, double cutoff);