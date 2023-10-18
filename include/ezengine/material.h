/*
Material struct to store shading parameters
*/

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>

struct Material
{
    // enum type -> the type of shading model

    // Phong shading model parameters
    glm::vec3 Ka; // ambient component
    glm::vec3 Kd; // diffuse component (WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular)
    glm::vec3 Ks; // specular component
    GLfloat SpecularFalloff; // falloff parameter from perfect specular direction (usually value from 1 to 200)

    // colors for the diffuse term and for the dielectric reflection
    glm::vec3 DiffuseColor;
    //GLfloat Specular;

    GLfloat Metallic;
    GLfloat Roughness;

    // ...
};
