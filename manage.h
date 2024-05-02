#ifndef MANAGE_H
#define MANAGE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include <iostream>
#include <string>


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Camera &camera, float deltaTime);



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
void checkCompileErrors(unsigned int shader, std::string type);

#endif