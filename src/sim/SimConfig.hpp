#ifndef SIMCONFIG_HPP
#define SIMCONFIG_HPP

#include <istream>
#include <string>

// Parámetros de la simulación, cargables desde un archivo `key = value`.
// Los defaults son el set estable recomendado (en unidades de sim, no SI).
struct SimConfig
{
    // --- Solver ---
    float restDensity      = 1.0f;
    float gasConstant      = 2000.0f;
    float viscosity        = 0.3f;
    float smoothingLength  = 16.0f;   // h
    float surfaceTension   = 0.0f;    // 0 = sin tensión hasta tunear
    float surfaceThreshold = 0.01f;   // |n| mínimo para considerar superficie

    // --- Escena ---
    int   numParticles     = 30;      // por lado → numParticles² partículas
    float spacing          = 8.0f;    // d (< h)
    float mass             = 80.0f;
    float originX          = 350.0f;  // esquina del bloque inicial
    float originY          = 350.0f;

    // --- Integración ---
    float dt               = 0.004f;
    int   substeps         = 6;

    // --- Mundo ---
    float gravity          = 9.81f;
    float restitution      = 0.2f;
    float domainMinX       = 0.0f;
    float domainMinY       = 0.0f;
    float domainMaxX       = 800.0f;
    float domainMaxY       = 600.0f;

    // --- Render ---
    float pointSize        = 30.0f;
};

// Aplica sobre `base` las claves `key = value` leídas de `in`. Las líneas vacías
// y el texto desde '#' se ignoran (comentarios). Claves desconocidas o valores
// no numéricos se avisan por stderr y se saltan (conservan su default).
SimConfig parseConfig(std::istream& in, SimConfig base = SimConfig{});

// Carga desde archivo; si no se puede abrir, avisa por stderr y devuelve defaults.
SimConfig loadConfig(const std::string& path);

#endif // SIMCONFIG_HPP
