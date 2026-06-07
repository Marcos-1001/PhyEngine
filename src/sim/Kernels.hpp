#ifndef KERNELS_HPP
#define KERNELS_HPP

#include "core/Math.hpp"
#include "core/Constants.hpp"

// 2D (los que usa el SphSolver)
Scalar poly6Kernel2D(Scalar r, Scalar h);
Vec2   spikyKernel2D(Scalar r, Scalar h, const Vec2& dir);
Scalar viscosityKernel2D(Scalar r, Scalar h);

// 3D (para el futuro)
Scalar poly6Kernel3D(Scalar r, Scalar h);
Vec3   spikyKernel3D(Scalar r, Scalar h, const Vec3& dir);
Scalar viscosityKernel3D(Scalar r, Scalar h);



#endif // KERNELS_HPP