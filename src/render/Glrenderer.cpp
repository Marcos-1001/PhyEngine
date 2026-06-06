#include "GlRenderer.hpp"

GlRenderer::GlRenderer(Shader shader) : IRenderer(shader) {}

void GlRenderer::render() {
    // En este caso, no necesitamos hacer nada aquí porque el renderizado se hace directamente en drawParticles.
    // Sin embargo, podríamos usar este método para configurar estados globales de OpenGL si fuera necesario.
}

void GlRenderer::clear(const glm::vec3& color) {
    
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlRenderer::drawParticles(const std::vector<glm::vec2>& positions,
                                const std::vector<glm::vec3>& colors,
                                float radiusPx,
                                const glm::mat4& viewProj,
                                unsigned int vboPos,
                                unsigned int vboCol,
                                unsigned int VAO
                                ){ 
    
                                    
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size()*sizeof(glm::vec2), positions.data());
    glBindBuffer(GL_ARRAY_BUFFER, vboCol);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size()*sizeof(glm::vec3), colors.data());

    this->shader.use();
    this->shader.setMat4("uViewProj", viewProj);   
    this->shader.setFloat("uRadius", radiusPx);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, (GLsizei)positions.size());

}