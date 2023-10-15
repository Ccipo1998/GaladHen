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

#include <utils/dblog.h>

class Shader
{
    public:
    
    GLuint Program;

    // Constructors

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
    {
        // Step 1: we retrieve shaders source code from provided filepaths
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // Open file
            vShaderFile.open(vertexShaderPath);
            std::stringstream vShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::string description = (std::string)vertexShaderPath + " not succesfully read";
            dblog::error("Shader", e.what());
        }

        try
        {
            // Open file
            fShaderFile.open(fragmentShaderPath);
            std::stringstream fShaderStream;
            // Read file's buffer contents into streams
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            fShaderFile.close();
            // Convert stream into string
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::string description = (std::string)fragmentShaderPath + " not succesfully read";
            dblog::error("Shader", description);
        }

        // Convert strings to char pointers
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Step 2: we compile the shaders
        GLuint vertex, fragment;

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // check compilation errors
        checkShaderCompilation(vertex, "Vertex");

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // check compilation errors
        checkShaderCompilation(fragment, "Fragment");

        // Step 3: Shader Program creation
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        // check linking errors
        checkShaderLinking(this->Program);

        // Step 4: we delete the shaders because they are linked to the Shader Program, and we do not need them anymore
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Constructors ending

    // Shader Program activation as part of the current rendering process
    void use() { glUseProgram(this->Program); }

    // Shader Program delete
    void _delete_() { glDeleteProgram(this->Program); }

    private:

    // Check shader compilation errors
    void checkShaderCompilation(GLuint shader, const char* shaderType)
    {
        GLint success;
		GLchar infoLog[1024];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::string type = shaderType;
            std::string description = " Shader compilation failed";
            dblog::error("Shader", (char*)infoLog);
        }
    }

    void checkShaderLinking(GLuint program)
    {
        GLint success;
		GLchar infoLog[1024];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            dblog::error("Shader", (char*)infoLog);
        }
    }
};
