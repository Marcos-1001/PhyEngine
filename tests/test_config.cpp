// Infra — carga de parámetros desde archivo `key = value`.
// Testea el parser (sobre istringstream, sin tocar disco) y el fallback de
// archivo inexistente. Verifica: overrides aplicados, claves ausentes conservan
// su default, valores malformados y claves desconocidas se ignoran.

#include "sim/SimConfig.hpp"

#include <sstream>
#include <cmath>
#include <iostream>

static int failures = 0;
static void chk(bool ok, const char* msg)
{
    if (!ok) { std::cerr << "FAIL: " << msg << "\n"; ++failures; }
}
static bool close(float a, float b) { return std::abs(a - b) < 1e-5f; }

int main()
{
    // 1. Overrides (con/sin espacios), comentarios e int.
    {
        std::istringstream in(
            "gasConstant = 1234\n"
            "# esto es un comentario\n"
            "viscosity=0.5\n"
            "numParticles = 12\n"
            "restitution = 0.3\n"
            "gravity = 5.0\n");
        SimConfig c = parseConfig(in);
        chk(close(c.gasConstant, 1234.0f), "gasConstant override");
        chk(close(c.viscosity, 0.5f),      "viscosity override (sin espacios)");
        chk(c.numParticles == 12,          "numParticles override (int)");
        chk(close(c.restitution, 0.3f),    "restitution override");
        chk(close(c.gravity, 5.0f),        "gravity override");
        chk(close(c.restDensity, 1.0f),    "clave ausente conserva default");
    }

    // 2. Valor malformado y clave desconocida se ignoran; lo válido se aplica.
    {
        std::istringstream in(
            "gasConstant = noEsNumero\n"
            "claveDesconocida = 9\n"
            "viscosity = 0.7\n");
        SimConfig c = parseConfig(in);
        chk(close(c.gasConstant, 2000.0f), "valor malformado se ignora (default)");
        chk(close(c.viscosity, 0.7f),      "línea válida tras una inválida");
    }

    // 3. Archivo inexistente → defaults.
    {
        SimConfig c = loadConfig("/ruta/que/no/existe/sim.conf");
        chk(close(c.gasConstant, 2000.0f) && close(c.dt, 0.004f),
            "archivo inexistente → defaults");
    }

    if (failures) { std::cerr << failures << " checks fallaron\n"; return 1; }
    std::cout << "OK: SimConfig parsea correctamente\n";
    return 0;
}
