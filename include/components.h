#include <iostream>
#include <map>

#include "vect.h"

using EntityID = uint64_t;


struct Position 
{
    Vect<int64_t> pos;
}

struct Velocities 
{
    Vect<int64_t> vel;
}

struct Registry 
{
    std::map<EntityID, Position> positions;
    std::map<EntityID, Velocity> velocities;
}