#ifndef SPHSOLVER_HPP
#define SPHSOLVER_HPP


#include "ParticleSystem.hpp"
#include "Kernels.hpp"
class SphSolver
{
private:
     Scalar restDensity; // Densidad de reposo
     Scalar gasConstant; // Constante de gas para la ecuación de estado
     Scalar viscosity;   // Coeficiente de viscosidad
     Scalar smoothingLength; // Longitud de suavizado (h)
public:
    SphSolver();
    SphSolver(Scalar restDensity, Scalar gasConstant, Scalar viscosity, Scalar smoothingLength);

    ~SphSolver();
    void computeDensityPressure(ParticleSystem2D& system);
    void computeForces(ParticleSystem2D& system, Scalar dt);
};


#endif // SPHSOLVER_HPP