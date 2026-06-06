cmake_minimum_required(VERSION 3.16)
project(PhyEngine LANGUAGES C CXX)   # C necesario para compilar glad.c

# --- Estándar de C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- Dependencias del sistema ---
# Si alguna falta, CMake aborta aquí con un mensaje claro: eso ES "verificar
# que las librerías están instaladas y se encuentran".
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# GLM: usa el del sistema si está; si no (p.ej. un colaborador que no lo tiene),
# lo descarga solo. Es header-only, así que sale barato.
find_package(glm QUIET)
if(NOT glm_FOUND)
  message(STATUS "glm no está en el sistema; se descargará con FetchContent")
  include(FetchContent)
  FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1)
  FetchContent_MakeAvailable(glm)
endif()

# --- GLAD (loader de OpenGL, vendored en extern/glad) ---
# Sustituye a GLEW: así un colaborador no necesita instalar nada extra para
# cargar las funciones de OpenGL. Para regenerarlo (otra versión de GL), ver
# glad.txt.
add_library(glad STATIC "${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/extern/glad/include")

# --- Capa sim (headless: solo depende de glm, CERO OpenGL) ---
# Es una librería propia para que los tests la enlacen sin arrastrar
# GLFW/GLAD/OpenGL. Ese aislamiento es justo el objetivo de la arquitectura por
# capas: la física se compila y se testea sin abrir una ventana.
# GLOB con CONFIGURE_DEPENDS re-detecta archivos nuevos al compilar.
file(GLOB_RECURSE SIM_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/sim/*.cpp")
add_library(phyengine_sim STATIC ${SIM_SOURCES})
# PUBLIC: quien enlace la lib hereda este include y glm (p.ej. #include "sim/ParticleSystem.hpp").
target_include_directories(phyengine_sim PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(phyengine_sim PUBLIC glm::glm)

# --- Ejecutable principal (app + render): la capa que SÍ usa OpenGL ---
file(GLOB_RECURSE PHYENGINE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/app/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/render/*.cpp")

add_executable(PhyEngine ${PHYENGINE_SOURCES})
target_include_directories(PhyEngine PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(PhyEngine PRIVATE
  phyengine_sim
  OpenGL::GL
  glad
  glfw
  glm::glm
  ${CMAKE_DL_LIBS})

# Ruta absoluta a /assets accesible desde el código (para cargar shaders sin
# depender del directorio de trabajo desde donde ejecutes).
target_compile_definitions(PhyEngine PRIVATE
  PHYENGINE_ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")

# --- Tests (headless: enlazan solo la capa sim, sin OpenGL) ---
# Correr con:  cmake --build build --target test_freefall && ctest --test-dir build
enable_testing()
add_executable(test_freefall "${CMAKE_SOURCE_DIR}/tests/test_freefall.cpp")
target_link_libraries(test_freefall PRIVATE phyengine_sim)
add_test(NAME freefall COMMAND test_freefall)
