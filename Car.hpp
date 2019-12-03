#ifndef Car_hpp
#define Car_hpp

#include "mathLib3D.hpp"
#include <stdio.h>


class Car{
public:
    Car(Point3D pos, Vec3D dir);
    Point3D position; 
    Vec3D direction;
};

#endif
