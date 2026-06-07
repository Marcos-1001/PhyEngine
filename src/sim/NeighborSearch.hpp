#ifndef NEIGHBORSEARCH_HPP
#define NEIGHBORSEARCH_HPP


#include "ParticleSystem.hpp"


class NeighborSearch
{
public:
    NeighborSearch();
    ~NeighborSearch();
    void build(const ParticleSystem2D& system);
    std::vector<std::vector<size_t>> findNeighbors(const ParticleSystem2D& system) const;    
}

#endif // NEIGHBORSEARCH_HPP