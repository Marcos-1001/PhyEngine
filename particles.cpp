#include "particles.h"


float inline density_kernel(Particle& p1, Particle& p2, float r) {
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

float inline calculate_pressure(Particle& p, float k) {
    return STIFFNESS_CONSTANT * (pow(p.density / REST_DENSITY, 7) - 1);
}



void Particle::calculate_density(std::vector<Particle>& Particle) {
    this->density = 0; 
    for (auto& p : Particle) {
        float r = glm::distance(position, p.position);                
        this->density += density_kernel(*this, p, r);
    }    
}



void Particle::update(float& dt) {
    // Update position
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
}

