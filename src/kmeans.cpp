#include "kmeans.hpp"

Cloud::Cloud(int _d, int _nmax, int _k)	
{
    /*!
     * @brief Builds an empty cloud
     * 
     * @param _d The dimension of the points
	 * @param _nmax The maximal number of points in this cloud
	 * @param _k The number of clusters to be drawn
     */

	d = _d;
	Point::d = _d;
	n = 0;
	k = _k;

	nmax = _nmax;

	points = new Point[nmax];
	centers = new Point[k];
}

Cloud::~Cloud()
{
    /*!
     * @brief Destructor of the cloud class
     */

	delete[] centers;
	delete[] points;
}

void Cloud::add_point(Point &p, int label)
{
    /*!
     * @brief Adds a point in the cloud by copying its values
     * 
     * @param p The point to be added
	 * @param label A label to give to this point
     */

	assert(("Too much points!", n < nmax));
	assert(("No center to assign to this point!", label<k));

	for(int m = 0; m < d; m++)
	{
		points[n].coords[m] = p.coords[m];
	}

	points[n].label = label;

	n++;
}

int Cloud::get_d()
{
    /*!
     * @brief Getter of the point dimension
     * 
     * @return The dimension of the points contained in this cloud
     */

	return d;
}

int Cloud::get_n()
{
    /*!
     * @brief Getter of the number of points
     * 
     * @return The number of points in this cloud
     */

	return n;
}

int Cloud::get_k()
{
    /*!
     * @brief Getter of the number of clusters
     * 
     * @return The number of clusters in this cloud
     */

	return k;
}

Point& Cloud::get_point(int i)
{
    /*!
     * @brief Getter of points
     * 
     * @param i Index of the point in this cloud
     * 
     * @return The point at index \p i
     */

	return points[i];
}

Point& Cloud::get_center(int j)
{
    /*!
     * @brief Getter of centers
     * 
     * @param j Index of the center in this cloud
     * 
     * @return The center point at index \p j
     */

	return centers[j];
}

void Cloud::set_center(Point &p, int j)
{
    /*!
     * @brief Enables to change centers from this cloud
     * 
     * @param p A point to change the center by
	 * @param j Index of the center to be modified
     */

	for(int m = 0; m < d; m++)
		centers[j].coords[m] = p.coords[m];
}

double Cloud::intracluster_variance()
{
    /*!
     * @brief Computes the intracluster variance in this cloud
     * 
     * @return The intracluster variance
     */

	double intraclusterVar = 0.0;
	for(int i = 0; i < this->n; i ++){
		intraclusterVar += points[i].distance(&(centers[points[i].label]));
	}
	if (n > 0) {
		intraclusterVar /= n;
	}

	return intraclusterVar;
}

int Cloud::set_voronoi_labels()
{
    /*!
     * @brief E-step in the kmeans algorithm
     */

	int changedLabels = 0;

	for (int i = 0; i < this->n; i ++){
		int pointLabel = points[i].label;
		int newLabel = points[i].label;
		double minDist = points[i].distance(&(centers[pointLabel]));

		for (int j = 0; j < this->k; j ++){
			double dist = points[i].distance(&(centers[j]));
			if (dist < minDist || (dist == minDist && j < newLabel)) {
				minDist = dist;
				newLabel = j;
			}
		}

		if (newLabel != pointLabel){
			points[i].label = newLabel;
			changedLabels += 1;
		}
	}
	
	return changedLabels;
}

void Cloud::set_centroid_centers()
{
    /*!
     * @brief M-step in the kmeans algorithm
     */

	int* clusterSizes = new int [this->k] ();

	// we start with "new" points
	Point *newCenters = new Point [this->k] ();

	// we count the number of data points in each cluster 
	// and accumulate their sum
	for (int i = 0; i < this->n; i ++){

		int pointLabel = points[i].label;
		clusterSizes[pointLabel] += 1;

		for (int j = 0; j < this->d; j ++){
			newCenters[pointLabel].coords[j] += points[i].coords[j];
		}
	}

	// we compute the average per cluster
	for (int i = 0; i < this->k; i ++){
		if (clusterSizes[i] > 0){
			for (int j = 0; j < this->d; j ++){
				newCenters[i].coords[j] /= clusterSizes[i];
			}
			
			this->set_center(newCenters[i], i);
		}
	}

	delete[] clusterSizes;
	clusterSizes = NULL;
	delete[] newCenters;
	newCenters = NULL;
}

void Cloud::kmeans()
{
	/*!
     * @brief Computes the kmeans algorithm on this cloud. It is possible 
	 * to select different initializations for the centers, within bounding 
	 * box, forgy, plusplus and random partition. 
     */

	std::cout << "\nComputing clustering using kmeans...";

	// init_bounding_box();
	init_forgy(); 
	// init_plusplus(); 
	// init_random_partition();

	// running Lloyd's heuristics
	int changesMade = 0;
	do {
		changesMade = set_voronoi_labels();
		set_centroid_centers();
	} while (changesMade > 0);
	
	std::cout << "[OK]" << std::endl;
}

