
#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "manage.h"
#include "shader.h"
#include <omp.h>
#include <iostream>

#include "include/imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"
#include "perlin_noise.h"
using namespace std;
const int SCR_WIDTH = 1200;
const int SCR_HEIGHT = 1200;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));

#define PI 3.1415926535897932384626433832795f

const int X = 150;
const int Y = 150;

vector<glm::vec3> PLANE(int init_X, int init_Y, vector<glm::vec3> &normals, vector<int> &indices, bool Brownian = false)
{

    const double scale = 0.06;

    double terrain[X + 1][Y + 1];

    float amplitude;
    float frequency;
    float result;
    for (int i = init_X; i <= (init_X + X); i++)
    {
        for (int j = init_Y; j <= (init_Y + Y); j++)
        {
            if (Brownian)
            {
                amplitude = 0.5f;
                frequency = 0.045f;
                result = 0.;
                for (int k = 0; k < 8; k++)
                {
                    result += amplitude * (perlin(i * frequency, j * frequency) * 5);
                    amplitude *= 0.5f;
                    frequency *= 2.0f;
                }
                terrain[i][j] = result;
            }
            else
            {
                terrain[i][j] = perlin(i * 0.045, j * 0.045) * 5;
            }
        }
    }

    vector<glm::vec3> points;

    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
        {

            points.push_back(glm::vec3(i * scale, terrain[i][j], j * scale));
            points.push_back(glm::vec3(i * scale, terrain[i][j + 1], (j + 1) * scale));
            points.push_back(glm::vec3((i + 1) * scale, terrain[i + 1][j + 1], (j + 1) * scale));

            points.push_back(glm::vec3(i * scale, terrain[i][j], j * scale));
            points.push_back(glm::vec3((i + 1) * scale, terrain[i + 1][j], j * scale));
            points.push_back(glm::vec3((i + 1) * scale, terrain[i + 1][j + 1], (j + 1) * scale));

            normals.push_back(glm::normalize(glm::vec3(i * scale, terrain[i][j], j * scale)));
            normals.push_back(glm::normalize(glm::vec3(i * scale, terrain[i][j + 1], (j + 1) * scale)));
            normals.push_back(glm::normalize(glm::vec3((i + 1) * scale, terrain[i + 1][j + 1], (j + 1) * scale)));

            normals.push_back(glm::normalize(glm::vec3(i * scale, terrain[i][j], j * scale)));
            normals.push_back(glm::normalize(glm::vec3((i + 1) * scale, terrain[i + 1][j], j * scale)));
            normals.push_back(glm::normalize(glm::vec3((i + 1) * scale, terrain[i + 1][j + 1], (j + 1) * scale)));
        }
    }

    return points;
}

int main()
{
    srand48(time(NULL));
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Platform/Renderer backends
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330 core");
    // ImGui SIZE
    ImGui::GetStyle().ScaleAllSizes(2.0f);

    Shader ourShader("VertexShaders/lighting.vs", "FragmentShaders/lighting.fs");
    ourShader.use();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    srand(time(NULL));

    vector<glm::vec3> normals;
    vector<glm::vec3> colors;
    vector<int> indices;
    GLuint VAO;

    vector<glm::vec3> points = PLANE(0, 0, normals, indices);
    for (int i = 0; i < normals.size(); i++)
    {
        colors.push_back(glm::vec3(0.640f, .430f, 0.750f) * points[i].y * 255.f);
    }
    GLuint VBO[3];
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(3, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    auto render_way = GL_LINES;
    bool Brownian = true;

    // game loop
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        processInput(window, camera, 0.001f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowSize(ImVec2(20, 20));
        ImGui::Begin("Menu");
        ImGui::Text("Choose render way:");
        ImGui::Button("Lines");
        if (ImGui::IsItemClicked())
        {
            render_way = GL_LINES;
        }
        ImGui::Button("Triangles");
        if (ImGui::IsItemClicked())
        {
            render_way = GL_TRIANGLES;
        }
        ImGui::Button("Brownian");
        if (ImGui::IsItemClicked())
        {
            Brownian = !Brownian;
            normals.clear();
            indices.clear();
            points.clear();

            points = PLANE(0, 0, normals, indices, Brownian);
            for (int i = 0; i < normals.size(); i++)
            {
                colors.push_back(glm::vec3(0.640f, .430f, 0.420f) * points[i].y * 255.f);
            }

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        ImGui::End();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();

        // ourShader.setVec3("lightPos",camera.Position);
        ourShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);

        // ourShader.setVec3("lightPos",glm::vec3(X/2.,2.0f,Y/2.));
        ourShader.setVec3("lightPos", camera.Position);

        glBindVertexArray(VAO);
        glDrawArrays(render_way, 0, points.size());
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        glFlush();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}
