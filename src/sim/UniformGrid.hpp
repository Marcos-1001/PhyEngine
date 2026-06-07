#ifndef UNIFORMGRID_HPP
#define UNIFORMGRID_HPP

#include "ParticleSystem.hpp"
#define HASH_X 73856093
#define HASH_Y 19349663

class UniformGrid
{
public:
    UniformGrid(int gridSize, float cellSize);
    ~UniformGrid();
    void build(const ParticleSystem2D& system);
    std::vector<std::vector<size_t>> findNeighbors(const ParticleSystem2D& system, int radius) const;

private:
    void clear();
    void insertParticle(size_t particleIndex, const ParticleSystem2D& system);
    std::vector<size_t> getNeighbors(size_t particleIndex, const ParticleSystem2D& system, int radius) const;
    int gridSize;
    int cellSize;
    // Grid data structure
    std::vector<std::vector<size_t>> m_grid;
};

#endif // UNIFORMGRID_HPP