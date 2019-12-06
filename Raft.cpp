#include "Raft.hpp"

Raft::Raft(Point3D pos, Vec3D dir, int in_len, double in_speed)
{
    position = pos;
    direction = dir;
    this->length = in_len;
    this->speed = in_speed;
}