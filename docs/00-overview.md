# PhyEngine — Diseño general

Motor de física en C++ pensado como **medio didáctico para aprender computer graphics**, construido en abierto para que la gente pueda contribuir. El primer módulo es un **simulador de fluidos SPH en 2D**.

Seguimiento de tareas: [`../ROADMAP.md`](../ROADMAP.md) · Teoría: [`01-sph-theory.md`](01-sph-theory.md) · Referencias: [`references.md`](references.md)

## Metas
1. Implementar un motor de física propio en C++.
2. Empezar por un fluido SPH (objetivo inmediato).
3. Repo abierto y colaborativo.
4. Que leer el código **enseñe** computer graphics.

## Decisiones de diseño y por qué

| Decisión | Elección | Por qué |
|----------|----------|---------|
| API gráfica | **OpenGL ahora**, Vulkan después | Vulkan son ~1000 líneas antes del primer triángulo; frenaría el objetivo SPH. OpenGL ya funciona en el repo y enseña los fundamentos de CG igual. Vulkan entra como `VulkanRenderer` en el milestone GPU. |
| Dimensión | **2D primero** | Debugging visual y obvio; corre rápido; los kernels son idénticos conceptualmente a 3D. Subir a 3D reusa casi todo. |
| Cómputo | **CPU primero** | Claridad sobre rendimiento mientras se aprende. GPU/compute es un milestone posterior. |
| Estructura | **Motor por capas**, SPH como módulo 1 | Sirve a las 3 metas (didáctico, colaborativo, motor) sin sobre-ingeniería (YAGNI: nada para módulos que aún no existen). |
| Datos | `std::vector<Particle>` por valor | Sin fugas ni `new/delete`, cache-friendly, más simple que `vector<Particle*>`. |
| Math | **GLM** | Estándar de la industria, header-only. |
| Build | **CMake** | Compila en Windows/Mac/Linux (el Makefile actual es solo Linux y bloquea colaboradores). |
| Tests | **doctest** | Un solo header; permite verificar la física sin GPU. |

## Arquitectura: 4 capas, dependencias en una sola dirección

```
app      →  pega todo: ventana, loop, input, cámara, demo
 │
render   →  dibuja (OpenGL detrás de IRenderer). NO sabe de física.
 │
sim      →  física pura SPH. Headless, testeable. NO sabe de OpenGL.
 │
core     →  math (Vec2/Vec3, alias de glm), constantes, timer. Sin dependencias.
```

**Regla de oro:** `sim` no incluye una sola línea de OpenGL. Eso permite (1) testear la física sin abrir ventana, (2) que alguien trabaje el solver sin tocar el render, y (3) meter mañana un solver en GPU sin reescribir nada de arriba.

### Flujo de datos por frame
```
input  →  solver.step(dt)  →  renderer.draw(positions, colors)  →  swap
          (sim produce             (render consume
           posiciones)              posiciones)
```
La costura es `IRenderer::drawParticles(span<const Vec2> pos, span<const Vec3> color, float radius)`. Hoy la implementa `GlRenderer`; el día de Vulkan, un `VulkanRenderer` implementa la misma interfaz y `app` ni se entera.

## Estructura de archivos

```
PhyEngine/
├── CMakeLists.txt          # build cross-platform (reemplaza el Makefile)
├── README.md               # qué es + GIF + cómo compilar
├── LICENSE                 # MIT
├── CONTRIBUTING.md
├── ROADMAP.md              # seguimiento de milestones
├── .clang-format
├── .github/workflows/ci.yml
├── docs/                   # ← el lado didáctico (estos archivos)
│   ├── 00-overview.md
│   ├── 01-sph-theory.md
│   └── references.md
├── Bibliography/           # PDFs (ya existe)
├── extern/                 # deps (glm, glad) vía FetchContent o vendored
├── assets/shaders/
│   ├── particle.vert
│   └── particle.frag
├── src/
│   ├── core/
│   │   ├── Math.hpp         # using Vec2 = glm::vec2; using Scalar = float;
│   │   └── Constants.hpp    # PI, GRAVITY…
│   ├── sim/                 # ← física pura, sin OpenGL
│   │   ├── Particle.hpp
│   │   ├── ParticleSystem.hpp/.cpp
│   │   ├── Kernels.hpp/.cpp
│   │   ├── NeighborSearch.hpp        # interfaz
│   │   ├── BruteForceSearch.hpp/.cpp # O(n²) ahora
│   │   ├── UniformGrid.hpp/.cpp      # milestone 4
│   │   ├── SphSolver.hpp/.cpp
│   │   └── Domain.hpp
│   ├── render/
│   │   ├── IRenderer.hpp    # ← interfaz; Vulkan la implementará después
│   │   ├── GlRenderer.hpp/.cpp
│   │   ├── Shader.hpp/.cpp
│   │   └── ParticleRenderer.hpp/.cpp
│   └── app/
│       ├── Window.hpp/.cpp
│       ├── Input.hpp/.cpp
│       ├── Camera2D.hpp
│       ├── SphDemo.hpp/.cpp
│       └── main.cpp
└── tests/
    ├── test_kernels.cpp
    └── test_sph.cpp
```

### Tu código actual NO se tira — se reubica

| Hoy | Mañana |
|-----|--------|
| `main.cpp` (window loop) | `src/app/main.cpp` + `Window` + `SphDemo` |
| `utils.cpp/.hpp` | `src/app/Input` + `Window` |
| `SPH/particles.*` | `src/sim/Particle.hpp` + `ParticleSystem` |
| `SPH/sph.*` (kernels + solver) | `src/sim/Kernels` + `SphSolver` (corrigiendo bugs) |
| `VertexShader/`, `FragmentShader/` | `assets/shaders/particle.vert/.frag` |

## Repo colaborativo
- **LICENSE: MIT** — permisiva y estándar en proyectos educativos. Sin licencia, legalmente nadie puede contribuir con seguridad.
- **README** con GIF/screenshot + instrucciones de build + cómo correr + roadmap. El GIF es lo que engancha.
- **CONTRIBUTING.md** — cómo compilar/testear, estilo (`.clang-format`), proceso de PR.
- **CI (GitHub Actions)** — compila + corre tests en cada PR ⇒ `main` siempre verde.
- **Issues `good first issue`** — la arquitectura por capas las hace autocontenidas (ej. `UniformGrid`, un kernel nuevo, interacción con mouse).

## Estrategia didáctica
- Cada módulo se escribe como una **"lección"**: comentarios que explican el *por qué* y **citan la ecuación** del paper.
- [`01-sph-theory.md`](01-sph-theory.md) deriva kernels y fuerzas y **liga código ↔ ecuación**.
- Los **tests son documentación ejecutable** (`test_kernels` muestra el comportamiento esperado).
- La capa `sim` pura permite estudiar la física sin ruido gráfico.
- Cada milestone puede ser un **tag/release o un devlog**.
