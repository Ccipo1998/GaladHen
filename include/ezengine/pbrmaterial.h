
// Material class for PBR shading

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>

#include <ezengine/material.h>

class PBRShader;

class PBRMaterial : public IMaterial
{
public:

    // colors for the diffuse term and for the dielectric reflection
    glm::vec3 DiffuseColor;
    //GLfloat Specular;

    // PBR parameters
    float Metallic;
    float Roughness;

    // ...
    
    // @brief
    // Create material with default PBR shader
    PBRMaterial();

    // @brief
    // Set shader instance
    void SetShader(PBRShader* pbrShader);

    PBRShader* GetShader();

    // @brief
    // It sends material data to its shader
    virtual void SendDataToShader() override;

protected:

    // DOING: copy and move

    // shader to use
    PBRShader* MaterialShader;

};