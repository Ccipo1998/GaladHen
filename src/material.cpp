
#include <ezengine/material.h>
#include <ezengine/shader.h>

Material::Material()
    : MaterialShaderClass(ShaderClass::None)
    , MaterialShader(nullptr)
    {}

void Material::SetShader(Shader* shader)
{
    this->MaterialShader = shader;
}

Shader* Material::GetShader()
{
    return this->MaterialShader;
}

void Material::SendDataToShader() {}
