#include "ParticleSystem.hpp"

ParticleSystem2D::ParticleSystem2D()
{
}

ParticleSystem2D::ParticleSystem2D(const std::vector<Particle2D>& particles)
{
    this->particles = particles;
}

ParticleSystem2D::~ParticleSystem2D()
{
}

void ParticleSystem2D::step(float dt)
{
    for (auto& particle : particles)
    {
        particle.velocity += Vec2(0.0f, -GRAVITY) * dt; // Gravedad
        particle.position += particle.velocity * dt; // Actualizar posición
        domain.resolveCollision(particle);
    }
}

void ParticleSystem2D::step_nodomain(float dt)
{
    for (auto& particle : particles)
    {
        particle.velocity += Vec2(0.0f, -GRAVITY) * dt; // Gravedad
        particle.position += particle.velocity * dt; // Actualizar posición
    }
}

void ParticleSystem2D::addParticle(const Particle2D& particle)
{
    particles.push_back(particle);
}

const std::vector<Particle2D>& ParticleSystem2D::getParticles() const
{
    return particles;
}