#ifndef MANAGE_H
#define MANAGE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include <iostream>
#include <string>


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Camera &camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessMouseMovement(0, 4);
    if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessMouseMovement(0, -4);
    if (glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessMouseMovement(-4, 0);
    if (glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)    
        camera.ProcessMouseMovement(4, 0);
    if (glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessMouseScroll(20);
    if (glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessMouseScroll(-20);
    




    
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

#endif