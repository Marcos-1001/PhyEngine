#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "glm/glm.hpp"

#include "Shader.hpp"
#include <vector>


class IRenderer { 
protected:
    Shader shader; // Shader común para todos los renderers, por simplicidad. Se puede mejorar esto luego.
public: 
    virtual ~IRenderer() = default;
    IRenderer() : shader(Shader("assets/shaders/particle.vert", "assets/shaders/particle.frag")) {}; 
    IRenderer(Shader shader) : shader(shader) {}; //Carga un shader por defecto
    virtual void render() = 0;
    virtual void clear(const glm::vec3& color) = 0;
    virtual void drawParticles( const std::vector<glm::vec2>& positions,
                                const std::vector<glm::vec3>& colors,
                                float radiusPx,
                                const glm::mat4& viewProj,
                                unsigned int VBO_POS,
                                unsigned int VBO_COLOR,
                                unsigned int VAO
                            ) = 0;
};

#endif // IRENDERER_HPP 