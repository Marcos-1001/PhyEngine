#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP


#include "IRenderer.hpp"

// OpenGL renderer definitions here
class GlRenderer : public IRenderer
{
public:
    ~GlRenderer() override = default;
    GlRenderer(Shader shader);
    void render() override;
    void clear(const glm::vec3& color) override;
    void drawParticles(const std::vector<glm::vec2>& positions, 
                       const std::vector<glm::vec3>& colors, 
                       float radiusPx, 
                       const glm::mat4& viewProj, 
                       unsigned int VBO_POS,
                       unsigned int VBO_COLOR,
                        unsigned int VAO
                       ) override;
};


#endif // GLRENDERER_HPP