#include "compare_clustering.hpp"

int nb_columns(const std::string &line)
{
    /*!
     * @brief Enables to count the number of columns from a header
     * 
     * @param line The header line of the considered file
     * 
     * @return The number of columns 
     */

	return std::count_if(line.begin(), line.end(),
                                [](unsigned char c){ return std::isspace(c); }) + 2;
}

void compare_clustering(std::string file_to_read, int k)
{
    /*!
     * @brief From a file given as entry and an integer k for the number of 
     * clusters to draw, establishes a comparison between kmeans and MST clustering
     * 
     * @param file_to_read Path to the data
     * @param k Number of clusters to draw
     * 
     */

	// get points from datafile

	std::ifstream is(file_to_read);
	assert(is.is_open());

	std::string header_line;
	std::getline(is, header_line);

	const int d = nb_columns(header_line) - 1;
    Point::d = d;

    std::vector<Point*> points = Point::read_points_from_file(file_to_read, d);
    int n = points.size();

    // run MST clustering 

    std::chrono::steady_clock::time_point graph_creation = std::chrono::steady_clock::now();

    std::cout << "\nCreating graph...";
    Graph* g = new Graph(points);
    std::cout << "[OK]" << std::endl;

    std::chrono::steady_clock::time_point kruskal_algo = std::chrono::steady_clock::now();

    KruskalAlgorithm kruskal = KruskalAlgorithm(g);
    kruskal.compute_mst();

    std::chrono::steady_clock::time_point kruskal_clustering = std::chrono::steady_clock::now();

    std::unordered_map<Node*, Node*> clusters = kruskal.compute_clustering(k);

    std::chrono::steady_clock::time_point mst_end = std::chrono::steady_clock::now();

    // run kmeans 

    std::chrono::steady_clock::time_point cloud_creation = std::chrono::steady_clock::now();

    std::cout << "\nCreating cloud...";

    Cloud cloud = Cloud(d, n, k);

    int label = 0;
    for (int i = 0; i < n; i ++) {
        cloud.add_point(*points[i], label);
        label = (label+1) % k;
    }

    std::cout << "[OK]" << std::endl;

    std::chrono::steady_clock::time_point kmeans_algo = std::chrono::steady_clock::now();

    cloud.kmeans();

    std::chrono::steady_clock::time_point kmeans_end = std::chrono::steady_clock::now();

    // comparison

    std::cout << "\nTotal time spent by MST clustering including graph creation: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(mst_end - graph_creation).count() << " µs" << std::endl;

    std::cout << "Total time spent by MST clustering: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(mst_end - kruskal_algo).count() << " µs" << std::endl;

    double mst_variance = intracluster_variance(clusters);
    std::cout << "Intracluster variance in MST: ";
    std::cout << mst_variance << std::endl;

    double mst_silhouette = silhouette(clusters);
    std::cout << "Silhouette in MST: ";
    std::cout << mst_silhouette << std::endl;

    std::cout << "\nTotal time spent by kmeans including cloud creation: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(kmeans_end - cloud_creation).count() << " µs" << std::endl;

    std::cout << "Total time spent by kmeans: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(kmeans_end - kmeans_algo).count() << " µs" << std::endl;

    double kmeans_variance = cloud.intracluster_variance();
    std::cout << "Intracluster variance in kmeans: ";
    std::cout << kmeans_variance << std::endl;

    double kmeans_silhouette = cloud.silhouette();
    std::cout << "Silhouette in kmeans: ";
    std::cout << kmeans_silhouette << std::endl;

    return;
}

void compare_inconsistency(std::string file_to_read, double cutoff)
{
    /*!
     * @brief From a file given as entry and an integer k for the number of 
     * clusters to draw, establishes a comparison between kmeans and MST clustering
     * 
     * @param file_to_read Path to the data
     * @param k Number of clusters to draw
     * 
     */

	// get points from datafile

	std::ifstream is(file_to_read);
	assert(is.is_open());

	std::string header_line;
	std::getline(is, header_line);

	const int d = nb_columns(header_line) - 1;
    Point::d = d;

    std::vector<Point*> points = Point::read_points_from_file(file_to_read, d);
    int n = points.size();

    // run Inconsistency method 

    std::chrono::steady_clock::time_point graph_creation = std::chrono::steady_clock::now();

    std::cout << "\nCreating graph...";
    Graph* g = new Graph(points);
    std::cout << "[OK]" << std::endl;

    std::chrono::steady_clock::time_point kruskal_algo = std::chrono::steady_clock::now();

    KruskalAlgorithm kruskal = KruskalAlgorithm(g);
    kruskal.compute_mst();

    std::chrono::steady_clock::time_point inc_clustering = std::chrono::steady_clock::now();

    std::unordered_map<Node*, Node*> clusters = kruskal.compute_clustering(cutoff);

    std::unordered_map<Node*, int> clusters_size;
    for (auto kv : clusters) {
        if(clusters_size.find(kv.second) != clusters_size.end()) {
            clusters_size.at(kv.second) += 1;
        } else {
            clusters_size.insert(std::pair<Node*, int>(kv.second, 1));
        }
    }
    std::cout << "In total, there are " << clusters_size.size() << " clusters when cutoff = " << cutoff << std::endl;

    int big_clusters = 0;
    for (auto kv : clusters_size) {
        if (kv.second > 1) {
            big_clusters += 1;
        }
    }
    std::cout << "There are " << big_clusters << " clusters of size at least 2" << std::endl;

    std::chrono::steady_clock::time_point inc_end = std::chrono::steady_clock::now();

    // run kmeans 

    std::chrono::steady_clock::time_point cloud_creation = std::chrono::steady_clock::now();

    std::cout << "\nCreating cloud...";

    Cloud cloud = Cloud(d, n, clusters.size());

    int label = 0;
    for (int i = 0; i < n; i ++) {
        cloud.add_point(*points[i], label);
        label = (label+1) % clusters.size();
    }

    std::cout << "[OK]" << std::endl;

    std::chrono::steady_clock::time_point kmeans_algo = std::chrono::steady_clock::now();

    cloud.kmeans();

    std::chrono::steady_clock::time_point kmeans_end = std::chrono::steady_clock::now();

    // comparison

    std::cout << "\nTotal time spent by Inconsistency clustering including graph creation: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(inc_end - graph_creation).count() << " µs" << std::endl;

    std::cout << "Total time spent by Inconsistency clustering: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(inc_end - kruskal_algo).count() << " µs" << std::endl;

    double inc_variance = intracluster_variance(clusters);
    std::cout << "Intracluster variance with Inconsistency clustering: ";
    std::cout << inc_variance << std::endl;

    std::cout << "\nTotal time spent by kmeans including cloud creation: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(kmeans_end - cloud_creation).count() << " µs" << std::endl;

    std::cout << "Total time spent by kmeans: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(kmeans_end - kmeans_algo).count() << " µs" << std::endl;

    double kmeans_variance = cloud.intracluster_variance();
    std::cout << "Intracluster variance in kmeans: ";
    std::cout << kmeans_variance << std::endl;

    return;
}