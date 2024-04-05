
// Interface for Material classes to store shading parameters

#pragma once

class Shader;

enum class ShaderClass
{
    None,
    Phong,
    PhysicallyBased
};

class Material
{
public:

    // Default copy and move contructors and assignments
    Material(const Material& material) = default;
    Material& operator=(const Material& material) = default;
    Material(Material&& material) = default;
    Material& operator=(Material&& material) = default;

    // @brief
    // Get shader instance
    Shader* GetShader();

    // @brief
    // To send material data to its shader
    virtual void SendDataToShader();

    // @brief
    // Set material shader for use
    void UseShader();

protected:

    // @brief
    // Default constructor
    Material();

    ShaderClass MaterialShaderClass;
    Shader* MaterialShader;

};
