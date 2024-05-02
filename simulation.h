#ifndef SIMULATION_H
#define SIMULATION_H



#include "particles.h"
#include "grid.h"
#include "shader.h"
#include <vector>
#include <memory>
#include <iostream>
#include <omp.h>




void initialization_simulation(std::vector<Particle*>& list_of_particles,int n);
void simulating(std::vector<Particle*>& list_of_particles, float dt, Shader ourShader);
void finish_simulation(std::vector<Particle*>& list_of_particles);
void simulation_gui(); 




#endif