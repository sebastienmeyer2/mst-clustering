#include "mst_algorithm.hpp"

#include <set>
#include <cfloat>
#include <limits>
#include <stdexcept>

class PrimKumarAlgorithm : public MSTAlgorithm
{
    private:
        int world_size;
        int rank;

        void compute_mst_slave();
        void compute_mst_master();

    public:
        PrimKumarAlgorithm(Graph* graph, int rank_, int world_size_);

        virtual void compute_mst();
};