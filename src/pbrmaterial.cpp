
#include <ezengine/pbrmaterial.h>
#include <ezengine/shader.h>
#include <gtc/type_ptr.hpp> // for value_ptr()
#include <utils/log.h>

// defaults
#define DEFAULT_DIFFUSE_COLOR glm::vec3(0.3f, 0.9f, 0.3f)
#define DEFAULT_METALLIC 0.0f
#define DEFAULT_ROUGHNESS 0.0f

PBRMaterial::PBRMaterial()
    : DiffuseColor(DEFAULT_DIFFUSE_COLOR)
    , Metallic(DEFAULT_METALLIC)
    , Roughness(DEFAULT_ROUGHNESS)
    {}

void PBRMaterial::SendDataToShader()
{
    if (this->MaterialShader == nullptr)
    {
        Log::Error("pbrmaterial.h", "Impossible to send data to shader: null material shader");

        return;
    }

    // send material data to shader
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Diffuse"), 1, value_ptr(this->DiffuseColor));
    //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), this->Specular);
    glProgramUniform1f(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Metallic"), this->Metallic);
    glProgramUniform1f(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Roughness"), this->Roughness);
}
