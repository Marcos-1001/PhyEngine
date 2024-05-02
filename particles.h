
#ifndef PARTICLES_H
#define PARTICLES_H

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Objeto.h"


#define STIFFNESS_CONSTANT 0.1f
#define REST_DENSITY 1000.0f
#define PARTICLE_RADIUS 0.1f

struct Particle {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);    
    glm::vec3 acceleration = glm::vec3(0.0f, -9.81f, 0.0f);    
    Esfera esfera; 
    float mass = 1.0f;  
    float density = {};
    glm::vec3 force = {};

    
    Particle() = default;
    Particle(glm::vec3 pos) : position(pos) {
        esfera.centro = pos;
        esfera.radius = 1.f;
        esfera.slices = 10;
        esfera.stacks = 10;
        esfera.setup();
    }
    Particle(glm::vec3 pos, glm::vec3 vel) : position(pos), velocity(vel) {
        esfera.centro = pos;
        esfera.radius = 1.f;
        esfera.slices = 10;
        esfera.stacks = 10;
        esfera.setup();
    }

    void update(Shader& sh, const float& dt); 
    
    void calculate_density(std::vector<Particle*>& Particle);    
    void calculate_fpressure(std::vector<Particle*>& Particle);  
    void calculate_viscosity(std::vector<Particle*>& Particle);  
    

};
float inline calculate_pressure(Particle& p, float k, float rest_density);
float inline density_kernel( float r);
float inline pressure_kernel( float r);
float inline viscosity_kernel( float r);




#endif // !PARTICLES_H