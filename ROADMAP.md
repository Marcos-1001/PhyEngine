# PhyEngine — Roadmap & Seguimiento

> Motor de física en C++ con un simulador de fluidos **SPH 2D** como primer módulo.
> Diseño: [`docs/00-overview.md`](docs/00-overview.md) · Teoría SPH: [`docs/01-sph-theory.md`](docs/01-sph-theory.md) · Referencias: [`docs/references.md`](docs/references.md)

Marca `[x]` conforme avances. **Cada milestone entrega algo visible en pantalla o un test que pasa** — nunca escribes a ciegas.

## Decisiones (fijadas)
- **Render:** OpenGL ahora (GLFW + GLEW), Vulkan después detrás de `IRenderer`.
- **Dimensión:** 2D primero, 3D después.
- **Cómputo:** CPU primero; GPU/compute en milestone futuro.
- **Arquitectura:** por capas `core → sim → render → app` (`sim` es headless, sin OpenGL).
- **Build:** CMake · **Tests:** doctest · **Math:** GLM.

---

## Estado actual (punto de partida)
Lo que existe hoy y lo que está roto. Se arregla en M0 (render) y M3 (física):

- [x] El render nunca dibuja: no compila/usa el shader, no llama `glDrawArrays`. *(Resuelto.)*
- [x] `FragmentShader/BaseShader.fs` está vacío (0 bytes). *(Resuelto en `particle.frag`.)*
- [x] `VertexShader/BaseShader.vs` tiene un `out vec4 FragColor` que va en el fragment shader. *(Resuelto en `particle.vert`.)*
- [x] El `Makefile` solo compila `*.cpp` de la raíz → `SPH/*.cpp` ni se compila. *(Resuelto: CMake compila `src/sim/`.)*
- [ ] `particles.hpp`: include guard con typo (`PARTICINLES_HPP`).
- [ ] SPH: la densidad no se reinicia entre frames (se acumula → todo mal).
- [ ] SPH: falta la auto-contribución de densidad (término `j = i`).
- [ ] SPH: `pressure_compute` y `viscosity_compute` están vacías.
- [ ] SPH: `kernel_viscosity_laplacian` devuelve `vec3`; debe ser **escalar**.
- [ ] SPH: no hay integración, ni gravedad, ni fuerza de presión.

---

## 🪜 Milestone 0 — Cimientos: build + un triángulo de verdad
- [x] `CMakeLists.txt` cross-platform (reemplaza el Makefile Linux-only).
- [x] Esqueleto de carpetas: `src/{core,sim,render,app}`, `assets/shaders`, `tests`.
- [x] Mover tu código al nuevo árbol (`app/`, `sim/`, shaders a `assets/shaders/`).
- [x] Build verificado: configura, compila, enlaza y corre → OpenGL 3.3 NVIDIA (smoke test en `src/app/main.cpp`).
- [x] Arreglar el shader: el `out vec4 FragColor` va en `particle.frag` (no en el `.vert`); `particle.frag` está vacío.
- [x] `Shader` (compila/linkea/usa el programa) + VAO/VBO + `glDrawArrays`.
- [x] **✅ Resultado: ventana con el triángulo** — ¡y RGB interpolado, mejor que naranja!
- 🎓 Aprendes: el pipeline OpenGL (VAO/VBO/shader program).

## 🪜 Milestone 1 — Puntos en pantalla: renderer de partículas
- [x] `IRenderer` (interfaz) + `GlRenderer` (impl OpenGL).
- [x] `ParticleRenderer`: dibuja N puntos desde `std::vector<Vec2>`. *(vía `GlRenderer::drawParticles` + VAO/VBO + `GL_POINTS`.)*
- [x] `Camera2D` ortográfica (coordenadas de mundo → pantalla). *(ortho inline en `main`; extraer a clase cuando haga falta pan/zoom.)*
- [x] **✅ Resultado: una grilla de cientos de puntos.** *(pipeline verificado a escala.)*
- 🎓 Aprendes: point/instanced rendering, proyección ortográfica, uniforms.

## 🪜 Milestone 2 — Partículas que caen (todavía SIN SPH)
- [x] `Particle2D` + `ParticleSystem2D` (`std::vector<Particle2D>` por valor, sin punteros).
- [x] Gravedad + integración semi-implícita de Euler.
- [x] `Domain2D`: caja con rebote amortiguado.
- [ ] **Test:** caída libre vs solución analítica `y = ½gt²`. *(⏳ DIFERIDO a propósito: el test compila y corre vía CTest, pero todavía no assertea ni corrige el factor `dt²`. Cerrar idealmente antes de M3 — es la red de regresión para el `step()`.)*
- [x] **✅ Resultado: partículas que caen y rebotan en la caja.**
- 🎓 Aprendes: integración numérica, manejo de fronteras, loop con `dt` fijo.

## 🪜 Milestone 3 — 💧 SPH de verdad (TU OBJETIVO)
- [ ] `Kernels`: poly6, spiky∇, visc∇² (escalar) — con tests.
- [ ] `SphSolver`: **densidad → presión → fuerzas → integrar**.
- [ ] Reset de densidad cada paso + auto-contribución.
- [ ] Color por densidad o velocidad (ver el campo).
- [ ] **Test:** densidad de un bloque en reposo ≈ `rest_density`; fuerzas simétricas (3ª ley de Newton).
- [ ] **✅ Resultado: agua 2D que se mueve, salpica y se asienta.**
- 🎓 Aprendes: el corazón de SPH (Müller 2003). Ver [`docs/01-sph-theory.md`](docs/01-sph-theory.md).

## 🪜 Milestone 4 — Que escale: neighbor search con grid
- [ ] `NeighborSearch` (interfaz) + `UniformGrid` (spatial hashing).
- [ ] **✅ Resultado: decenas de miles de partículas a 60 FPS.**
- 🎓 Aprendes: spatial hashing, perfilado, optimización. 🎯 *Ideal como "good first issue".*

## 🪜 Milestone 5 — Pulido + interacción
- [ ] Mouse para empujar / echar agua.
- [ ] Parámetros en vivo (ImGui, opcional).
- [ ] GIF para el README.
- [ ] **✅ Resultado: demo compartible que atrae colaboradores.**

## 🔮 Futuro (ya como "motor", no solo demo)
- [ ] 3D + cámara orbital (reusa casi todo: `Vec2` → `Vec3`).
- [ ] GPU compute — **aquí entra Vulkan** (o compute shaders de OpenGL).
- [ ] Nuevos módulos: rigid bodies, colisiones, cloth.
- [ ] Superficie del fluido (marching squares / screen-space fluid).

---

## 🤝 Infra del repo colaborativo
- [ ] `LICENSE` (MIT).
- [ ] `README.md` (qué es + GIF + build con CMake + cómo correr + link al roadmap).
- [ ] `CONTRIBUTING.md` (cómo compilar/testear, estilo, proceso de PR).
- [ ] `.clang-format` (estilo consistente para PRs).
- [ ] `.github/workflows/ci.yml` (compila + corre tests en cada PR).
- [ ] Issues etiquetados `good first issue` (ej. `UniformGrid`, un kernel nuevo, interacción con mouse).
