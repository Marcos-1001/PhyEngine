# Teoría SPH (y los bugs actuales mapeados)

Referencia base: **Müller, Charypar & Gross (2003)**, *Particle-Based Fluid Simulation for Interactive Applications*. Es de donde salen los kernels que ya tienes en `SPH/sph.cpp`.

Notación: `h` = smoothing length, `rᵢⱼ = xᵢ − xⱼ`, `r = |rᵢⱼ|`, `mⱼ` = masa, `ρ` = densidad, `p` = presión, `v` = velocidad.

## El loop de simulación (un paso `dt`)

```
1. Densidad:   ρᵢ = Σⱼ mⱼ · W_poly6(rᵢⱼ, h)         (incluye j = i)
2. Presión:    pᵢ = k · (ρᵢ − ρ₀)                    (gas ideal lineal)
3. Fuerzas:    fᵢ = f_presión + f_viscosidad + f_gravedad
4. Integrar:   aᵢ = fᵢ / ρᵢ ;  vᵢ += aᵢ·dt ;  xᵢ += vᵢ·dt
5. Fronteras:  clamp posición + reflejar velocidad (con amortiguación)
```

## Kernels (Müller 2003)

**Densidad — poly6:**
```
W_poly6(r, h) = 315/(64π h⁹) · (h² − r²)³        para 0 ≤ r ≤ h, si no 0
```
Auto-contribución (j = i, r = 0): `W_poly6(0,h) = 315/(64π h³)`.

**Gradiente de presión — spiky:**
```
∇W_spiky(r, h) = −45/(π h⁶) · (h − r)² · (rᵢⱼ / r)   para 0 < r ≤ h
```

**Laplaciano de viscosidad — viscosity (ESCALAR):**
```
∇²W_visc(r, h) = 45/(π h⁶) · (h − r)              para 0 ≤ r ≤ h
```

## Fuerzas

**Presión** (simétrica para conservar momento):
```
f_presiónᵢ = − Σⱼ≠ᵢ  mⱼ · (pᵢ + pⱼ)/(2ρⱼ) · ∇W_spiky(rᵢⱼ, h)
```

**Viscosidad:**
```
f_viscosidadᵢ = μ · Σⱼ≠ᵢ  mⱼ · (vⱼ − vᵢ)/ρⱼ · ∇²W_visc(rᵢⱼ, h)
```

**Gravedad:**
```
f_gravedadᵢ = ρᵢ · g
```

## Integración (semi-implícita / simpléctica de Euler)
```
aᵢ = fᵢ / ρᵢ
vᵢ ← vᵢ + dt · aᵢ
xᵢ ← xᵢ + dt · vᵢ
```
La de Euler semi-implícita (actualizar `v` antes que `x`) es estable y barata — usar esa, no la de Euler explícita.

## Fronteras
Si la partícula sale del dominio (`Domain`), clampear la posición al borde e invertir la componente de velocidad perpendicular con un coeficiente de restitución (ej. 0.5) para amortiguar el rebote.

---

## Los bugs actuales, mapeados a la teoría

| En `SPH/sph.cpp` / `particles.hpp` | Problema | Arreglo |
|---|---|---|
| `density_compute` | `ρ` nunca se reinicia a 0 → se acumula entre frames | Resetear `ρᵢ = 0` (o = auto-contribución) antes del loop de densidad cada paso |
| `density_compute` | Falta el término `j = i` | Sumar `mᵢ · W_poly6(0, h)` |
| `pressure_compute` | Vacío | Implementar `f_presión` con `∇W_spiky` |
| `viscosity_compute` | Vacío | Implementar `f_viscosidad` con `∇²W_visc` |
| `kernel_viscosity_laplacian` | Devuelve `vec3`, calcula `(h−r)·(rᵢⱼ/r)` | Debe ser **escalar**: `45/(π h⁶)·(h − r)` |
| `update()` | No integra ni aplica fuerzas | Añadir el paso 4 (integración) y gravedad |
| `particles.hpp` | Include guard `#define PARTICINLES_HPP` (typo) | `PARTICLES_HPP` |

Estos arreglos son el contenido del **Milestone 3** (y el typo/integración base ya en M2). Ver [`../ROADMAP.md`](../ROADMAP.md).

## Parámetros típicos para empezar (2D)
- `h` (smoothing length): ~ 2–4× el espaciado inicial entre partículas.
- `ρ₀` (rest density): según masa y `h`; calibrar para que un bloque en reposo dé `ρ ≈ ρ₀`.
- `k` (rigidez de presión): empezar pequeño y subir hasta que el fluido no se "explote".
- `μ` (viscosidad): pequeño; subir si el fluido tiembla demasiado.
- `dt`: pequeño y fijo (ej. 1–4 ms); SPH explícito es sensible al paso de tiempo.
