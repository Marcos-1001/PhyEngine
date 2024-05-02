
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

using namespace std; 
const int SCR_WIDTH = 1200;
const int SCR_HEIGHT = 1200;


float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

#define PI 3.1415926535897932384626433832795f


vector<glm::vec3> points_generation(int sample=100){
    // equation x = y cos(4y)
    vector<glm::vec3> points(sample);

    for(double i = 0 ; i <= 2.*PI; i += 2.*PI/sample){
        points.push_back(glm::vec3(i*cos(4*i), i ,0));
    }
    return points;
}

vector<glm::vec3> points_generation_3d(int sample=100){
    vector<glm::vec3> points;
    const double gridSize = 2.*PI  ;
    const double skip = 2.*PI/sample;
    
    for(double t= 0;  t <= 2.*PI; t +=skip){
        for(double phi = 0 ; phi <= 2.*PI; phi += skip){
            // float x0 = static_cast<float>(t) / (gridSize );
            // float y0 = static_cast<float>(phi) / (gridSize );
            // float x1 = static_cast<float>(t + skip) / (gridSize );
            // float y1 = static_cast<float>(phi + skip) / (gridSize );
            
            // glm::vec3 p0 = glm::vec3(x0*cos(y0), x0*sin(y0), x0);
            // glm::vec3 p1 = glm::vec3(x0*cos(y1), x0*sin(y1), x0);
            // glm::vec3 p2 = glm::vec3(x1*cos(y0), x1*sin(y0), x1);
            // glm::vec3 p3 = glm::vec3(x1*cos(y1), x1*sin(y1), x1);

            // points.push_back(p0);
            // points.push_back(p1);
            // points.push_back(p2);
            
            // points.push_back(p1);
            // points.push_back(p2);
            // points.push_back(p3);
            points.push_back(glm::vec3((2+cos(t))*cos(phi),(2+cos(t))*sin(phi),t));
        }
    }

    
    return points; 
}




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

    

    Shader ourShader("VertexShaders/lighting.vs","FragmentShaders/lighting.fs");
    ourShader.use();



    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);
    ourShader.setMat4("projection",projection);
    
    ourShader.setVec3("lightColor",glm::vec3(1.0f,1.0f,1.0f));
    ourShader.setVec3("lightPos",glm::vec3(5,0.0f,5));    
    srand(time(NULL));
    
    
    vector<glm::vec3> points = points_generation(100);
    // send points to GPU
    unsigned int VBO,VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,points.size()*sizeof(glm::vec3),&points[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);



    

        
    
    // game loop
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
        processInput(window,camera, 0.001f);
        

        //Calculate bounds volume
        //float volume = (max_x-min_x)*(max_y-min_y)*(max_z-min_z);

        //rest_density = 1000.f; 

        


        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = camera.GetViewMatrix();

        ourShader.setVec3("lightPos",glm::vec3(-5,0.0f,-5));
        ourShader.setMat4("view",view);
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model",model);


        
        


        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP,0,points.size());
        glBindVertexArray(0);




        glfwSwapBuffers(window);


        
    }
    

        
    glfwTerminate();



    
    return 0;

        
       



}
