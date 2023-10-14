/*
Material struct to store shading parameters
*/

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>

using namespace glm;

struct Material
{
    // enum type -> the type of shading model

    // colors for the diffuse term and for the dielectric reflection
    vec3 DiffuseColor;
    //GLfloat Specular;

    GLfloat Metallic;
    GLfloat Roughness;

    // ...
};
