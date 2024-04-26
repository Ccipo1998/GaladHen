
// Shader interface

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class Shader
{
public:

    // Constructors

    // Shader as move-only class. We delete copy constructor and copy assignment
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    // No need for move constructor and move assignment
    Shader(Shader&& shader) = delete;
    Shader& operator=(Shader&& shader) = delete;

    // Default shader constructor
    Shader();

    void LoadVertexFragmentShaders(const char* vertexShaderPath, const char* fragmentShaderPath);

    // getters

    // Get program id
    GLuint GetShaderProgram() const;

    // Shader program activation as part of the current rendering process
    void Use() const;

    // Shader program delete
    void Delete();

protected:

    // Check for shader compilation errors
    void CheckShaderCompilation(GLuint shader, const char* shaderPath);

    // Check for shader linking errors
    void CheckShaderLinking(GLuint program);

    GLuint Program;
};
