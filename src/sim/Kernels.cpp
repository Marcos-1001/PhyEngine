#include "Kernels.hpp"

Scalar poly6Kernel2D(Scalar r, Scalar h)
{
    if (r < 0 || r > h) return 0.0f;
    // poly6 NO tiene singularidad en r=0 (no divide por r), así que la fórmula
    // general ya da el valor correcto: coeff·(h²)³. Un caso especial r==0 que
    // devolviera coeff sin el factor (h²)³ anularía la auto-contribución de
    // densidad → densidad≈0 en partículas aisladas → /density explota.
    Scalar coeff = 4.0f / ( PI * pow(h, 8));
    return coeff * pow(h * h - r * r, 3);
}

Vec2 poly6GradientKernel2D(Scalar r, Scalar h, const Vec2& dir)
{
    if (r < 0 || r > h) return Vec2(0.0f, 0.0f);
    Scalar coeff = (-24.0f / (PI * pow(h, 8))) * pow(h * h - r * r, 2);
    return coeff * dir;
}

Scalar poly6LaplacianKernel2D(Scalar r, Scalar h)
{
    if (r < 0 || r > h) return 0.0f;
    Scalar coeff = (-48.0f / (PI * pow(h, 8)))  * (h * h - r * r) * (h * h - 3.0f * r * r);
    return coeff * (h * h - r * r) * (5.0f * r * r - h * h);
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
    // Igual que en 2D: poly6 no diverge en r=0; la fórmula general da coeff·(h²)³.
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
