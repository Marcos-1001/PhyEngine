// M4 — Smoke test de estabilidad (headless).
// Reproduce el escenario de la app (bloque de 30×30 partículas, mismos parámetros
// y dt/substeps) y corre muchos pasos de SphSolver::step. Tras la simulación todas
// las posiciones deben seguir siendo FINITAS y dentro de la caja del dominio: si las
// fuerzas explotaran (NaN/inf) o el grid fallara, esto lo detecta. No reemplaza la
// inspección visual, pero garantiza que la sim no revienta.

#include "sim/SphSolver.hpp"
#include "sim/ParticleSystem.hpp"
#include "core/Math.hpp"

#include <vector>
#include <cmath>
#include <iostream>

int main()
{
    const int   side    = 30;
    const float spacing = 8.0f;
    const float pmass   = 64.0f;

    std::vector<Particle2D> ps(side * side);
    for (int i = 0; i < side; ++i)
        for (int j = 0; j < side; ++j)
            ps[i * side + j] = Particle2D(Vec2(350.0f + i * spacing, 350.0f + j * spacing),
                                          Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), pmass);

    ParticleSystem2D system(ps);
    SphSolver solver(1.0f, 2000.0f, 0.1f, 16.0f, 0.0f); // mismos parámetros que la app (sin tensión)

    const float dt = 0.004f;
    const int frames = 200, substeps = 4;
    for (int f = 0; f < frames; ++f)
        for (int s = 0; s < substeps; ++s)
            solver.step(system, dt);

    const Domain2D& d = system.getDomain();
    int bad = 0;
    for (size_t i = 0; i < system.size(); ++i)
    {
        const Vec2 p = system.getParticles()[i].position;
        const bool finite = std::isfinite(p.x) && std::isfinite(p.y);
        const bool inBox = finite && p.x >= d.min.x && p.x <= d.max.x &&
                                     p.y >= d.min.y && p.y <= d.max.y;
        if (!finite || !inBox)
        {
            if (bad < 5)
                std::cerr << "Partícula " << i << " inestable: (" << p.x << ", " << p.y << ")\n";
            ++bad;
        }
    }

    if (bad)
    {
        std::cerr << "FAIL: " << bad << " partículas fuera de la caja o no finitas tras "
                  << frames * substeps << " pasos\n";
        return 1;
    }
    std::cout << "OK: " << system.size() << " partículas estables tras "
              << frames * substeps << " pasos\n";
    return 0;
}
