
#include <ezengine/material.h>
#include <ezengine/shader.h>

Material::Material()
    : MaterialShaderClass(ShaderClass::None)
    , MaterialShader(nullptr)
    {}

Shader* Material::GetShader()
{
    return this->MaterialShader;
}

void Material::SendDataToShader() {}

void Material::UseShader()
{
    if (this->MaterialShader == nullptr)
        return;
        
    this->MaterialShader->Use();
}
