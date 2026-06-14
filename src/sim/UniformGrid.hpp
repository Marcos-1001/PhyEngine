#ifndef UNIFORMGRID_HPP
#define UNIFORMGRID_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <set>
#include "core/Math.hpp"

#define HASH_P1 73856093 // primos grandes de Teschner et al. 2003
#define HASH_P2 19349663

// Neighbor search por SPATIAL HASHING
// (Teschner et al. 2003, "Optimized Spatial Hashing for Collision Detection of
//  Deformable Objects").
//
// El plano se divide en celdas virtuales de lado cellSize (= h). Una celda (cx, cy)
// se proyecta a un bucket de una tabla hash de tamaño fijo:
//
//     hash(cx, cy) = ( cx*HASH_P1  XOR  cy*HASH_P2 )  mod  tableSize
//
// No hay rejilla material acotada: las coordenadas de celda pueden ser negativas o
// enormes (dominio NO acotado), y el módulo final mantiene el índice dentro de la
// tabla. Los candidatos de un punto son las partículas de los 9 buckets de su
// vecindad 3×3. El filtro exacto r ≤ h lo aplica quien consulta (los kernels SPH
// valen 0 para r > h), lo que de paso descarta los falsos positivos por colisión.
class UniformGrid
{
public:
    UniformGrid();

    // Fija el lado de celda y dimensiona la tabla hash (redondea `buckets` al primo
    // siguiente para repartir mejor). Llamar antes de build().
    void configure(float cellSize, std::size_t buckets);

    // Reconstruye la tabla con las posiciones actuales (vacía buckets + re-inserta).
    void build(const std::vector<Vec2>& positions);

    // Vuelca en `out` (limpiado, sin duplicados) los índices candidatos de la
    // vecindad 3×3 alrededor de p, incluida su propia celda (y su propia partícula).
    void queryNeighbors(const Vec2& p, const float& radius, std::vector<std::size_t>& out) const;

private:
    int         cellCoord(float x) const;       // floor(x / cellSize); admite negativos
    std::size_t hashCell(int cx, int cy) const; // Teschner, con módulo seguro

    float       m_cellSize;
    std::size_t m_tableSize;
    std::vector<std::vector<std::size_t>> m_table; // tabla hash: bucket → índices
};

#endif // UNIFORMGRID_HPP
