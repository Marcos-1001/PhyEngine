// M4 — Regresión del neighbor search.
// El UniformGrid acotado debe devolver, tras filtrar por el radio de suavizado h,
// EXACTAMENTE el mismo conjunto de vecinos que la fuerza bruta O(n²). Con celda = h
// y vecindad 3×3, los candidatos del grid son un superconjunto de {j : r ≤ h}; el
// filtro por distancia debe dejar ambos conjuntos idénticos.

#include "sim/UniformGrid.hpp"
#include "core/Math.hpp"

#include <vector>
#include <set>
#include <iostream>

static std::set<size_t> bruteForce(const std::vector<Vec2>& pos, size_t i, float h)
{
    std::set<size_t> s;
    for (size_t j = 0; j < pos.size(); ++j)
        if (glm::distance(pos[i], pos[j]) <= h) s.insert(j);
    return s;
}

static std::set<size_t> viaGrid(const UniformGrid& grid, const std::vector<Vec2>& pos, size_t i, float h)
{
    std::vector<size_t> candidates;
    grid.queryNeighbors(pos[i], h, candidates);
    std::set<size_t> s;
    for (size_t j : candidates)
        if (glm::distance(pos[i], pos[j]) <= h) s.insert(j);
    return s;
}

int main()
{
    const float h = 16.0f;

    // Bloque regular pegado al borde inferior-izquierdo del dominio: las partículas
    // de la esquina ejercitan el clamp del stencil (celdas vecinas fuera de rango).
    std::vector<Vec2> pos;
    for (int gx = 0; gx < 12; ++gx)
        for (int gy = 0; gy < 12; ++gy)
            pos.push_back(Vec2(4.0f + gx * 8.0f, 4.0f + gy * 8.0f));

    UniformGrid grid;
    grid.configure(h, pos.size() * 2);
    grid.build(pos);

    int failures = 0;
    for (size_t i = 0; i < pos.size(); ++i)
    {
        const std::set<size_t> expected = bruteForce(pos, i, h);
        const std::set<size_t> actual   = viaGrid(grid, pos, i, h);
        if (expected != actual)
        {
            if (failures < 5)
                std::cerr << "Mismatch en partícula " << i
                          << ": fuerza bruta=" << expected.size()
                          << "  grid=" << actual.size() << "\n";
            ++failures;
        }
    }

    if (failures)
    {
        std::cerr << "FAIL: " << failures << " partículas con vecinos discrepantes\n";
        return 1;
    }
    std::cout << "OK: grid == fuerza bruta para " << pos.size() << " partículas\n";
    return 0;
}
