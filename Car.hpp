#ifndef Car_hpp
#define Car_hpp

#include "mathLib3D.hpp"
#include <stdio.h>


class Car{
public:
    Car(int in_x, int in_y, int in_z); 
    Point3D position; 
    Vec3D direction;
    double speed;
    bool onMiddle;
    bool onRiver;
    bool onStart;
    bool onRoad;
};

#endif