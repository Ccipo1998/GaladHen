
#include "pbrmaterial.h"
#include "shader.h"
#include <gtc/type_ptr.hpp> // for value_ptr()
#include <utils/log.h>
#include "texture.h"

// defaults
#define DEFAULT_DIFFUSE_COLOR glm::vec3(0.3f, 0.9f, 0.3f)
#define DEFAULT_METALLIC 0.0f
#define DEFAULT_ROUGHNESS 0.0f

GLuint PBRMaterial::SubroutineIndices[] = {};

PBRMaterial::PBRMaterial(Shader* pbrShader)
    : DiffuseColor(DEFAULT_DIFFUSE_COLOR)
    , DiffuseTexture(Texture{ nullptr })
    , NormalMap(Texture{ nullptr })
    , Metallic(DEFAULT_METALLIC)
    , MetallicTexture(Texture{ nullptr })
    , Roughness(DEFAULT_ROUGHNESS)
    , RoughnessTexture(Texture{ nullptr })
{
    MaterialShader = pbrShader;   
}

void PBRMaterial::SendDataToShader() const
{
    if (this->MaterialShader == nullptr)
    {
        Log::Error("pbrmaterial.h", "Impossible to send data to shader: null material shader");

        return;
    }

    GLuint diffuseSubroutine = 2;
    GLuint normalSubroutine = 4;
    GLuint metallicSubroutine = 6;
    GLuint roughnessSubroutine = 8;

    // send material data to shader
    glProgramUniform3fv(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "DiffuseColor"), 1, value_ptr(this->DiffuseColor));
    //glProgramUniform1f(shader.Program, glGetUniformLocation(shader.Program, "Specular"), this->Specular);
    glProgramUniform1f(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Metallic"), this->Metallic);
    glProgramUniform1f(this->MaterialShader->GetShaderProgram(), glGetUniformLocation(this->MaterialShader->GetShaderProgram(), "Roughness"), this->Roughness);

    if (this->DiffuseTexture.GetTextureImage() != nullptr)
    {
        this->DiffuseTexture.SetActiveTexture(GL_TEXTURE0);
        ++diffuseSubroutine;
    }
    
    if (this->NormalMap.GetTextureImage() != nullptr)
    {
        this->NormalMap.SetActiveTexture(GL_TEXTURE1);
        ++normalSubroutine;
    }
    
    if (this->MetallicTexture.GetTextureImage() != nullptr)
    {
        this->MetallicTexture.SetActiveTexture(GL_TEXTURE2);
        ++metallicSubroutine;
    }

    if (this->RoughnessTexture.GetTextureImage() != nullptr)
    {
        this->RoughnessTexture.SetActiveTexture(GL_TEXTURE3);
        ++roughnessSubroutine;
    }

    // subroutine selection
    this->SubroutineIndices[0] = (GLuint)this->MaterialShadingMode;
    this->SubroutineIndices[1] = diffuseSubroutine;
    this->SubroutineIndices[2] = normalSubroutine;
    this->SubroutineIndices[3] = metallicSubroutine;
    this->SubroutineIndices[4] = roughnessSubroutine;
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 5, PBRMaterial::SubroutineIndices);
}
