
#include "glad/glad.h"


#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "particles.h"
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

const int SCR_WIDTH = 1200;
const int SCR_HEIGHT = 1200;


float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));







int main(){

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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330 core");
    //ImGui SIZE
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    

    Shader ourShader("VertexShaders/lighting.vs","FragmentShaders/lighting.fs");
    ourShader.use();



    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
    ourShader.setMat4("projection",projection);
    
    ourShader.setVec3("lightColor",glm::vec3(1.0f,1.0f,1.0f));
    ourShader.setVec3("lightPos",glm::vec3(5,0.0f,5));    
    srand(time(NULL));
    
    
    int n = 100;
    init_simulation(n);


    
    cout<<list_of_particles.size()<<endl;

        
    
    // game loop
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
        processInput(window,camera, 0.001f);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Edit variables");
        ImGui::SliderFloat("Radius",&radius,0.0f,1.5f);
        ImGui::SliderFloat("rest_density",&rest_density,1000.0f,5000.0f);
        ImGui::SliderFloat("K",&k_,0.0f,1.5f);
        ImGui::SliderFloat("vstrength",&vstrength,0.0f,1.0f);
        
        ImGui::SliderFloat("Gravity",&gravity_constant,-10.0f,10.0f);
        ImGui::SliderFloat("min_x",&min_x,-10.0f,10.0f);
        ImGui::SliderFloat("max_x",&max_x,-10.0f,10.0f);
        ImGui::SliderFloat("min_y",&min_y,-10.0f,10.0f);
        ImGui::SliderFloat("max_y",&max_y,-10.0f,10.0f);
        ImGui::SliderFloat("min_z",&min_z,-10.0f,10.0f);
        ImGui::SliderFloat("max_z",&max_z,-10.0f,10.0f);

        //Calculate bounds volume
        //float volume = (max_x-min_x)*(max_y-min_y)*(max_z-min_z);

        //rest_density = 1000.f; 

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,ImGui::GetIO().Framerate);


        ImGui::Button("Add a particle");
        if(ImGui::IsItemClicked()){
            shared_ptr<Particle> p = make_shared<Particle>(glm::vec3((min_x+max_x)/2.,max_y,(min_z+max_z)/2.) , 
                                                                glm::vec3(0,0,0),
                                                                glm::vec3(0,0,0));
            list_of_particles.push_back(p);
            insert(p);
        }
        ImGui::Button("Add 10 Particle");
        if(ImGui::IsItemClicked()){
            for(int i = 0 ; i< 10; i++){
                shared_ptr<Particle> p = make_shared<Particle>(glm::vec3((min_x+max_x)/2.,max_y,(min_z+max_z)/2.) , 
                                                                glm::vec3(0,0,0),
                                                                glm::vec3(0,0,0));
                list_of_particles.push_back(p);
                insert(p);
            }
            
        }

        ImGui::End();
        


        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = camera.GetViewMatrix();

        ourShader.setVec3("lightPos",glm::vec3(-5,0.0f,-5));
        ourShader.setMat4("view",view);
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model",model);


        simulating(0.0001f,ourShader);
        
        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




        glfwSwapBuffers(window);


        
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

        
    glfwTerminate();



    
    return 0;

        
       



}



    











