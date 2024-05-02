#include "particles.h"


float inline density_kernel( float r) {
    const float h_f =  r/PARTICLE_RADIUS;
    if (h_f >= 0 && h_f <= 1) {
        return 4.0f / (M_PI * pow(PARTICLE_RADIUS, 2)) * (2.0f / 3.0f - pow(h_f, 2) + 0.5f * pow(h_f, 3));
    }
    else if (h_f > 1 && h_f <= 2) {
        return 4.0f / (M_PI * pow(PARTICLE_RADIUS, 2)) * (1.0f / 6.0f * pow(2 - h_f, 3));
    }
    else {
        return 0;
    }
}
float inline pressure_kernel( float r) {
    const float h_f =  r/PARTICLE_RADIUS;
    if (h_f >= 0 && h_f <= 1) {
        return 4.0f / (M_PI * pow(PARTICLE_RADIUS, 2)) * (-2.0f * h_f + 1.5f * pow(h_f, 2));
    }
    else if (h_f > 1 && h_f <= 2) {
        return 4.0f / (M_PI * pow(PARTICLE_RADIUS, 2)) * (-0.5f * pow(2 - h_f, 2));
    }
    else {
        return 0;
    }
}
float inline viscosity_kernel(float r){
    const float h_f =  r/PARTICLE_RADIUS;
    if (h_f >= 0 && h_f <= 1) {
        return 20.0f / (3.0f * M_PI * pow(PARTICLE_RADIUS, 3)) * (1.0f - h_f);
    }
    else {
        return 0;
    }

}

float inline calculate_pressure(Particle& p) {
    return STIFFNESS_CONSTANT * (pow(p.density / REST_DENSITY, 7) - 1);
}



void Particle::calculate_density(std::vector<Particle*>& Particle) {
    this->density = 0; 
    for (auto& p : Particle) {
        float r = glm::distance(position, p->position);                        
        if(this == p) continue; 
        
        this->density += density_kernel( r) * p->mass;
    }    
}
void Particle::calculate_fpressure(std::vector<Particle*>& Particle) {
    this->force = glm::vec3(0.0f, 0.0f, 0.0f);
    
    float this_particle_characteristic = calculate_pressure(*this) / (this->density * this->density); 

    float symm_formula = 0;

    for (auto& p : Particle) {
        float r = glm::distance(position, p->position);
        if(this == p) continue; 
        
        symm_formula = this_particle_characteristic  +  calculate_pressure(*p) / (p->density * p->density);
        glm::vec3 dir = p->position - position;
        this->force +=  dir * pressure_kernel( r) * p->mass * symm_formula * (-1.0f);
    }

}
void Particle::calculate_viscosity(std::vector<Particle*>& Particle) {
    glm::vec3 force_viscosity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    for (auto& p : Particle) {
        float r = glm::distance(position, p->position);
        if(this == p) continue; 

        force_viscosity += viscosity_kernel( r) * p->mass / p->density;
    }
    this->force += force_viscosity;
}


void Particle::update(Shader& sh, const float& dt) {
    // Update position
    acceleration = force / mass;
    velocity += acceleration;
    position += velocity * dt;

    if (position.y < 0.0f) {
        position.y = 0.0f;
        velocity.y *= -.8f; // bounce + damping
    }
    if (position.y > 10.0f) {
        position.y = 10.0f;
        velocity.y *= -.8f; // bounce + damping
    }
    if (position.x < -10.0f) {
        position.x = -10.0f;
        velocity.x  *= -.8f; // bounce + damping
    }
    if (position.x > 10.0f) {
        position.x = 10.0f;
        velocity.x  *= -.8f; // bounce + damping
    }
    if (position.z < -10.0f) {
        position.z = -10.0f;
        velocity.z  *= -.8f; // bounce + damping
    }
    if (position.z > 10.0f) {
        position.z = 10.0f;
        velocity.z  *= -.8f; // bounce + damping
    }

    esfera.centro = position;
    esfera.display(sh);    
}

