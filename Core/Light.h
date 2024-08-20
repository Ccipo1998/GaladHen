
// Light classes for different light types

#pragma once

#include <glm/glm.hpp>
#include "Transform.h"

// Copy constructor, Copy assignment, Move constructor and Move assignment are defaulted

namespace GaladHen
{
    class Light
    {
    public:

        TransformQuat Transform;
        glm::vec3 Color;
        float Intensity;

        Light();

        Light(const glm::vec3& color, float intensity);

    };

    class DirectionalLight : public Light
    {
    public:

        DirectionalLight();

        DirectionalLight(const glm::vec3& color, float intensity, const glm::vec3& direction);

        // @brief
        // Get the direction of the light (front versor)
        glm::vec3 GetDirection();

        // @brief
        // Set the direction of the light (front versor)
        void SetDirection(const glm::vec3& direction);

    };

    class PointLight : public Light
    {
    public:

        float BulbSize; // The (fake) "physical" size of the light bulb
        float Radius; // Of the illuminated area (sphere)

        PointLight();

        PointLight(const glm::vec3& color, float intensity, float bulbSize, float radius);

    };

    class SpotLight : public PointLight
    {
        public:

        float PenumbraAngle;
        float FallOffAngle; // this is the angle from the penumbra angle and the complete shadow (UmbraAngle = PenumbraAngle + FallOffAngle)
        // TODO: directional falloff function type (penso vada bene un enum da cui dipende la funzione usata negli shader)

        SpotLight();

        SpotLight(const glm::vec3& color, float intensity, float bulbSize, float radius, float penumbra_angle, float falloff_angle);
    };
}
