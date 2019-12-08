#include "Raft.hpp"

Raft::Raft(Point3D pos, Vec3D dir, int in_len, double in_speed, int target)
{
    position = pos;
    direction = dir;
    this->length = in_len;
    this->speed = in_speed;
    this->target_num = target;
}