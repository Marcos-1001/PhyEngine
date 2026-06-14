#ifndef SPHSOLVER_HPP
#define SPHSOLVER_HPP

#include <vector>
#include "ParticleSystem.hpp"
#include "Kernels.hpp"
#include "UniformGrid.hpp"
#include <omp.h>
#include <iostream>
const Vec3 interior(0.0f, 0.1f, 0.6f);   // azul profundo
const Vec3 surface (0.4f, 0.9f, 1.0f);   // cian claro


class SphSolver
{
private:
    Scalar restDensity;     // Densidad de reposo
    Scalar gasConstant;     // Constante de gas para la ecuación de estado
    Scalar viscosity;       // Coeficiente de viscosidad
    Scalar smoothingLength; // Longitud de suavizado (h)
    Scalar surfaceTension; // Coeficiente de tensión superficial (para agua a temperatura ambiente)
    
    UniformGrid grid;                              // Neighbor search acelerado
    bool m_configured = false;                     // grid dimensionado al dominio
    std::vector<Vec2> m_positions;                 // posiciones cacheadas por substep
    std::vector<std::vector<size_t>> m_neighbors;  // candidatos 3×3 por partícula

    // Reconstruye el grid con las posiciones actuales y recalcula las listas de
    // vecinos candidatos (configura el grid al dominio la primera vez).
    void rebuildGrid(ParticleSystem2D& system);
    void computeDensityPressure(ParticleSystem2D& system); // usa m_neighbors
    void computeForces(ParticleSystem2D& system, Scalar dt); // usa m_neighbors

public:
    SphSolver();
    SphSolver(Scalar restDensity, Scalar gasConstant, Scalar viscosity, Scalar smoothingLength, Scalar surfaceTension);
    ~SphSolver();

    // Un substep completo de SPH: reconstruye el grid, calcula densidad/presión y
    // fuerzas usando los vecinos del grid, e integra (gravedad + dominio).
    void step(ParticleSystem2D& system, Scalar dt);
};

#endif // SPHSOLVER_HPP
