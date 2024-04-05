
// Material class for PBR shading

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>

#include <ezengine/material.h>

class PBRMaterial : public Material
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
    // Create material
    PBRMaterial(Shader* pbrShader);

    // @brief
    // It sends material data to its shader
    virtual void SendDataToShader() override;

};