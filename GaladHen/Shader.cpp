
#include "Shader.h"

namespace GaladHen
{
    Shader::Shader()
        : ShaderType(ShaderStage::None)
        , ShaderID(0)
        {}

    Shader::Shader(std::string& shaderFilePath, ShaderStage shaderType)
        : ShaderFilePath(shaderFilePath)
        , ShaderType(shaderType)
        , ShaderID(0)
        {}

    Shader::Shader(const Shader& other) noexcept
        : ShaderFilePath(other.ShaderFilePath)
        , ShaderType(other.ShaderType)
        , ShaderID(0)
        {}

    Shader& Shader::operator=(const Shader& other) noexcept
    {
        ShaderFilePath = other.ShaderFilePath;
        ShaderType = other.ShaderType;
        ShaderID = 0;

        return *this;
    }

    Shader::Shader(Shader&& other) noexcept
        : ShaderFilePath(std::move(other.ShaderFilePath))
        , ShaderType(std::move(other.ShaderType))
        , ShaderID(other.ShaderID)
    {
        other.ShaderID = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept
    {
        ShaderID = other.ShaderID;
        other.ShaderID = 0;
        ShaderFilePath = std::move(other.ShaderFilePath);
        ShaderType = std::move(other.ShaderType);

        return *this;
    }
}
