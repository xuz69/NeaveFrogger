#ifndef Raft_hpp
#define Raft_hpp

#include "mathLib3D.hpp"
#include <stdio.h>


class Raft{
public:
    Raft(Point3D pos, Vec3D dir, int in_len, double in_speed, int target);
    Point3D position; 
    Vec3D direction;
    int length;
    double speed;
    int target_num;
};

#endif