
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

    // @brief
    // Default constructor
    Material();

    // Default copy and move contructors and assignments
    Material(const Material& material) = default;
    Material& operator=(const Material& material) = default;
    Material(Material&& material) = default;
    Material& operator=(Material&& material) = default;

    // @brief
    // Set shader instance
    void SetShader(Shader* shader);

    // @brief
    // Get shader instance
    Shader* GetShader();

    // @brief
    // To send material data to its shader
    virtual void SendDataToShader();

protected:

    ShaderClass MaterialShaderClass;
    Shader* MaterialShader;

};
