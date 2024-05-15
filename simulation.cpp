#include "simulation.h"


void initialization_simulation(std::vector<Particle*>& list_of_particles, SpatialHashing& grid , int n){
    list_of_particles.clear();
    list_of_particles.reserve(n);
    int dim = pow(n, 1/3.0f); 
    cout<<dim<<endl;
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            for(int k = 0; k < dim; k++){
                Particle* p = new Particle(glm::vec3(i*.2,j*.2,k*.2));
                list_of_particles.push_back(p);
                grid.insert(p);
            }
        }
        
    }
}

void simulating(std::vector<Particle*>& list_of_particles, SpatialHashing& grid, float dt, Shader ourShader){
    vector<vector<Particle*>> neighbours(list_of_particles.size());
    float t = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic) num_threads(8)
    for(int i = 0; i < list_of_particles.size(); i++){
        neighbours[i] = grid.get_neighbours(list_of_particles[i], PARTICLE_RADIUS);
    }


    #pragma omp parallel for num_threads(8)
    for(int i = 0; i < list_of_particles.size(); i++){
        list_of_particles[i]->calculate_density(neighbours[i]);
    }
    #pragma omp parallel for num_threads(8)
    for(int i = 0; i < list_of_particles.size(); i++){
        list_of_particles[i]->calculate_fpressure(neighbours[i]);
    }
    #pragma omp parallel for num_threads(8)
     for(int i = 0; i < list_of_particles.size(); i++){
         list_of_particles[i]->calculate_viscosity(neighbours[i]);
     }
    //#pragma omp parallel for
    for(int i = 0; i < list_of_particles.size(); i++){
        grid.erase(list_of_particles[i]);
        list_of_particles[i]->update(ourShader, dt);
        grid.insert(list_of_particles[i]);
    }
    //cout<<omp_get_wtime()-t<<endl;
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