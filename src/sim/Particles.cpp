#include "Particles.hpp"


Particle2D::Particle2D()
{
    position = Vec2(0.0f, 0.0f);
    velocity = Vec2(0.0f, 0.0f);
    color = Vec3(0.0f, 0.0f, 0.0f);
    mass = 0.0f;
    density = 0.0f;
    pressure = 0.0f;
}

Particle2D::Particle2D(Vec2 position, Vec2 velocity, Vec3 color, Scalar mass)
{
    this->position = position;
    this->velocity = velocity;
    this->color = color;
    this->mass = mass;
    this->density = 0.0f;
    this->pressure = 0.0f;
}

Particle2D::~Particle2D()
{
}

