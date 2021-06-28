#include "point.hpp"

int Point::d;

Point::Point() 
{
    /*!
     * @brief Base constructor of a Point
     * 
     */

    this->coords = new double [d] ();
    this->label = 0;
}

Point::~Point()
{
    /*!
     * @brief Base destructor of a Point
     * 
     */

    delete[] this->coords;
}

int Point::get_dim()
{
    /*!
     * @brief Gives the shared dimension between all the points
     * 
     * @return The dimension \p d of all the points
     * 
     */

    return this->d;
}

void Point::print()
{
    /*!
     * @brief Enables to print d-dimensional points on the console
     * 
     */

    for (int i = 0; i < d; i ++) {
        std::cout << coords[i] << std::endl;
    }
    std::cout << "\n" << std::endl;
    return;
}

double Point::distance(Point* other)
{
    /*!
     * @brief Evaluates the distance between two instances of the class Point
     * 
     * @param other Another Point to evaluate the distance with 
     * 
     * @return The distance between this point and the \p other 
     */

    double dist = 0.0;
    for (int i = 0; i < d; i ++) {
        dist += std::pow(coords[i] - other->coords[i], 2);
    }
    dist = std::sqrt(dist);

    return dist;
}

std::vector<Point*> Point::read_points_from_file(std::string file_path, int d) 
{
    /*!
     * @brief Builds a vector of points from a file where data is correctly
     * formatted (double spaced with " ")
     * 
     * @param file_path the path to the file to get data from
     * @param d the dimension of the points
     * 
     * @return A vector of pointers to the created points
     */

    std::vector<Point*> points;

    std::ifstream is(file_path);
    assert(("No such file", is.is_open()));

    std::string line;

	// while not at the end of the file
	while(is.peek() != EOF)
	{
		// read new points
        Point* p = new Point();

		for(int m = 0; m < d; m++)
		{
			is >> p->coords[m];
		}

        points.push_back(p);

		// new line
		is.get();
	}
    points.pop_back(); // last line makes an empty point otherwise

    return points;
}