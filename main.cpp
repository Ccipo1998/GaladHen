#include <iostream>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "include/utils/shader.h"

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

using namespace std;

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

    static const GLfloat vertices[6][2] =
    {
        { -1.0, 0.0},
        { 0.0, 1.0},
        { 0.0, 0.0},
        { 0.0, 0.0},
        { 0.0, 1.0},
        { 1.0, 0.0}
    };

    GLuint VAO;
    glCreateVertexArrays(1, &VAO);

    GLuint Buffer;
    glCreateBuffers(1, &Buffer);
    glNamedBufferStorage(Buffer, sizeof(vertices), vertices, 0);

    Shader testShader = Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
    testShader.use();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        static const float black[] = { .0f, .0f, .0f, .0f };
        glClearBufferfv(GL_COLOR, 0, black);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

