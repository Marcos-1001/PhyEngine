#ifndef SIMULATION_H
#define SIMULATION_H



#include "particles.h"
#include "grid.h"
#include "shader.h"
#include <vector>
#include <memory>
#include <iostream>
#include <omp.h>


std::vector<Particle> list_of_particles;

float vstrength = .08f;


void initialization_simulation(int n);
void simulating( float dt, Shader ourShader);
void simulation_gui(); 




#endif