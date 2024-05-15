#ifndef SPATIAL_HASHING_H
#define SPATIAL_HASHING_H
#include <list>
#include <unordered_map>
#include <vector>
#include "particles.h"

class SpatialHashing
{
    float dim_x, dim_y, dim_z;
    std::unordered_map<int, std::vector<Particle*>> hash_table;
    int hash_function(int x, int y, int z);
public: 
    SpatialHashing();
    SpatialHashing(float _dim_x, float _dim_y, float _dim_z);
    void insert(Particle* p);
    void erase(Particle* p);
    std::vector<Particle*> get_neighbours(Particle* p, float radius);
    void clear();
};




#endif