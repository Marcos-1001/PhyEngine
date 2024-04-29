#ifndef SIMULATION_H
#define SIMULATION_H



#include "particles.h"
#include "grid.h"
#include <vector>
#include <memory>
#include <iostream>
#include <omp.h>


vector<shared_ptr<Particle>> list_of_particles;

float vstrength = .08f;


void initialization_simulation(int n);
void simulating( float dt, Shader ourShader);
void simulation_gui(); 




#endif