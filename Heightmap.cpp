#include "Heightmap.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <math.h>

/*
 Height Map Function
 
 Choosing the size of the terrain and the algorithm of terrain that you want to draw
 Using a 2D array to store all points, and another 2D array to store color
 */


Heightmap::Heightmap(std::vector< std::vector<Point3D> > & two_d_array_points, int w, int l){
    
    // Point3D 2D array
    std::vector<Point3D> points;
    /* initialized points with height 0 */
    for(int x = 0; x < w; x ++){
        points.clear();
        for(int z = 0; z < l; z++){
            points.push_back(Point3D(x-w/2,0,z-l/2-5));
        }
        two_d_array_points.push_back(points);
    }
  
}

