/*
Shaders creation and management for the rendering pipeline
*/

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/log.h>

class Shader
{
    public:

    // Constructors

    // We want Shader to be a move-only class. We delete copy constructor and copy assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/copy_constructor
    // https://en.cppreference.com/w/cpp/language/copy_assignment
    // https://www.geeksforgeeks.org/preventing-object-copy-in-cpp-3-different-ways/
    // Section 4.6 of the "A Tour in C++" book
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    // For the Shader class, a default move constructor and move assignment is sufficient
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/move_constructor
    // https://en.cppreference.com/w/cpp/language/move_assignment
    // https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
    // https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
    // Section 4.6 of the "A Tour in C++" book
    Shader(Shader&& shader) = default;
    Shader& operator=(Shader&& shader) = default;

    // No default constructor -> at least a shader file is needed
    Shader() = delete;

    // @brief
    // Create shader object with file's content as source code for the shader type rendering stage
    // @param shaderPath: path of the shader file
    // @param shaderType: OpenGL shader type (ex: GL_VERTEX_SHADER)
    Shader(const char* shaderPath, GLenum shaderType);

    // @brief
    // Create shader object with file's content as source code for both vertex shader and fragment shader
    // @param vertexShaderPath: path of the vertex shader file
    // @param fragmentShaderPath: path of the fragment shader file
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    // getters

    unsigned int GetShaderProgram();

    // Shader program activation as part of the current rendering process
    void Use();

    // Shader program delete
    void Delete();

protected:

    // Check for shader compilation errors
    void CheckShaderCompilation(GLuint shader, const char* shaderPath);

    // Check for shader linking errors
    void CheckShaderLinking(GLuint program);

    GLuint Program;
};
