

#ifndef GRID_H
#define GRID_H

#include "particles.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
using namespace std;

#define HASH_SIZE 1000000
#define L 2.f

vector<shared_ptr<Particle>> grid[HASH_SIZE];

inline size_t hasha(glm::vec3 &p) {

    int ix = (unsigned int)((p.x) / L);
    int iy = (unsigned int)((p.y) / L);
    int iz = (unsigned int)((p.z) / L);
    return (unsigned int)((ix * 73856093) ^ (iy * 19349663) ^ (iz * 83492791)) % HASH_SIZE;
}

void insert(shared_ptr<Particle> p){
    size_t h = hasha(p->position);
    grid[h].push_back(p);
}

void erase(shared_ptr<Particle> p){
    size_t h = hasha(p->position);
    for(int i = 0; i < grid[h].size(); i++){
        if(grid[h][i] == p){
            grid[h].erase(grid[h].begin() + i);
            return;
        }
    }
}


vector<shared_ptr<Particle>> range_query(shared_ptr<Particle> p, float r){
    vector<shared_ptr<Particle>> neighbours;
    map<int, bool> visited;
    
    float minx = p->position.x - r;
    float miny = p->position.y - r;
    float minz = p->position.z - r;
    float maxx = p->position.x + r;
    float maxy = p->position.y + r;
    float maxz = p->position.z + r;


    double distance = 0; 

    
    for(float i = minx; i <= maxx; i += L){
        for(float j = miny; j <= maxy; j += L){
            for(float k = minz; k <= maxz; k += L){

                glm::vec3 q(i,j,k);            
                size_t h = hasha(q);                
                if(visited[h]) continue;    
                visited[h] = true;
                for(int l = 0; l < grid[h].size(); l++){
                    distance = glm::dot(grid[h][l]->position - p->position, grid[h][l]->position - p->position);
                    if(distance <= r){
                        neighbours.push_back(grid[h][l]);
                    }
                }
            }
        }
    }
    
    return neighbours;
}




#endif