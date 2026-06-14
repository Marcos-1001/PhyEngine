#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP


#include "Particles.hpp"
#include "Domain.hpp"
#include <vector>


class ParticleSystem2D
{
private:
    std::vector<Particle2D> particles;
    Domain2D domain;
    Scalar gravity = GRAVITY; // configurable; por defecto el #define de Constants
public:
    ParticleSystem2D();
    ParticleSystem2D(const std::vector<Particle2D>& particles);
    ParticleSystem2D(const std::vector<Particle2D>& particles, const Domain2D& domain, Scalar gravity);
    ~ParticleSystem2D();
    void addParticle(const Particle2D& particle);
    const std::vector<Particle2D>& getParticles() const;
    void step(float dt);
    void step_nodomain(float dt);
    
    size_t size() const { return particles.size(); }
    Particle2D& operator[](size_t index) { return particles[index]; }
    const Domain2D& getDomain() const { return domain; }
};

#endif // PARTICLESYSTEM_HPP