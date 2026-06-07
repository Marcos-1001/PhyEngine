#include "UniformGrid.hpp"
UniformGrid::UniformGrid(int gridSize, float cellSize)
{
    this->gridSize = gridSize;
    this->cellSize = cellSize;
    m_grid.resize(gridSize, std::vector<size_t>());
}

UniformGrid::~UniformGrid()
{
}

void UniformGrid::build(const ParticleSystem2D& system)
{
    clear();
    for (size_t i = 0; i < system.size(); ++i)
    {
        insertParticle(i, system);
    }
}

std::vector<std::vector<size_t>> UniformGrid::findNeighbors(const ParticleSystem2D& system, int radius) const
{
    std::vector<std::vector<size_t>> neighbors(system.size());
    for (size_t i = 0; i < system.size(); ++i)
    {
        neighbors[i] = getNeighbors(i, system, radius);
    }
    return neighbors;
}

void UniformGrid::clear()
{
    for (auto& cell : m_grid)
    {
        cell.clear();
    }
}

void UniformGrid::insertParticle(size_t particleIndex, const ParticleSystem2D& system)
{
    const auto& particle = system[particleIndex];
    int cellX = static_cast<int>(particle.position.x / cellSize) % gridSize;
    int cellY = static_cast<int>(particle.position.y / cellSize) % gridSize;
    int hash = (cellX * HASH_X) ^ (cellY * HASH_Y);
    m_grid[hash].push_back(particleIndex);
}

std::vector<size_t> UniformGrid::getNeighbors(size_t particleIndex, const ParticleSystem2D& system, int radius) const
{
    Particle2D particle = system[particleIndex];
    int cellX = static_cast<int>(particle.position.x / cellSize) % gridSize;
    int cellY = static_cast<int>(particle.position.y / cellSize) % gridSize;

    std::vector<size_t> neighbors;
    for (int offsetX = -radius; offsetX <= radius; ++offsetX)
    {
        for (int offsetY = -radius; offsetY <= radius; ++offsetY)
        {
            int neighborCellX = (cellX + offsetX + gridSize) % gridSize;
            int neighborCellY = (cellY + offsetY + gridSize) % gridSize;
            int hash = (neighborCellX * HASH_X) ^ (neighborCellY * HASH_Y);
            for (size_t neighborIndex : m_grid[hash])
            {
                if (neighborIndex != particleIndex)
                {
                    neighbors.push_back(neighborIndex);
                }
            }
        }
    }
    return neighbors;
 
}
 