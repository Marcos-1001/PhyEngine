#include "UniformGrid.hpp"

#include <algorithm>
#include <cmath>

namespace
{
bool isPrime(std::size_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (std::size_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

std::size_t nextPrime(std::size_t n)
{
    std::size_t c = (n <= 2) ? 2 : (n % 2 == 0 ? n + 1 : n);
    while (!isPrime(c)) c += 2;
    return c;
}
} // namespace

UniformGrid::UniformGrid()
    : m_cellSize(1.0f), m_tableSize(1), m_table(1)
{
}

void UniformGrid::configure(float cellSize, std::size_t buckets)
{
    m_cellSize = cellSize;
    m_tableSize = nextPrime(std::max<std::size_t>(2, buckets));
    m_table.assign(m_tableSize, std::vector<std::size_t>());
}

int UniformGrid::cellCoord(float x) const
{
    // floor (no truncación) para que las coordenadas negativas caigan en la celda
    return static_cast<int>(std::floor(x / m_cellSize));
}

std::size_t UniformGrid::hashCell(int cx, int cy) const
{
    const std::int64_t h = (static_cast<std::int64_t>(cx) * HASH_P1)
                         ^ (static_cast<std::int64_t>(cy) * HASH_P2);
    std::int64_t m = h % static_cast<std::int64_t>(m_tableSize);
    if (m < 0) m += static_cast<std::int64_t>(m_tableSize); // módulo seguro para hash negativo
    return static_cast<std::size_t>(m);
}

void UniformGrid::build(const std::vector<Vec2>& positions)
{
    for (auto& bucket : m_table)
        bucket.clear();

    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        const int cx = cellCoord(positions[i].x);
        const int cy = cellCoord(positions[i].y);
        m_table[hashCell(cx, cy)].push_back(i);
    }
}

void UniformGrid::queryNeighbors(const Vec2& p, const float& radius, std::vector<std::size_t>& out) const
{
    out.clear();
    const int cx = cellCoord(p.x);
    const int cy = cellCoord(p.y);
    std::set<std::size_t> buckets;

    const int range = static_cast<int>(std::ceil(radius / m_cellSize));
    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dy = -range; dy <= range; ++dy)
        {
            std::size_t bucket = hashCell(cx + dx, cy + dy);
            buckets.insert(bucket);
        }
    }
    
    for (std::size_t bucket : buckets)
    {
        out.insert(out.end(), m_table[bucket].begin(), m_table[bucket].end());
    }
}
