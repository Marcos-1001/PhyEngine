#include "SphSolver.hpp"

SphSolver::SphSolver()
{
    // Parámetros 2D auto-consistentes. restDensity DEBE coincidir con la densidad que
    // el kernel produce en reposo (≈ mass·Σpoly6); con mass=64, d=8, h=16 sale ~1.
    // gasConstant / viscosity son knobs de TUNING: ajústalos viendo correr la sim.
    restDensity = 1.0f;
    gasConstant = 500.0f;
    viscosity = 0.8f;
    smoothingLength = 16.0f; // h
}

SphSolver::SphSolver(Scalar restDensity, Scalar gasConstant, Scalar viscosity, Scalar smoothingLength)
{
    this->restDensity = restDensity;
    this->gasConstant = gasConstant;
    this->viscosity = viscosity;
    this->smoothingLength = smoothingLength;
}


SphSolver::~SphSolver()
{
}

void SphSolver::computeDensityPressure(ParticleSystem2D& system)
{
    size_t systemSize = system.size();
    for (size_t i = 0; i < systemSize; ++i)
    {
        system[i].density = 0.0f;
        for(size_t j = 0; j < systemSize; ++j)
        {
            system[i].density += system[j].mass * poly6Kernel2D(
                glm::distance(system[i].position, system[j].position), 
                this->smoothingLength);
        }

        system[i].pressure = gasConstant * (system[i].density - restDensity);
    }
}

void SphSolver::computeForces(ParticleSystem2D& system, Scalar dt)
{
    // Implementar el cálculo de fuerzas (presión, viscosidad, etc.) para cada partícula
    size_t systemSize = system.size();

    for (size_t i = 0; i < systemSize; ++i)
    {
        Vec2 pressureForce(0.0f);
        Vec2 viscosityForce(0.0f);

        for(size_t j = 0; j < systemSize; ++j)
        {
            if (i == j) continue;

            Vec2 dir = system[i].position - system[j].position;
            Scalar r = glm::length(dir);
            if (r > smoothingLength) continue;

            // Fuerza de presión
            pressureForce += -system[j].mass * (system[i].pressure + system[j].pressure) / (2.0f * system[j].density) * spikyKernel2D(r, smoothingLength, dir);

            // Fuerza de viscosidad
            viscosityForce += viscosity * system[j].mass * (system[j].velocity - system[i].velocity) / system[j].density * viscosityKernel2D(r, smoothingLength);
        }
        
        // Actualizar la velocidad de la partícula con las fuerzas calculadas
        system[i].velocity += (pressureForce + viscosityForce) / system[i].density * dt;
    }
}

