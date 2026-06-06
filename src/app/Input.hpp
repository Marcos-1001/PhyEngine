#ifndef PHYENGINE_APP_INPUT_HPP
#define PHYENGINE_APP_INPUT_HPP

#include <GLFW/glfw3.h>

// Entrada de teclado y callbacks de ventana.
// (Movido desde el antiguo utils.*; sepáralo en Window/Input cuando crezca.)
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif  // PHYENGINE_APP_INPUT_HPP
