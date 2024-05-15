
#include "glad/glad.h"


#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "manage.h"
#include "shader.h"
#include "Objeto.h"
#include "simulation.h"
#include <omp.h>
#include <iostream>

#include "include/imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"





using namespace std;








int main(){


    const int SCR_WIDTH = 1200;
    const int SCR_HEIGHT = 1200;


    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    std::vector<Particle*> list_of_particles;
    SpatialHashing grid(0.8, 0.8,0.8);

     // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

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
    // IMGUI_CHECKVERSION();
    // ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO(); (void)io;

    // // Setup Platform/Renderer backends
    // ImGui::StyleColorsDark();

    // ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    // ImGui_ImplOpenGL3_Init("#version 330 core");
    // //ImGui SIZE
    // ImGui::GetStyle().ScaleAllSizes(2.0f);
    

    Shader ourShader("VertexShaders/lighting.vs","FragmentShaders/lighting.fs");
    ourShader.use();



    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
    ourShader.setMat4("projection",projection);
    
    ourShader.setVec3("lightColor",glm::vec3(1.0f,1.0f,1.0f));
    srand(time(NULL));
    
    
    int n = 3000;
    initialization_simulation(list_of_particles, grid, n);
    
    
    //Esfera obj(glm::vec3(0,0,0),.05f,100,100);
    //obj.setup();
    //glm::vec3 move=  glm::vec3(1.0f,0.0f,0.0f);
    
    cout<<list_of_particles.size()<<endl;

        
    
    // game loop
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
        processInput(window,camera, 0.001f);
        
        

        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setVec3("lightPos", camera.Position);
        ourShader.setMat4("view",view);
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model",model);
        
        /*obj.centro += move*0.01f; 
        if (obj.centro.x > 10.0f) {
            obj.centro.x = 10.0f;
            move *= -1.0f;
        }
        if (obj.centro.x < -10.0f) {
            obj.centro.x = -10.0f;
            move *= -1.0f;
        }
        obj.display(ourShader);
        std::cout<<camera.Position.x<<" "<<camera.Position.y<<" "<<camera.Position.z<<std::endl;*/
        simulating(list_of_particles,grid,0.0001f,ourShader);





        glfwSwapBuffers(window);


        
    }
    // Cleanup   
    finish_simulation(list_of_particles);
    glfwTerminate();



    
    return 0;

        
       



}



    











