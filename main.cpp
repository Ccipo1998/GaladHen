#include <iostream>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "include/utils/shader.h"
#include "include/utils/model.h"

#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp" // for lookat() and perspective()
#include "include/glm/gtc/type_ptr.hpp" // for value_ptr()

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

using namespace std;
using namespace glm;

GLFWwindow* initContext(int width, int height, const char* name)
{
    if (!glfwInit())
        throw runtime_error("Error: GLFW failed to initialize the context");

    // setting the minimum required version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // core profile is a subset of OpenGL features (without the backward-compatible features)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for MacOS:
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!gl3wInit())
        throw runtime_error("Error: GL3W failed to initialize the context");

    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (window == nullptr)
        throw runtime_error("Error: GLFW failed to create the window");

    return window;
}

int main()
{
    GLFWwindow* window = initContext(600, 400, "EzEngine");

    glfwMakeContextCurrent(window);

    /*
    vector<Vertex> vertices;
    Vertex v0 = Vertex { vec3(1.0f, .0f, .0f), vec3(.0f, .0f, .0f), vec2(.0f, .0f), vec3(.0f, .0f, .0f), vec3(.0f, .0f, .0f) };
    Vertex v1 = Vertex { vec3(-1.0f, .0f, .0f), vec3(.0f, .0f, .0f), vec2(.0f, .0f), vec3(.0f, .0f, .0f), vec3(.0f, .0f, .0f) };
    Vertex v2 = Vertex { vec3(.0f, 1.0f, .0f), vec3(.0f, .0f, .0f), vec2(.0f, .0f), vec3(.0f, .0f, .0f), vec3(.0f, .0f, .0f) };
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);

    vector<GLuint> indices = { 0, 1, 2 };
    
    Mesh cube = Mesh(vertices, indices);
    */

    Model cube = Model("prefabs/cube.obj");
    /*
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);

    GLuint Buffer;
    glCreateBuffers(1, &Buffer);
    glNamedBufferStorage(Buffer, sizeof(vertices), vertices, 0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    */

    Shader testShader = Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
    
    mat4 viewMatrix = lookAt(vec3(.0f, .0f, -1.0f), vec3(.0f, .0f, 3.0f), vec3(.0f, 1.0f, .0f));
    mat4 projectionMatrix = perspective(radians(170.0f), 600.0f/400.0f, .1f, 100.0f);
    
    glProgramUniformMatrix4fv(testShader.Program, glGetUniformLocation(testShader.Program, "projectionMatrix"), 1, GL_FALSE, value_ptr(projectionMatrix));
    glProgramUniformMatrix4fv(testShader.Program, glGetUniformLocation(testShader.Program, "viewMatrix"), 1, GL_FALSE, value_ptr(viewMatrix));

    testShader.use();
    
    while (!glfwWindowShouldClose(window))
    {
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static const float black[] = { .0f, .0f, .0f, .0f };
        glClearBufferfv(GL_COLOR, 0, black);

        /*
        glBindVertexArray(cube.VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, cube.Vertices.size());
        */
        cube.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

