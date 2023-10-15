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

    // colors for the diffuse term and for the dielectric reflection
    glm::vec3 DiffuseColor;
    //GLfloat Specular;

    GLfloat Metallic;
    GLfloat Roughness;

    // ...
};
