#include "particles.h"
#define ALPHA_CONSTANT 4.0f / (M_PI * pow(PARTICLE_RADIUS, 3));
glm::vec3 inline random_direction() {
    return glm::vec3(
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
    );
}
float inline density_kernel( float r) {
    const float h_f =  r/PARTICLE_RADIUS;
    const float t1 = 1.f - h_f;//(1.0f - h_f,0  > 0) ? 1.0f - h_f : 0.0f ;
    const float t2 = 2.f - h_f;//(2.0f - h_f,0 > 0) ? 2.0f - h_f : 0.0f;
    if (h_f >= 0 && h_f <= 1) {
        return pow(t2,3) - 4*pow(t1,3);
    }
    else if (h_f > 1 && h_f <= 2) {
        return pow(t2,3);
    }
    else {
        return 0;
    }
}
float inline pressure_kernel( float r) {
    const float h_f =  r/PARTICLE_RADIUS;
    const float t1 = 1.f - h_f;//(1.0f - h_f,0 >0) ? 1.0f - h_f : 0.0f ;
    const float t2 = 2.f - h_f;//(2.0f - h_f,0 > 0) ? 2.0f - h_f : 0.0f;
    if (h_f >= 0 && h_f <= 1) {
        return -3*pow(t2, 2) + 12*pow(t1, 2);
    }
    else if (h_f > 1 && h_f <= 2) {
        return -3*pow(t2, 2);
    }
    else {
        return 0;
    }
}
float inline viscosity_kernel(float r){
    const float h_f =  r/PARTICLE_RADIUS;
    const float t1 = 1.f - h_f;//(1.0f - h_f,0 >0) ? 1.0f - h_f : 0.0f ;
    const float t2 = 2.f - h_f;//(2.0f - h_f,0 > 0) ? 2.0f - h_f : 0.0f;
    if (h_f >= 0 && h_f <= 1) {
        return -6*t2 + 24*1;
    }
    else if (h_f > 1 && h_f <= 2) {
        return -6*t2;
    }
    else {
        return 0;
    }
    /*if (h_f >= 0 && h_f <= 1) {
        return 20.0f / (3.0f * M_PI * pow(PARTICLE_RADIUS, 3)) * (1.0f - h_f);
    }
    else {
        return 0;
    }*/

}

float inline calculate_pressure(Particle& p) {
    //return STIFFNESS_CONSTANT * (pow(p.density / REST_DENSITY, 7) - 1);
    //cout<<max(STIFFNESS_CONSTANT * (p.density - REST_DENSITY), 0.0f)<<endl;
    return STIFFNESS_CONSTANT * (p.density/REST_DENSITY - 1);
}



void Particle::calculate_density(std::vector<Particle*>& Particle) {
    this->density = 0; 
    int continues = 0;
    for (auto& p : Particle) {
        float r = glm::distance(position, p->position);                        
        if(this == p || r > PARTICLE_RADIUS){ 
            continues++;
            continue; 
        }
        
        this->density += density_kernel( r) * p->mass;
    }   
    
}
void Particle::calculate_fpressure(std::vector<Particle*>& Particle) {
    this->force = glm::vec3(0.0f, 0.0f, 0.0f);
    
    float this_particle_characteristic = calculate_pressure(*this) / (this->density * this->density); 

    float symm_formula = 0;
    int continues = 0;
    for (auto& p : Particle) {
        float r = glm::distance(position, p->position);
        if(this == p ){ 
            continues++;
            continue; 
        }
        else if( r > PARTICLE_RADIUS ){
            continue;
        }
        
        symm_formula = this_particle_characteristic  +  calculate_pressure(*p) / (p->density * p->density);
        glm::vec3 dir = (r == 0.0f ) ? random_direction() :   glm::normalize(p->position - position);
        this->force +=  dir * pressure_kernel( r) * p->mass * symm_formula * (-1.0f);
    }


}
void Particle::calculate_viscosity(std::vector<Particle*>& Particle) {
    glm::vec3 force_viscosity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    for (auto& p : Particle) {
        float r = glm::distance(position, p->position);
        if(this == p || r > PARTICLE_RADIUS) continue; 
        glm::vec3 diff_vel =  this->velocity -p->velocity;
        glm::vec3 diff_pos =  this->position - p->position;
        float magnitude = glm::length(diff_pos)*glm::length(diff_pos);
        glm::vec3 dir = (r == 0.0f ) ? random_direction() :   glm::normalize(this->position - p->position);
        force_viscosity +=  dir * viscosity_kernel( r) * p->mass / p->density * dot(diff_vel , diff_pos) /(magnitude + 0.01f*PARTICLE_RADIUS*PARTICLE_RADIUS); ;
    }
    this->force += force_viscosity*VISCOSITY_CONSTANT;
}


void Particle::update(Shader& sh, const float& dt) {
    // Update position
    acceleration = force / mass + glm::vec3(0.0f, -9.8f, 0.0f);
    velocity += acceleration;
    position += velocity * dt;

    if (position.y < 0.0f) {
        position.y = 0.0f;
        velocity.y *= -.8f; // bounce + damping
    }
    if (position.y > BOX_SIZE) {
        position.y = BOX_SIZE;
        velocity.y *= -.8f; // bounce + damping
    }
    if (position.x < -BOX_SIZE) {
        position.x = -BOX_SIZE;
        velocity.x  *= -.8f; // bounce + damping
    }
    if (position.x > BOX_SIZE) {
        position.x = BOX_SIZE;
        velocity.x  *= -.8f; // bounce + damping
    }
    if (position.z < -BOX_SIZE) {
        position.z = -BOX_SIZE;
        velocity.z  *= -.8f; // bounce + damping
    }
    if (position.z > BOX_SIZE) {
        position.z = BOX_SIZE;
        velocity.z  *= -.8f; // bounce + damping
    }
    sh.use();
    esfera.centro = position;
    //cout<<this->acceleration.x<<" "<<this->acceleration.y<<" "<<this->acceleration.z<<endl;
    //cout<<this->force.x<<" "<<this->force.y<<" "<<this->force.z<<endl;

    //cout<<position.x<<" "<<position.y<<" "<<position.z<<endl;
    //cout<<velocity.x<<" "<<velocity.y<<" "<<velocity.z<<endl;
    esfera.display(sh);    
}

