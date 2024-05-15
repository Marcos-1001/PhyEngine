//
// Created by hgallegos on 5/10/2022.
//

#ifndef OBJETO_H
#define OBJETO_H
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include <vector>
#include "shader.h"
using namespace std;

class Esfera
{
public:
    glm::vec3 centro;
    float radius;
    int slices, stacks;
    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    GLuint vao;
    glm::mat4 model;
    bool visible = true;
    bool mueve = true;
    float escala = 1;
    GLint POSITION_ATTRIBUTE = 0, NORMAL_ATTRIBUTE = 1, TEXCOORD0_ATTRIBUTE = 8;
    Esfera()
    {
        escala = 1.;
        centro = glm::vec3(0.0);
    }
    Esfera(glm::vec3 _centro)
    {
        escala = 1.;
        centro = _centro;
    }
    Esfera(glm::vec3 _centro, float _radius, int _slices, int _stacks)
    {
        escala = .5;
        centro = _centro;
        radius = _radius;
        slices = _slices;
        stacks = _stacks;
    }
    GLuint setup()
    {
        const float pi = 3.1415926535897932384626433832795f;
        const float _2pi = 2.0f * pi;
        vector<glm::vec3> positions;
        vector<glm::vec3> normals;
        vector<glm::vec2> textureCoords;
        for (int i = 0; i <= stacks; ++i)
        {
            // V texture coordinate.
            float V = i / (float)stacks;
            float phi = V * pi;
            for (int j = 0; j <= slices; ++j)
            {
                // U texture coordinate.
                float U = j / (float)slices;
                float theta = U * _2pi;
                float X = cos(theta) * sin(phi);
                float Y = cos(phi);
                float Z = sin(theta) * sin(phi);
                positions.push_back(glm::vec3(X, Y, Z) * radius);
                normals.push_back(glm::vec3(X, Y, Z));
                textureCoords.push_back(glm::vec2(U, V));
            }
        }
        // Now generate the index buffer
        // vector<GLuint> indicies;
        for (int i = 0; i < slices * stacks + slices; ++i)
        {
            indices.push_back(i);
            indices.push_back(i + slices + 1);
            indices.push_back(i + slices);
            indices.push_back(i + slices + 1);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        // GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbos[3];
        glGenBuffers(3, vbos);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(POSITION_ATTRIBUTE);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, (void *)0);
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE);
        /*
        glBindBuffer( GL_ARRAY_BUFFER, vbos[2] );
        glBufferData( GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), textureCoords.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        glEnableVertexAttribArray( TEXCOORD0_ATTRIBUTE );
        */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        indices_size = indices.size();
        return vao;
    }

    void display(Shader &sh)
    {
        
        model = glm::mat4(1.0);
        model = translate(model, centro);
        model = scale(model, glm::vec3(escala));

        sh.setMat4("model", model);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    ~Esfera() = default;
};

#endif // OBJETO_H
