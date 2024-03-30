
#include <ezengine/phongmaterial.h>
#include <ezengine/phongshader.h>
#include <gtc/type_ptr.hpp> // for value_ptr()

PhongMaterial::PhongMaterial()
    : DiffuseColor(glm::vec3(0.3f, 0.9f, 0.3f))
    , Ka(glm::vec3(.1f, .1f, .1f))
    , Kd(glm::vec3(.6f, .6f, .6f))
    , Ks(glm::vec3(.8f, .8f, .8f))
    , MaterialShader(nullptr)
    {}

PhongShader* PhongMaterial::GetShader()
{
    return this->MaterialShader;
}

void PhongMaterial::SendDataToShader()
{
    if (this->MaterialShader == nullptr)
        return;

    // send material data to shader
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Diffuse"), 1, value_ptr(this->DiffuseColor));
    //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), this->Specular);
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Ka"), 1, value_ptr(this->Ka));
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Kd"), 1, value_ptr(this->Kd));
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Ks"), 1, value_ptr(this->Ks));
}

