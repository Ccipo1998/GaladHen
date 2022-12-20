/*
Shaders creation and management for the rendering pipeline
*/

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
    public:
    
    GLuint Program;

    // Constructors

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
    {
        // Step 1: we retrieve shaders source code from provided filepaths
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        try
        {
            // Open file
            vShaderFile.open(vertexShaderPath);
            stringstream vShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
        }
        catch (ifstream::failure e)
        {
            string path = vertexShaderPath;
            string description = " not succesfully read";
            printError(path + description, e.what());
        }

        try
        {
            // Open file
            fShaderFile.open(fragmentShaderPath);
            stringstream fShaderStream;
            // Read file's buffer contents into streams
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            fShaderFile.close();
            // Convert stream into string
            fragmentCode = fShaderStream.str();
        }
        catch (ifstream::failure e)
        {
            string path = fragmentShaderPath;
            string description = " not succesfully read";
            printError(path + description, e.what());
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

    void printError(const char* description, const char* log)
    {
        cout << "\nError: " << description << "\n" << log << "\n\n--------------------" << endl;
    }

    void printError(string description, const char* log)
    {
        cout << "\nError: " << description << "\n" << log << "\n\n--------------------" << endl;
    }

    // Check shader compilation errors
    void checkShaderCompilation(GLuint shader, const char* shaderType)
    {
        GLint success;
		GLchar infoLog[1024];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            string type = shaderType;
            string description = " Shader compilation failed";
            printError(type + description, infoLog);
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
            printError("Shader linking failed", infoLog);
        }
    }
};
