
// Shader is the class representing a stage of the Graphics Pipeline (Vertex, Tesselation, Geometry, Fragment, Compute)

#pragma once

#include <string>

namespace GaladHen
{
    enum ShaderStage
    {
        None = 0,
        Vertex = 1,
        Fragment = 2
    };

    class Shader
    {
    public:

        // Constructors

        Shader();

        // @brief
        // Construction of a shader object with a shader file and a shader type, no compilation call
        Shader(const char* shaderFilePath, ShaderStage shaderType);

        // Copy ctor
        Shader(const Shader& shader) noexcept;
        // Copy assignment
        Shader& operator=(const Shader& shader) noexcept;
        // Move ctor
        Shader(Shader&& shader) noexcept;
        // Move assignment
        Shader& operator=(Shader&& shader) noexcept;

        std::string ShaderFilePath;
        ShaderStage ShaderType;

    protected:

        unsigned int ShaderID; // 0 means -1, so the low level pair is not already been created
    };
}
