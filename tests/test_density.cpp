// M4 — Regresión de la integración grid↔solver.
// SphSolver::step() calcula la densidad usando los vecinos del grid en vez del
// doble bucle O(n²). El resultado debe ser idéntico (salvo reordenamiento en coma
// flotante) a la densidad de fuerza bruta: density_i = Σ_j mass_j · poly6(r_ij, h).
// step() calcula la densidad sobre las posiciones de INICIO del substep (antes de
// integrar), así que comparamos contra la fuerza bruta sobre esas mismas posiciones.

#include "sim/SphSolver.hpp"
#include "sim/ParticleSystem.hpp"
#include "sim/Kernels.hpp"
#include "core/Math.hpp"

#include <vector>
#include <cmath>
#include <iostream>

int main()
{
    const float h    = 16.0f;
    const float mass = 64.0f;

    std::vector<Particle2D> ps;
    for (int gx = 0; gx < 10; ++gx)
        for (int gy = 0; gy < 10; ++gy)
            ps.push_back(Particle2D(Vec2(100.0f + gx * 8.0f, 100.0f + gy * 8.0f),
                                    Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), mass));

    // Posiciones de referencia (las de inicio del substep).
    std::vector<Vec2> pos0(ps.size());
    for (size_t i = 0; i < ps.size(); ++i) pos0[i] = ps[i].position;

    ParticleSystem2D system(ps);
    SphSolver solver(1.0f, 500.0f, 0.8f, h, 0.0f);
    solver.step(system, 0.004f);

    int failures = 0;
    for (size_t i = 0; i < system.size(); ++i)
    {
        float brute = 0.0f;
        for (size_t j = 0; j < pos0.size(); ++j)
            brute += mass * poly6Kernel2D(glm::distance(pos0[i], pos0[j]), h);

        const float got = system.getParticles()[i].density;
        const float tol = 1e-3f * std::max(1.0f, std::abs(brute));
        if (std::abs(got - brute) > tol)
        {
            if (failures < 5)
                std::cerr << "Densidad discrepante en partícula " << i
                          << ": grid=" << got << "  fuerza bruta=" << brute << "\n";
            ++failures;
        }
    }

    if (failures)
    {
        std::cerr << "FAIL: " << failures << " densidades fuera de tolerancia\n";
        return 1;
    }
    std::cout << "OK: densidad grid == fuerza bruta para " << system.size() << " partículas\n";
    return 0;
}
