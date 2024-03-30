
#include <ezengine/pbrmaterial.h>
#include <ezengine/pbrshader.h>
#include <gtc/type_ptr.hpp> // for value_ptr()

PBRMaterial::PBRMaterial()
    : DiffuseColor(glm::vec3(0.3f, 0.9f, 0.3f))
    , Metallic(0.0f)
    , Roughness(1.0f) // defaults
    , MaterialShader(nullptr)
    {}

void PBRMaterial::SetShader(PBRShader* pbrShader)
{
    this->MaterialShader = pbrShader;
}

PBRShader* PBRMaterial::GetShader()
{
    return this->MaterialShader;
}

void PBRMaterial::SendDataToShader()
{
    if (this->MaterialShader == nullptr)
        return;

    // send material data to shader
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Diffuse"), 1, value_ptr(this->DiffuseColor));
    //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), this->Specular);
    glProgramUniform1f(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Metallic"), this->Metallic);
    glProgramUniform1f(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Roughness"), this->Roughness);
}
