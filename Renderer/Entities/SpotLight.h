
#pragma once

#include "PointLight.h"

namespace GaladHen
{
    class SpotLight : public PointLight
    {
    public:

        SpotLight();

        SpotLight(const glm::vec3& color, float intensity, float bulbSize, float radius, float penumbraAngle, float falloffAngle);

        float PenumbraAngle;
        float FallOffAngle; // this is the angle from the penumbra angle and the complete shadow (UmbraAngle = PenumbraAngle + FallOffAngle)
        // TODO: directional falloff function type (penso vada bene un enum da cui dipende la funzione usata negli shader)
    };
}
