#pragma once 

#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>

class Point
{
    /*!
     * @class Describes a Point with static dimension
     * 
     */

    public:

        static int d;
        double *coords;
        int label; // for k-means

        Point();
        ~Point();

        int get_dim();
        void print();
        double distance(Point* other);

        static std::vector<Point*> read_points_from_file(std::string file_path, int d);

};