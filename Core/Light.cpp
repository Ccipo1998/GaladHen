
#include "Light.h"
#include <glm/gtx/quaternion.hpp>

namespace GaladHen
{
    Light::Light()
        : Transform{}
        , Color(glm::vec3(1.0f))
        , Intensity(10000.0f)
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
        , BulbSize(0.01f) // default
        , Radius(100.0f) // default
        {}

    PointLight::PointLight(const glm::vec3& color, float intensity, float bulbSize, float radius)
        : Light(color, intensity)
        , BulbSize(bulbSize)
        , Radius(radius)
        {}

    SpotLight::SpotLight()
        : PointLight()
        {}

    SpotLight::SpotLight(const glm::vec3& color, float intensity, float bulbSize, float radius, float penumbra_angle, float falloff_angle)
        : PointLight(color, intensity, bulbSize, radius)
        , PenumbraAngle(penumbra_angle)
        , FallOffAngle(falloff_angle)
        {}
}
