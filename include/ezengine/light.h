/*
Light classes for different light types
*/

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>

using namespace glm;

class Light
{
    public:

    Light(vec3 color, GLfloat intensity)
    : Color(color), Intensity(intensity)
    {

    }

    vec3 Color;
    GLfloat Intensity;
};

class DirectionalLight : public Light
{
    public:

    DirectionalLight(vec3 color, GLfloat intensity, vec3 direction)
    : Light(color, intensity), Direction(normalize(direction)) {}
    
    vec3 getDirection()
    {
        return this->Direction;
    }

    void setDirection(vec3 new_direction)
    {
        this->Direction = normalize(new_direction);
    }

    private:
    
    vec3 Direction;
};

class PointLight : public Light
{
    public:

    PointLight(vec3 color, GLfloat intensity, vec3 position)
    : Light(color, intensity), Position(position) {}

    vec3 Position;
    GLfloat FallOffDistance;
    // TODO: falloff function type (penso vada bene un enum da cui dipende la funzione usata negli shader)
};

class SpotLight : public PointLight
{
    public:

    SpotLight(vec3 color, GLfloat intensity, vec3 position, vec3 direction, GLfloat penumbra_angle, GLfloat falloff_angle)
    : PointLight(color, intensity, position), Direction(normalize(direction)), PenumbraAngle(penumbra_angle), FallOffAngle(falloff_angle) {}

    GLfloat PenumbraAngle;
    GLfloat FallOffAngle; // this is the angle from the penumbra angle and the complete shadow (UmbraAngle = PenumbraAngle + FallOffAngle)
    // TODO: directional falloff function type (penso vada bene un enum da cui dipende la funzione usata negli shader)

    vec3 getDirection()
    {
        return this->Direction;
    }

    void setDirection(vec3 new_direction)
    {
        this->Direction = normalize(new_direction);
    }

    private:

    vec3 Direction;
};
