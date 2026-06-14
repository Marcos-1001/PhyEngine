// Infra — gravedad configurable por el constructor de ParticleSystem2D.
// Verifica que el valor de gravedad pasado al ctor (no el #define GRAVITY) es el
// que usa la integración: con g=0 no acelera; con g custom acelera a esa tasa.

#include "sim/ParticleSystem.hpp"
#include "sim/Domain.hpp"
#include "core/Math.hpp"

#include <cmath>
#include <iostream>

static int failures = 0;
static void chk(bool ok, const char* msg)
{
    if (!ok) { std::cerr << "FAIL: " << msg << "\n"; ++failures; }
}
static bool close(float a, float b) { return std::abs(a - b) < 1e-4f; }

int main()
{
    const Domain2D dom(Vec2(0.0f, 0.0f), Vec2(800.0f, 600.0f), 0.2f);
    const Particle2D p(Vec2(400.0f, 300.0f), Vec2(0.0f, 0.0f), Vec3(0, 0, 1), 1.0f);

    // g = 0 → sin aceleración.
    {
        ParticleSystem2D sys({p}, dom, 0.0f);
        sys.step_nodomain(0.1f);
        const Particle2D& q = sys.getParticles()[0];
        chk(close(q.velocity.y, 0.0f), "g=0: velocidad no cambia");
        chk(close(q.position.y, 300.0f), "g=0: posición no cambia");
    }

    // g = 20 → vy = -20·0.1 = -2.0 ; y = 300 + (-2.0)·0.1 = 299.8.
    {
        ParticleSystem2D sys({p}, dom, 20.0f);
        sys.step_nodomain(0.1f);
        const Particle2D& q = sys.getParticles()[0];
        chk(close(q.velocity.y, -2.0f), "g=20: vy = -g·dt");
        chk(close(q.position.y, 299.8f), "g=20: y = y0 + vy·dt");
    }

    if (failures) { std::cerr << failures << " checks fallaron\n"; return 1; }
    std::cout << "OK: gravedad configurable por ctor\n";
    return 0;
}
