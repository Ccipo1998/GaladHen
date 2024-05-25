
// Material class for PBR shading

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>

#include "material.h"

class Texture;

class PBRMaterial : public Material
{
    
public:

    // PBR parameters
    glm::vec3 DiffuseColor;
    Texture DiffuseTexture;
    Texture NormalMap;
    float Metallic;
    Texture MetallicTexture;
    float Roughness;
    Texture RoughnessTexture;
    //GLfloat Specular;

    // ...
    
    // @brief
    // Create material
    PBRMaterial(Shader* pbrShader);

    // @brief
    // It sends material data to its shader
    virtual void SendDataToShader() const override;

protected:

    // shared array for pbr subroutines
    static GLuint SubroutineIndices[5];

};