#include "render/Shader.hpp"
#include "render/GlRenderer.hpp"
#include "sim/ParticleSystem.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include "Input.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Error: glfwInit() falló\n";
        return -1;
    }

    // Contexto OpenGL 3.3 core: lo mínimo "moderno".
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "PhyEngine", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Error: no se pudo crear la ventana GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD carga los punteros a las funciones de OpenGL, usando GLFW como fuente.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error: gladLoadGLLoader() falló\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION)
              << "  |  GPU: " << glGetString(GL_RENDERER) << '\n';


    int numParticles = 10;
    std::vector<Particle2D> particles(numParticles*numParticles);
    for(int i = 0; i < numParticles; ++i) {
        for(int j = 0; j < numParticles; ++j) {
            particles[i*numParticles + j] = Particle2D(Vec2(0.0f + i*40.0f, 20.0f + j*40.0f), 
            Vec2(0.0f, 0.0f), 
            Vec3(0.0f, 0.0f, 1.0f), 1.0f);
        }
    }

    ParticleSystem2D particleSystem(particles);
    
    
    Shader  mshader("assets/shaders/particle.vert", "assets/shaders/particle.frag");
    

    
    unsigned int VBO_POS, VBO_COLOR, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_POS);
    glGenBuffers(1, &VBO_COLOR);


    glBindVertexArray(VAO);



    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO_POS);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,  sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (void*) 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_PROGRAM_POINT_SIZE); // Permite setear el tamaño de los puntos desde el shader
    glBindVertexArray(0); // Unbind VAO
    

    IRenderer* renderer = new GlRenderer(mshader);



    std::vector<glm::vec2> positions(particles.size());
    std::vector<glm::vec3> colors(particles.size());


    glm::mat4 view = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    const float colorChangeSpeed = 0.001f;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderer->clear(glm::vec3(0.1f, 0.1f, 0.1f));
        particleSystem.step(0.016f); // Simula un paso de 16 ms (aprox. 60 FPS)
        particles = particleSystem.getParticles();
        
        for (size_t i = 0; i < particles.size(); ++i)
        {
            positions[i] = particles[i].position;
            colors[i] = particles[i].color;
        }
        renderer->drawParticles(positions, 
                                colors, 
                                30.0f, 
                                view, 
                                VBO_POS, 
                                VBO_COLOR,
                                VAO
                            );

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_POS);
    glDeleteBuffers(1, &VBO_COLOR);

    glfwTerminate();
    return 0;
}
