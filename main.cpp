#include <iostream>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "include/utils/shader.h"
#include "include/utils/model.h"
#include "include/utils/camera.h"

#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp" // for lookat() and perspective()
#include "include/glm/gtc/type_ptr.hpp" // for value_ptr()

#define BUFFER_OFFSET(offset) (void*)(offset) // MACRO

// we initialize an array of booleans for each keybord key
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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
    glfwSetKeyCallback(window, key_callback);

    glClearColor(.0f, .0f, .0f, .0f);
    // we enable Z test
    glEnable(GL_DEPTH_TEST);

    Model cube = Model("prefabs/bunny.obj");

    Shader testShader = Shader("shaders/stylized/Stylized.vert", "shaders/stylized/Stylized.frag");
    
    Camera camera = Camera(vec3(.0f, .0f, 10.0f), 90.0f, .0f, .0f, radians(45.0f), 600.0f/400.0f, .1f, 100.0f);

    //mat4 viewMatrix = lookAt(vec3(.0f, .0f, -1.0f), vec3(.0f, .0f, 3.0f), vec3(.0f, 1.0f, .0f));
    //mat4 projectionMatrix = perspective(radians(170.0f), 600.0f/400.0f, .1f, 100.0f);
    
    vec3 lightPos = vec3(10.0f, 10.0f, 10.0f);

    testShader.use();
    
    GLfloat lastTime = .0f;
    GLfloat deltaTime = .0f;
    while (!glfwWindowShouldClose(window))
    {
        // we "clear" the frame and z buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // time counter
        GLfloat currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // camera movements
        camera.applyMovements(keys, deltaTime);

        // send camera data to shaders
        glProgramUniformMatrix4fv(testShader.Program, glGetUniformLocation(testShader.Program, "projectionMatrix"), 1, GL_FALSE, value_ptr(camera.getProjectionMatrix()));
        glProgramUniformMatrix4fv(testShader.Program, glGetUniformLocation(testShader.Program, "viewMatrix"), 1, GL_FALSE, value_ptr(camera.getViewMatrix()));
        glProgramUniform3fv(testShader.Program, glGetUniformLocation(testShader.Program, "lightPos"), 1, value_ptr(lightPos));

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // we keep trace of the pressed keys
    // with this method, we can manage 2 keys pressed at the same time:
    // many I/O managers often consider only 1 key pressed at the time (the first pressed, until it is released)
    // using a boolean array, we can then check and manage all the keys pressed at the same time
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

