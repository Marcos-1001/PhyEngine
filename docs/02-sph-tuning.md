# SPH — Parámetros estables (zona segura)

> Set de parámetros **verificado** para que la simulación SPH 2D no explote.
> Verificado con un arnés *headless* que corre el solver bajo compresión fuerte
> (bloques de 2.5–4× la densidad de reposo) y bajo el escenario real con gravedad,
> midiendo que la velocidad máxima quede acotada (sin `NaN`, sin runaway).

## Baseline seguro (el que tienes hoy)

| Parámetro            | Valor   | Dónde            |
|----------------------|---------|------------------|
| `restDensity`        | 1.0     | `SphSolver` ctor |
| `gasConstant`        | 500     | `SphSolver` ctor |
| `viscosity`          | 0.8     | `SphSolver` ctor |
| `smoothingLength` h  | 16      | `SphSolver` ctor |
| `spacing` d          | 8       | `main.cpp`       |
| `mass`               | 64      | `main.cpp`       |
| `dt`                 | 0.0004  | `main.cpp`       |
| `substeps`           | 4       | `main.cpp`       |

## Zona segura para jugar (medida)

Con el resto fijo, cada knob se mantuvo estable en este rango:

| Knob          | Rango probado estable | Efecto al subirlo            |
|---------------|-----------------------|------------------------------|
| `gasConstant` | 100 – 2000            | fluido más incompresible/"duro" |
| `dt`          | 0.0004 – 0.008        | más rápido, pero menos estable  |
| `viscosity`   | 0.0 – 1.0+            | más amortiguado/"viscoso"       |

Tu `dt=0.0004` tiene ~20× de margen antes del límite que llegué a probar. Hay aire de sobra.

## Las 4 reglas que SIEMPRE deben cumplirse (invariantes)

Esto NO es tuning, son requisitos: romperlos sí rompe la sim.

1. **`spacing < smoothingLength`** — si no, no hay vecinos → no hay fluido (cada partícula aislada).
2. **`mass = restDensity · spacing²`** — mantiene la densidad en reposo ≈ `restDensity`. Si cambias `spacing`, recalcula `mass`.
3. **Resetear `density = 0` cada paso** — sin esto se acumula entre frames → explota. (Ya está en `computeDensityPressure`.)
4. **`dt` pequeño + substeps** — SPH es rígido. Si subes mucho `gasConstant` o el nº de partículas, baja `dt` o sube `substeps` (condición tipo CFL).

## Si algún día SÍ explota — orden de diagnóstico

1. ¿Subiste `gasConstant` mucho? → baja `dt` en proporción.
2. ¿Muchas más partículas / pila más profunda? → baja `dt` o sube `substeps`.
3. ¿Bajaste `viscosity` a ~0 con `dt` grande? → sube `viscosity` (es tu amortiguador).
4. ¿Cambiaste `spacing` sin recalcular `mass`? → aplica la regla 2.