void Cloud::init_bounding_box()
{
    /*!
     * @brief Initializes the centers of this cloud by using a bounding box over 
	 * all the points
     */

	double* minBox = new double [this->d] ();
	double* maxBox = new double [this->d] ();
	for (int i = 0; i < this->d; i ++){
		minBox[i] = DBL_MAX;
		maxBox[i] = DBL_MIN;
	}

	for (int i = 0; i < this->n; i ++){
		for (int j = 0; j < this->d; j ++){
			if (points[i].coords[j] < minBox[j]){
				minBox[j] = points[i].coords[j];
			}
			if (points[i].coords[j] > maxBox[j]){
				maxBox[j] = points[i].coords[j];
			}
		}
	}

	Point center;
	for (int i = 0; i < this->k; i ++){
		center.label = i;

		for (int j = 0; j < this->d; j ++){
			double randomValue = (double)rand() / RAND_MAX;
			randomValue = minBox[j] + (randomValue * (maxBox[j] - minBox[j]));
			
			center.coords[j] = randomValue;
		}

		this->set_center(center, i);
	}

	delete[] minBox;
	minBox = NULL;
	delete[] maxBox;
	maxBox = NULL;
}

void Cloud::init_forgy()
{
    /*!
     * @brief Initializes the centers of this cloud using Forgy method,
	 * that is it samples the centers uniformly from the data points
     */

	// we will pick points from our data 
	int* availablePoints = new int [this->n] ();
	for (int i = 0; i < this->n; i ++){
		availablePoints[i] = 1;
	}

	// we must know what we've chosen so far
	int* chosenPoints = new int [this->k] ();
	int leftCenters = this->k;

	while (leftCenters > 0){
		int randomPoint = rand() % this->n;

		if (availablePoints[randomPoint] > 0){
			availablePoints[randomPoint] = 0;
			chosenPoints[this->k - leftCenters] = randomPoint;
			leftCenters -= 1;
		}
	}

	// we just have to build our centers now!
	Point c;
	for (int i = 0; i < this->k; i ++){
		c.label = i;

		for (int j = 0; j < this->d; j ++){
			c.coords[j] = points[chosenPoints[i]].coords[j];
		}

		this->set_center(c, i);
	}

	delete[] availablePoints;
	availablePoints = NULL;
	delete[] chosenPoints;
	chosenPoints = NULL;
}

void Cloud::init_plusplus()
{
    /*!
     * @brief Initializes the centers of this cloud using kmeans++
	 * which consists in drawing a distribution of points
     */

	// step of initialization for kmeans++
	int* chosenPoints = new int [this->k] ();
	int randomPoint = rand() % this->n;
	chosenPoints[0] = randomPoint;

	// loop 
	double* distances = new double [this->n] ();
	for (int i = 1; i < this->k; i ++){

		// we compute the nearest center for each data point
		double totalDist = 0.0;

		for (int j = 0; j < this->n; j ++){
			distances[j] = DBL_MAX;

			for (int l = 0; l < i; l ++){
				double dist = points[j].distance(&(points[chosenPoints[l]]));

				if (dist < distances[j]){
					distances[j] = dist;
				}

				totalDist += distances[j];
			}
		}

		for (int j = 0; j < this->n; j ++){
			distances[j] /= totalDist; 
			if (j > 0){
				distances[j] += distances[j-1]; // we create a cumulative sum
			}
		}

		// we find the next center from a uniform distribution
		double randomUnif = (double)rand() / RAND_MAX;

		for (int j = 0; j < this->n; j ++){
			if ( (j == 0 && distances[j] > randomUnif && randomUnif >= 0) \
					|| (j > 0 && distances[j] > randomUnif && randomUnif >= distances[j-1]) ){
				chosenPoints[i] = j;
			}
		}
	}

	// we just have to build our centers now!
	Point c;
	for (int i = 0; i < this->k; i ++){
		c.label = i;

		for (int j = 0; j < this->d; j ++){
			c.coords[j] = points[chosenPoints[i]].coords[j];
		}

		this->set_center(c, i);
	}		

	delete[] chosenPoints;
	chosenPoints = NULL;
	delete[] distances;
	distances = NULL;
}

void Cloud::init_random_partition()
{
    /*!
     * @brief Initializes the centers of this cloud using random partitions,
	 * ie builds centers as the centers of random partitions of the data points
     */

	// random labelization of data points
	int randomCenter = 0;
	for (int i = 0; i < this->n; i ++){
		randomCenter = rand() % this->k;
		points[i].label = randomCenter;
	}

	// centers will be centroids of this partition
	this->set_centroid_centers();
}

double Cloud::silhouette()
{
    /*!
     * @brief Computes the silhouette of this cloud 
     * 
     * @return The silhouette of this cloud
     */

	double silhouette = 0.0;

	// we compute a and b which are respectively the 
	// average distance to points of the same cluster 
	// and min of average distances to foreign clusters
	double* a = new double [n] ();
	double* b = new double [n] ();
	double* s = new double [n] ();

	for (int i = 0; i < this->n; i ++){
		double* distPointClusters = new double [k] ();
		int* clustersSize = new int [k] ();
		
		for (int j = 0; j < this->n; j ++){
			if (j != i){
				int pointLabel = points[j].label;
				clustersSize[pointLabel] += 1;

				distPointClusters[pointLabel] += std::sqrt(points[i].distance(&(points[j])));
			}
		}

		double minDist = DBL_MAX;
		for (int j = 0; j < this->k; j ++){
			if (clustersSize[j] > 0){
				distPointClusters[j] /= clustersSize[j];

				if (j != points[i].label && distPointClusters[j] < minDist) {
					minDist = distPointClusters[j];
				}
			}
		}

		a[i] = distPointClusters[points[i].label];
		b[i] = 0.0;
		if (minDist < DBL_MAX){
			b[i] = minDist;
		}

		delete[] distPointClusters;
		distPointClusters = NULL;
		delete[] clustersSize;
		clustersSize = NULL;

		s[i] = (b[i] - a[i]) / (std::max(a[i], b[i]));
	}

	for (int i = 0; i < this->n; i ++){
		silhouette += s[i];
	}
	silhouette /= this->n;

	return silhouette;
}
