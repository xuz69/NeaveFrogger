#ifndef Heightmap_hpp
#define Heightmap_hpp

#include <stdio.h>
#include <vector>
#include "mathLib3D.hpp"


class Heightmap{
public:
    Heightmap(std::vector< std::vector<Point3D> > & two_d_array_points, int w, int l);

};

#endif
