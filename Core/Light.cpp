
#include "Light.h"
#include <glm/gtx/quaternion.hpp>

namespace GaladHen
{
    Light::Light()
        : Transform{}
        , Color(glm::vec3(1.0f))
        , Intensity(1.0f)
        {}

    Light::Light(const glm::vec3& color, float intensity)
        : Transform{}
        , Color(color)
        , Intensity(intensity)
        {}

    DirectionalLight::DirectionalLight()
        : Light()
        {}

    DirectionalLight::DirectionalLight(const glm::vec3& color, float intensity, const glm::vec3& direction)
        : Light(color, intensity)
    {
        SetDirection(direction);
    }

    glm::vec3 DirectionalLight::GetDirection()
    {
        return Transform.GetFront();
    }

    void DirectionalLight::SetDirection(const glm::vec3& direction)
    {
        glm::quat rot = glm::lookAt(Transform.GetPosition(), Transform.GetPosition() + direction, TransformQuat::GlobalUp);
        Transform.SetOrientation(rot);
    }

    PointLight::PointLight()
        : Light()
        , FallOffDistance(5.0f) // default
        {}

    PointLight::PointLight(const glm::vec3& color, float intensity, float fallOffDistance)
        : Light(color, intensity)
        , FallOffDistance(fallOffDistance)
        {}

    SpotLight::SpotLight()
        : PointLight()
        {}

    SpotLight::SpotLight(const glm::vec3& color, float intensity, float fallOffDistance, float penumbra_angle, float falloff_angle)
        : PointLight(color, intensity, fallOffDistance)
        , PenumbraAngle(penumbra_angle)
        , FallOffAngle(falloff_angle)
        {}
}
