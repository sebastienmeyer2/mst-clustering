"""
The purpose of this file is to plot results in fancy charts.

Note:
    - Results from MST algorithms are the mean results after 5 iterations 
    with the same data 
    - Results from MST clustering & k-means are the mean results after 5 
    iterations with the same data (k=5 and different size & n=1000 and 
    different number of clusters)
"""

import numpy as np
import matplotlib.pyplot as plt 
import matplotlib as mpl
font = {"size": 18} 
mpl.rc("font", **font)

######################################################################
###       COMPARISON OF MST ALGORITHMS ON GENERATED GRAPHS         ###
######################################################################

MST_ALGORITHMS = {"P": "Prim", "B": "Boruvka", "K": "Kruskal"}

ERDOS_RENYI_RESULTS = {
                      100: {"P": 5.428, "B": 14.104, "K": 2.707, "PP": 1.237},
                      250: {"P": 7.108, "B": 32.221, "K": 6.360, "PP": 5.033},
                      500: {"P": 23.607, "B": 120.978, "K": 17.007, "PP": 16.427},
                      1000:  {"P": 98.500, "B": 480.175, "K": 65.417, "PP": 64.981}
                      }

ERDOS_RENYI_SIZE = {
                   100: 540,
                   250: 3089,
                   500: 12498,
                   1000: 50210
                   }

BARABASI_ALBERT_RESULTS = {
                          100: {"P": 6.128, "B": 14.120, "K": 5.249, "PP": 1.067},
                          250: {"P": 3.107, "B": 8.099, "K": 3.645, "PP": 1.636},
                          500: {"P": 5.267, "B": 16.708, "K": 6.148, "PP": 4.677},
                          1000:  {"P": 9.082, "B": 30.212, "K": 10.780, "PP": 8.436}
                          }

BARABASI_ALBERT_SIZE = {
                       100: 614,
                       250: 465,
                       500: 966,
                       1000: 1966
                       }

def plot_mst_results():
    """Plot the results of MST algorithms. 
    
    Plot the mean time spent on different graphs generated using 
    Erdos-Rényi and Barabasi-Albert methods
    """

    # Plot for Erdos-Rényi results
    nb_nodes = []
    nb_edges = []
    compl = []

    prim_res = []
    boruvka_res = []
    kruskal_res = []
    prim_para_res = []

    for n, algos in ERDOS_RENYI_RESULTS.items():

        prim_res.append(algos["P"])
        boruvka_res.append(algos["B"])
        kruskal_res.append(algos["K"])
        prim_para_res.append(algos["PP"])

        nb_nodes.append(n)
        nb_edges.append(ERDOS_RENYI_SIZE[n])
        compl.append(nb_edges[-1]*np.log(nb_nodes[-1]))

    plt.figure(1)
    plt.plot(compl, prim_res, label="Prim")
    plt.plot(compl, boruvka_res, label="Boruvka")
    plt.plot(compl, kruskal_res, label="Kruskal")
    plt.xlabel(r"$m \times \log(n)$")
    plt.ylabel("Time spent in $ms$")
    plt.title("Comparison: Erdos-Rényi graphs")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/ER_MST.png")
    plt.show()

    plt.figure(2) 
    plt.plot(compl, prim_res, label="Prim")
    plt.plot(compl, prim_para_res, label="Parallelized Prim")
    plt.plot(compl, kruskal_res, label="Kruskal")
    plt.xlabel(r"$m \times \log(n)$")
    plt.ylabel("Time spent in $ms$")
    plt.title("Comparison: Erdos-Rényi graphs")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/ER_MST_para.png")
    plt.show()

    # Plot for Barabasi-Albert results
    nb_nodes = []
    nb_edges = []
    compl = []

    prim_res = []
    boruvka_res = []
    kruskal_res = []
    prim_para_res = []

    for n, algos in BARABASI_ALBERT_RESULTS.items():

        prim_res.append(algos["P"])
        boruvka_res.append(algos["B"])
        kruskal_res.append(algos["K"])
        prim_para_res.append(algos["PP"])

        nb_nodes.append(n)
        nb_edges.append(BARABASI_ALBERT_SIZE[n])
        compl.append(nb_edges[-1]*np.log(nb_nodes[-1]))

    plt.figure(3)
    plt.plot(compl, prim_res, label="Prim")
    plt.plot(compl, boruvka_res, label="Boruvka")
    plt.plot(compl, kruskal_res, label="Kruskal")
    plt.xlabel(r"$m \times \log(n)$")
    plt.ylabel("Time spent in $ms$")
    plt.title("Comparison: Barabasi-Albert graphs")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/BA_MST.png")
    plt.show()

    plt.figure(4)
    plt.plot(compl, prim_res, label="Prim")
    plt.plot(compl, prim_para_res, label="Parallelized Prim")
    plt.plot(compl, kruskal_res, label="Kruskal")
    plt.xlabel(r"$m \times \log(n)$")
    plt.ylabel("Time spent in $ms$")
    plt.title("Comparison: Barabasi-Albert graphs")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/BA_MST_para.png")
    plt.show()

