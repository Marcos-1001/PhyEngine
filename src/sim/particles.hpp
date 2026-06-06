#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <glm/glm.hpp>
#include "core/Math.hpp"
#include "core/Constants.hpp"

struct Particle2D
{
    Vec2 position;
    Vec2 velocity;        
    Vec3 color;     
    Scalar mass;

    Particle2D();
    Particle2D(Vec2 position, Vec2 velocity, Vec3 color, Scalar mass);    
    ~Particle2D();
    
    
};








#endif