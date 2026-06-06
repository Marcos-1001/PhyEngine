// This file's objective is to test the free fall of a particle under gravity using the ParticleSystem class.
// It will create a particle with an initial position and velocity, and then update its state over time to observe the effects of gravity.


#include "sim/ParticleSystem.hpp"
#include <iostream>



int main(){
    // Create a particle with an initial position and velocity
    Particle2D particle(Vec2(0.0f, 100.0f), Vec2(10.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), 1.0f);
    std::vector<Particle2D> particles = { particle };
    ParticleSystem2D particleSystem(particles);

    // Simulate the particle's motion under gravity for a certain number of steps
    const float timeStep = 0.1f; // Time step for the simulation
    const int numSteps = 4; // Number of simulation steps

    for (int i = 0; i < numSteps; ++i) {
        particleSystem.step_nodomain(timeStep);
        const auto& updatedParticle = particleSystem.getParticles()[0];
        std::cout << "Time: " << (i + 1) * timeStep << "s, Position: (" 
                  << updatedParticle.position.x << ", " 
                  << updatedParticle.position.y << "), Velocity: (" 
                  << updatedParticle.velocity.x << ", " 
                  << updatedParticle.velocity.y << ")\n";
    }

    // analytical solve
    std::cout << "\nAnalytical Solution:\n";
    float analyticalSolution = 0.5f * GRAVITY * numSteps * numSteps;
    std::cout << "Expected Position after " << numSteps * timeStep << "s: (" 
              << particle.position.x + particle.velocity.x * numSteps * timeStep << ", " 
              << particle.position.y + particle.velocity.y * numSteps * timeStep - analyticalSolution << ")\n"
              << "Error in Y position: " << std::abs((particle.position.y + particle.velocity.y * numSteps * timeStep - analyticalSolution) - particleSystem.getParticles()[0].position.y) << "\n";




}