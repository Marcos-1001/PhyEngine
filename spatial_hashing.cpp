#include "spatial_hashing.h"

SpatialHashing::SpatialHashing()
{
    dim_x = 1;
    dim_y = 1;
    dim_z = 1;
}

SpatialHashing::SpatialHashing(float _dim_x, float _dim_y, float _dim_z)
{
    dim_x = _dim_x;
    dim_y = _dim_y;
    dim_z = _dim_z;
}

int SpatialHashing::hash_function(int x, int y, int z)
{    
    return x*73856093 ^ y*19349663 ^ z*83492791;
}

void SpatialHashing::insert(Particle* p)
{
    int x = p->position.x / dim_x;
    int y = p->position.y / dim_y;
    int z = p->position.z / dim_z;
    int key = hash_function(x, y, z);
    hash_table[key].push_back(p);
}

void SpatialHashing::erase(Particle* p)
{
    int x = p->position.x / dim_x;
    int y = p->position.y / dim_y;
    int z = p->position.z / dim_z;
    int key = hash_function(x, y, z);
    for (auto it = hash_table[key].begin(); it != hash_table[key].end(); it++)
    {
        if (*it == p)
        {
            hash_table[key].erase(it);
            break;
        }
    }
    
}

std::vector<Particle*> SpatialHashing::get_neighbours(Particle* p, float radius)
{
    std::vector<Particle*> neighbours;

    int minX = (p->position.x- radius) / dim_x;
    int minY = (p->position.y- radius) / dim_y;
    int minZ = (p->position.z- radius) / dim_z;
    
    int maxX = (p->position.x + radius) / dim_x;
    int maxY = (p->position.y + radius) / dim_y;
    int maxZ = (p->position.z + radius) / dim_z;



    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            for (int z = minZ; z <= maxZ; z++)
            {
                int key = hash_function(x, y, z);
                if (hash_table.find(key) != hash_table.end())
                {
                    for (auto& particle : hash_table[key])
                    {
                        if (glm::distance(p->position, particle->position) <= radius)
                        {
                            neighbours.push_back(particle);
                        }
                    }
                }
            }
        }
    }



    return neighbours;
}