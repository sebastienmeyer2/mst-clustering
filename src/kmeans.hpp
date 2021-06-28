#pragma once 

#include <iostream>
#include <cassert>
#include <cmath>	// for sqrt, fabs
#include <cfloat>	// for DBL_MAX
#include <cstdlib>	// for rand, srand
#include <ctime>	// for rand seed
#include <fstream>
#include <cstdio>	// for EOF
#include <string>
#include <algorithm>	// for count
#include <vector>

#include "point.hpp"

class Cloud 
{
private:
	int d;
	int n;
	int k;

	// maximum possible number of Points
	int nmax;

	Point *points;
	Point *centers;

public:
    Cloud(int _d, int _nmax, int _k);
    ~Cloud();

    int get_d();
    int get_n();
    int get_k();
    Point& get_point(int i);
    Point& get_center(int j);

    void add_point(Point &p, int label);
    void set_center(Point &p, int j);

    double intracluster_variance();
    int set_voronoi_labels();
    void set_centroid_centers();
    void kmeans();
    void init_bounding_box();
    void init_forgy();
    void init_plusplus();
    void init_random_partition();

    double silhouette();
};