######################################################################
###            COMPARISON OF MST CLUSTERING AND K-MEANS            ###
######################################################################

CLUSTERING_ALGORITHMS = {"K": "Kruskal MST Clustering", "KM": "K-means Clustering",
                         "I": "Edge inconsistency"}

# With k=5, comparing time spent
WALMART_N_RESULTS = {
                    100: {"K": 26.035, "KM": 1.820},
                    250: {"K": 145.940, "KM": 4.271},
                    500: {"K": 683.692, "KM": 7.229},
                    1000: {"K": 3296.818, "KM": 25.571}
                    }

# With n=1000, comparing intracluster variance
WALMART_K_RESULTS = {
                    3: {"K": 2.40, "KM": 2.09},
                    4: {"K": 2.39, "KM": 2.04},
                    5: {"K": 2.33, "KM": 1.85},
                    6: {"K": 2.32, "KM": 1.81},
                    7: {"K": 2.30, "KM": 1.77},
                    8: {"K": 2.18, "KM": 1.67},
                    10: {"K": 2.16, "KM": 1.60}
                    }

ERDOS_RENYI_I = {
                5: {"all": 249, "big": 23},
                10: {"all": 193, "big": 13},
                50: {"all": 149, "big": 4},
                100: {"all": 145, "big": 3},
                500: {"all": 142, "big": 1},
                1000: {"all": 142, "big": 1}
                }

WALMART_I_RESULTS = {
                    5: {"all": 244, "big": 53},
                    10: {"all": 189, "big": 30},
                    50: {"all": 148, "big": 6},
                    100: {"all": 137, "big": 4},
                    500: {"all": 138, "big": 3},
                    1000: {"all": 134, "big": 3}
                    }

def plot_clustering_results():
    """Plot clustering results on Walmart dataset."""

    # Plot for time complexity
    nb_nodes = []

    mst_res = []
    kmeans_res = []

    for n, algos in WALMART_N_RESULTS.items():

        mst_res.append(np.log10(algos["K"]))
        kmeans_res.append(np.log10(algos["KM"]))

        nb_nodes.append(n)

    plt.figure(1)
    plt.plot(nb_nodes, mst_res, label="MST Clustering")
    plt.plot(nb_nodes, kmeans_res, label="K-means")
    plt.xlabel("Number of points")
    plt.ylabel(r"$\log($Time spent$)$")
    plt.title("Comparison: Time spent $(k = 5)$")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/clustering_time.png")
    plt.show()

    # Plot for intracluster variance
    nb_clusters = []

    mst_res = []
    kmeans_res = []

    for k, algos in WALMART_K_RESULTS.items():

        mst_res.append(algos["K"])
        kmeans_res.append(algos["KM"])

        nb_clusters.append(k)

    plt.figure(2)
    plt.plot(nb_clusters, mst_res, label="MST Clustering")
    plt.plot(nb_clusters, kmeans_res, label="K-means")
    plt.xlabel("Number of clusters")
    plt.ylabel("Intracluster variance")
    plt.title("Comparison: Performance $(n = 1000)$")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/clustering_perf.png")
    plt.show()

    # Plot for MST clustering (inconsistency method)
    cutoffs = []

    er_res = []
    walmart_res = []

    for c, er_clusters in ERDOS_RENYI_I.items():

        cutoffs.append(np.log10(c))

        er_res.append(er_clusters["big"])
        walmart_res.append(WALMART_I_RESULTS[c]["big"])

    plt.figure(3)
    plt.plot(cutoffs, er_res, label="E-R graph")
    plt.plot(cutoffs, walmart_res, label="Walmart data")
    plt.xlabel(r"$\log($Cutoff$)$")
    plt.ylabel(r"Clusters of size $\geq 2$")
    plt.title("Comparison: Inconsistency method")
    plt.legend()
    plt.tight_layout()
    plt.savefig("report/figures/clustering_inc.png")
    plt.show()

if __name__ == "__main__":

    plot_mst_results()

    plot_clustering_results()
