#pragma once

#include "mst_algorithm.hpp"
#include "union_find.hpp"

#include <iostream>
#include <queue>

class KruskalAlgorithm : public MSTAlgorithm
{
public:
    KruskalAlgorithm(Graph* graph);

    virtual void compute_mst();
};