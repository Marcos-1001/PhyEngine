#include "SphSolver.hpp"

SphSolver::SphSolver()
{
    // Parámetros 2D auto-consistentes. restDensity DEBE coincidir con la densidad
    // que el kernel produce en reposo (≈ mass·Σpoly6); con mass=64, d=8, h=16 ≈ 1.
    // gasConstant / viscosity son knobs de TUNING: ajústalos viendo correr la sim.
    restDensity = 1.0f;
    gasConstant = 500.0f;
    viscosity = 0.8f;
    smoothingLength = 16.0f; // h
    surfaceTension = 0.0728f; // coeficiente de tensión superficial para agua a temperatura ambiente
}

SphSolver::SphSolver(Scalar restDensity, Scalar gasConstant, Scalar viscosity, Scalar smoothingLength, Scalar surfaceTension)
{
    this->restDensity = restDensity;
    this->gasConstant = gasConstant;
    this->viscosity = viscosity;
    this->smoothingLength = smoothingLength;
    this->surfaceTension = surfaceTension;
}

SphSolver::~SphSolver()
{
}

void SphSolver::rebuildGrid(ParticleSystem2D& system)
{
    const size_t n = system.size();

    
    if (!m_configured)
    {
        grid.configure(smoothingLength, n * 2);
        m_configured = true;
    }

    // Posiciones actuales → reconstruir el grid desde cero cada substep.
    m_positions.resize(n);
    for (size_t i = 0; i < n; ++i)
        m_positions[i] = system[i].position;
    grid.build(m_positions);

    // Vecinos candidatos (3×3) por partícula; el filtro exacto r≤h lo aplican
    // densidad/fuerzas (los kernels valen 0 para r>h).
    m_neighbors.resize(n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        grid.queryNeighbors(m_positions[i], smoothingLength, m_neighbors[i]);
}

void SphSolver::computeDensityPressure(ParticleSystem2D& system)
{
    const size_t n = system.size();
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        Scalar density = 0.0f;
        // Los candidatos incluyen a la propia partícula (r=0) ⇒ auto-contribución.
        for (size_t j : m_neighbors[i])
        {
            density += system[j].mass * poly6Kernel2D(
                glm::distance(system[i].position, system[j].position),
                this->smoothingLength);
        }
        system[i].density = std::max(restDensity*0.1f, density);
        system[i].pressure = std::max(0.0f, gasConstant * (density - restDensity));
    }
}

void SphSolver::computeForces(ParticleSystem2D& system, Scalar dt)
{
    const size_t n = system.size();
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        Vec2 pressureForce(0.0f);
        Vec2 viscosityForce(0.0f);
        Vec2 surfaceForce(0.0f);
        Vec2 normal(0.0f);
        Scalar curvature = 0.0f;
        for (size_t j : m_neighbors[i])
        {
            if (i == j) continue;

            Vec2 dir = system[i].position - system[j].position;
            Scalar r = glm::length(dir);
            if (r > smoothingLength) continue;

            // Fuerza de presión (simétrica entre i y j).
            pressureForce += -system[j].mass * (system[i].pressure + system[j].pressure)
                             / (2.0f * system[j].density) * spikyKernel2D(r, smoothingLength, dir);

            // Fuerza de viscosidad.
            viscosityForce += viscosity * system[j].mass * (system[j].velocity - system[i].velocity)
                              / system[j].density * viscosityKernel2D(r, smoothingLength);
            
            normal += system[j].mass / system[j].density * poly6GradientKernel2D(r, smoothingLength, dir);
            curvature += system[j].mass / system[j].density * poly6LaplacianKernel2D(r, smoothingLength);

        }
        //std::cout<< "Normal: " << glm::length(normal) << std::endl;
        if (glm::length(normal) > 0.0001f)
        {
            surfaceForce = -surfaceTension * curvature * normal / glm::length(normal);
        }   
        system[i].velocity += (pressureForce + viscosityForce + surfaceForce) / system[i].density * dt;
        //Scalar colorFactor = glm::length(normal) / (system[i].density * smoothingLength);
        system[i].color = glm::mix(interior, surface, std::min(1.0f, system[i].density / restDensity));
    }
}

void SphSolver::step(ParticleSystem2D& system, Scalar dt)
{
    rebuildGrid(system);
    computeDensityPressure(system);
    computeForces(system, dt);
    system.step(dt);
}
