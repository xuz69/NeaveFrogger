#include "FrogPlayer.hpp"

FrogPlayer::FrogPlayer(){
    // set the origin position
    this->position = Point3D(0,0,105);
    
    // set the origin direction
    this->direction = Vec3D(0,0,-1);

    // the count of frog‘s life
    this->lifes = 3;

    //check the frog is on which part of the ground, river, middle safe place, start place or water.
    this->onRiver = false;

    this->onMiddle = false;

    this->onStart = true;

    this->onRoad = false;
    
    //which line of raft the frog is on
    this->num_tile_raft = 0;

    //check the frog whether on the raft
    this->on_raft_num = 0;

    //whether the frog is live
    this->live = true;

    //whether the frog is win
    this->win = false;

    //the score of the round
    this->score = 0;
}
