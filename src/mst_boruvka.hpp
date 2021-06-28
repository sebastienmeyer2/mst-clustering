#pragma once

#include "mst_algorithm.hpp"
#include "union_find.hpp"

#include <iostream>

class BoruvkaAlgorithm : public MSTAlgorithm
{
public:
    BoruvkaAlgorithm(Graph* graph);

    virtual void compute_mst();
};