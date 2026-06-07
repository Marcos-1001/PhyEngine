#include "Kernels.hpp"

Scalar poly6Kernel2D(Scalar r, Scalar h)
{
    if (r < 0 || r > h) return 0.0f;
    if (r == 0) return 4.0f / (PI * pow(h, 8)); // Evitar división por cero
    Scalar coeff = 4.0f / ( PI * pow(h, 8));
    return coeff * pow(h * h - r * r, 3);
}

Vec2 spikyKernel2D(Scalar r, Scalar h, const Vec2& dir)
{
    if (r <= 1e-6f || r > h) return Vec2(0.0f, 0.0f); // guard r→0: sin esto /r → ∞ al colapsar
    Scalar coeff = -30.0f / (PI * pow(h, 5));
    Scalar factor = coeff * pow(h - r, 2) / r;
    return factor * dir;
}

Scalar viscosityKernel2D(Scalar r, Scalar h)
{
    if (r < 0 || r > h) return 0.0f;
    Scalar coeff = 40.0f / (PI * pow(h, 5)); // 2D (antes tenía la constante 3D 45/(πh⁶))
    return coeff * (h - r);
}



Scalar poly6Kernel3D(Scalar r, Scalar h)
{
    if (r < 0 || r > h) return 0.0f;
    if (r == 0) return 315.0f / (64.0f * PI * pow(h, 9)); // Evitar división por cero
    Scalar coeff = 315.0f / (64.0f * PI * pow(h, 9));
    return coeff * pow(h * h - r * r, 3);
}

Vec3 spikyKernel3D(Scalar r, Scalar h, const Vec3& dir)
{
    if (r < 0 || r > h) return Vec3(0.0f, 0.0f, 0.0f);
    Scalar coeff = -45.0f / (PI * pow(h, 6));
    Scalar factor = coeff * pow(h - r, 2) / r;
    return factor * dir;
}

Scalar viscosityKernel3D(Scalar r, Scalar h)
{
    if (r < 0 || r > h) return 0.0f;
    Scalar coeff = 45.0f / (PI * pow(h, 6));
    return coeff * (h - r);
}
