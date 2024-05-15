#ifndef SIMULATION_H
#define SIMULATION_H



#include "spatial_hashing.h"
#include "shader.h"
#include <vector>
#include <memory>
#include <iostream>
#include <omp.h>




void initialization_simulation(std::vector<Particle*>& list_of_particles, SpatialHashing& grid,int n);
void simulating(std::vector<Particle*>& list_of_particles, SpatialHashing& grid,float dt, Shader ourShader);
void finish_simulation(std::vector<Particle*>& list_of_particles);
void simulation_gui(); 




#endif