// M3/M4 — Regresión del rebote.
// El test `stability` solo corre 800 pasos (~3.2 s): NO alcanza a tocar el piso
// (el bloque tarda ~7.5 s en caer desde y≈350). Este test corre lo suficiente para
// CAER, REBOTAR y seguir varios segundos después, y asevera que la sim no revienta:
// posiciones finitas y velocidad acotada en todo momento.

#include "sim/SphSolver.hpp"
#include "sim/ParticleSystem.hpp"
#include "core/Math.hpp"

#include <vector>
#include <cmath>
#include <iostream>

int main()
{
    const int side = 30; const float spacing = 8.0f, pmass = 64.0f;
    std::vector<Particle2D> ps(side * side);
    for (int i = 0; i < side; ++i)
        for (int j = 0; j < side; ++j)
            ps[i * side + j] = Particle2D(Vec2(350.0f + i * spacing, 350.0f + j * spacing),
                                          Vec2(0.0f, 0.0f), Vec3(0, 0, 1), pmass);

    ParticleSystem2D system(ps);
    SphSolver solver(1.0f, 2000.0f, 0.1f, 16.0f, 0.0f); // = main.cpp (sin tensión)

    const float dt = 0.004f; const int substeps = 4, frames = 800;
    const float MAX_SPEED = 1000.0f; // muy por encima del régimen normal (~400), muy por debajo del reventón

    for (int f = 0; f < frames; ++f)
    {
        for (int s = 0; s < substeps; ++s) solver.step(system, dt);

        for (size_t k = 0; k < system.size(); ++k)
        {
            const Particle2D& p = system.getParticles()[k];
            const bool finite = std::isfinite(p.position.x) && std::isfinite(p.position.y);
            if (!finite || glm::length(p.velocity) > MAX_SPEED)
            {
                std::cerr << "FAIL: reventón en frame " << f << " (t=" << f * substeps * dt
                          << "s), partícula " << k << " speed=" << glm::length(p.velocity)
                          << " pos=(" << p.position.x << "," << p.position.y << ")\n";
                return 1;
            }
        }
    }

    std::cout << "OK: " << system.size() << " partículas estables a través del rebote ("
              << frames * substeps << " pasos)\n";
    return 0;
}
