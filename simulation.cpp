#include "simulation.h"


void initialization_simulation(std::vector<Particle*>& list_of_particles,int n){
    list_of_particles.clear();
    list_of_particles.reserve(n);
    float dim = pow(n, 1/3.0f); 

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            for(int k = 0; k < dim; k++){
                Particle* p = new Particle(glm::vec3(i+.5,j+.5,k+.5));
                list_of_particles.push_back(p);
                //insert(p);
            }
        }
        
    }
}

void simulating(std::vector<Particle*>& list_of_particles, float dt, Shader ourShader){
    //#pragma omp parallel for
    for(int i = 0; i < list_of_particles.size(); i++){
        list_of_particles[i]->calculate_density(list_of_particles);
    }
    //#pragma omp parallel for
    for(int i = 0; i < list_of_particles.size(); i++){
        list_of_particles[i]->calculate_fpressure(list_of_particles);
    }
    // #pragma omp parallel for
    // for(int i = 0; i < list_of_particles.size(); i++){
    //     list_of_particles[i]->calculate_viscosity(list_of_particles);
    // }
    //#pragma omp parallel for
    for(int i = 0; i < list_of_particles.size(); i++){
        list_of_particles[i]->update(ourShader, dt);
    }
    // #pragma omp parallel for
    // for(int i = 0; i < list_of_particles.size(); i++){
    //     erase(list_of_particles[i]);
    //     list_of_particles[i]->update(0.0000001);
    //     insert(list_of_particles[i]);
    // }
    
}

void finish_simulation(std::vector<Particle*>& list_of_particles){
    for(auto p : list_of_particles){
        delete p;
    }
}