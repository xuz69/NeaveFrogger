#ifndef FrogPlayer_hpp
#define FrogPlayer_hpp

#include "mathLib3D.hpp"
#include <stdio.h>


class FrogPlayer{
public:
    FrogPlayer(); 
    Point3D position; 
    Vec3D direction;
    int lifes; 
    bool onMiddle;
    bool onRiver;
    bool onStart;
    bool onRoad;
};

#endif