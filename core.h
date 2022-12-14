#ifndef CORE_H
#define CORE_H

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::sqrt;
using std::cout;
using std::endl;
using std::cerr;
using std::flush;
using std::string;
using std::to_string;
using std::numeric_limits;
using std::ostream;
using std::swap;
using std::sort;

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}
inline double clamp(double x, double min, double max)
{
    if (x < min)
    {
        return min;
    }
    if (x > max)
    {
        return max;
    }
    return x;
}
inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max+1));
}
#endif