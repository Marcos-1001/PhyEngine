# Referencias

Curadas y ordenadas por **cuándo las necesitas**. No las leas todas de golpe.

## OpenGL — úsalas YA (Milestones 0–1)
- **LearnOpenGL** — <https://learnopengl.com> (Joey de Vries). El tutorial moderno de referencia, gratis. La ruta principal.
- **The Cherno — OpenGL series** (YouTube). Construye clases `Shader`/`VertexBuffer` igual que las de la capa `render`.
- **docs.gl** — <https://docs.gl>. Referencia rápida de cada función GL.

## SPH — el corazón (Milestone 3)
- **Müller, Charypar & Gross (2003)** — *Particle-Based Fluid Simulation for Interactive Applications* (SCA). El paper canónico y didáctico; **tus kernels salen de aquí**. ~8 páginas, léelo completo.
- **Sebastian Lague — *Coding Adventure: Simulating Fluids*** (YouTube). SPH en 2D, visual, calcado a este objetivo. Empieza por aquí.
- **Matthias Müller — *Ten Minute Physics*** — <https://matthias-research.github.io/pages/> + YouTube. Didáctico, con fluidos.
- **Koschier, Bender, Solenthaler & Teschner (2022)** — *SPH Techniques for the Physics-Based Simulation of Fluids and Solids* (STAR). Ya lo tienes en `Bibliography/`. Para profundizar.
- **SPlisHSPlasH** — <https://github.com/InteractiveComputerGraphics/SPlisHSPlasH>. Implementación de referencia (Bender et al.).

## Math / fundamentos
- **GLM manual** — <https://github.com/g-truc/glm>.
- **3Blue1Brown — Essence of Linear Algebra** (YouTube). Intuición geométrica.
- *Opcional:* Eric Lengyel — *Foundations of Game Engine Development, Vol. 1: Mathematics*.

## Vulkan — para el Milestone "GPU" (NO ahora) 🔖
> Bookmark. No tocar hasta el milestone de cómputo en GPU. Vulkan hoy solo frenaría el objetivo SPH.
- **vulkan-tutorial.com** — <https://vulkan-tutorial.com>. El tutorial estándar de entrada.
- **Vulkan Guide (vkguide.dev)** — <https://vkguide.dev> (Victor Blanco). Más moderno y orientado a motor.
- **Sascha Willems — Vulkan examples** — <https://github.com/SaschaWillems/Vulkan>. Incluye **ejemplos de compute**, justo lo que usarás para correr SPH en GPU.
- **Khronos Vulkan Samples** — <https://github.com/KhronosGroup/Vulkan-Samples> + la spec.
- *Después:* zeux — *Writing an efficient Vulkan renderer*.

## Arquitectura de motores (referencia, no para leer entero)
- **Jason Gregory — *Game Engine Architecture***. El libro de referencia.
- Código de **Box2D** (<https://github.com/erincatto/box2d>) y **Bullet** para ver cómo se estructura un engine de física real.
