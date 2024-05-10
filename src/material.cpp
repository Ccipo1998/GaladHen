
#include <ezengine/material.h>
#include <ezengine/shader.h>

Material::Material()
    : MaterialShaderClass(ShaderClass::None)
    , MaterialShader(nullptr)
    , MaterialShadingMode(ShadingMode::SmoothShading)
    {}

Shader* Material::GetShader()
{
    return this->MaterialShader;
}

void Material::SendDataToShader() const {}

void Material::UseShader() const
{
    if (this->MaterialShader == nullptr)
        return;
        
    this->MaterialShader->Use();
}
