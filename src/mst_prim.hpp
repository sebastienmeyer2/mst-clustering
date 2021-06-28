#pragma once

#include "mst_algorithm.hpp"

#include <set>
#include <cfloat>
#include <limits>
#include <stdexcept>

class PrimAlgorithm : public MSTAlgorithm
{
public:
    PrimAlgorithm(Graph* graph);

    virtual void compute_mst();
};