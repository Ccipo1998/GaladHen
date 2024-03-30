
#pragma once

#include <ezengine/material.h>

#include <glm.hpp>

class PhongShader;

class PhongMaterial : public IMaterial
{

public:

    // TODO: enum type -> the type of shading model?

    // colors for the diffuse term and for the dielectric reflection
    glm::vec3 DiffuseColor;
    // Phong shading model parameters
    glm::vec3 Ka; // ambient component
    glm::vec3 Kd; // diffuse component (WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular)
    glm::vec3 Ks; // specular component
    float SpecularFallOff; // falloff parameter from perfect specular direction (usually value from 1 to 200)

    // @brief
    // Create material with default PBR shader
    PhongMaterial();

    PhongShader* GetShader();

    // @brief
    // It sends material data to its shader
    virtual void SendDataToShader() override;

protected:

    // DOING: copy and move

    // shader to use
    PhongShader* MaterialShader;

};
