
#include "phongmaterial.h"
#include "shader.h"
#include <gtc/type_ptr.hpp> // for value_ptr()
#include <utils/log.h>

#define DEFAULT_DIFFUSE_COLOR glm::vec3(0.3f, 0.9f, 0.3f)
#define DEFAULT_KA glm::vec3(.1f, .1f, .1f)
#define DEFAULT_KD glm::vec3(.6f, .6f, .6f)
#define DEFAULT_KS glm::vec3(.8f, .8f, .8f)

PhongMaterial::PhongMaterial()
    : DiffuseColor(DEFAULT_DIFFUSE_COLOR)
    , Ka(DEFAULT_KA)
    , Kd(DEFAULT_KD)
    , Ks(DEFAULT_KS)
    {}

void PhongMaterial::SendDataToShader() const
{
    if (this->MaterialShader == nullptr)
    {
        Log::Error("phongmaterial.h", "Impossible to send data to shader: null material shader");

        return;
    }

    // send material data to shader
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Diffuse"), 1, value_ptr(this->DiffuseColor));
    //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), this->Specular);
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Ka"), 1, value_ptr(this->Ka));
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Kd"), 1, value_ptr(this->Kd));
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Ks"), 1, value_ptr(this->Ks));
}

