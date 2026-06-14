#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include "Particles.hpp"


struct Domain2D
{
    Vec2 min;
    Vec2 max;
    Scalar restitution; // Coeficiente de restitución para colisiones con los bordes

    Domain2D(Vec2 min, Vec2 max, Scalar restitution = 0.2f) : min(min), max(max), restitution(restitution) {}
    Domain2D() : min(Vec2(0.0f, 0.0f)), max(Vec2(800.0f, 600.0f)), restitution(0.8f) {}
    void resolveCollision(Particle2D& particle) const
    {
        // Colisión con el borde izquierdo
        if (particle.position.x < min.x)
        {
            particle.position.x = min.x;
            particle.velocity.x *= -restitution;
        }
        // Colisión con el borde derecho
        else if (particle.position.x > max.x)
        {
            particle.position.x = max.x;
            particle.velocity.x *= -restitution;
        }
        // Colisión con el borde inferior
        if (particle.position.y < min.y)
        {
            particle.position.y = min.y;
            particle.velocity.y *= -restitution;
        }
        // Colisión con el borde superior
        else if (particle.position.y > max.y)
        {
            particle.position.y = max.y;
            particle.velocity.y *= -restitution;
        }
    }
};
#endif // DOMAIN_HPP