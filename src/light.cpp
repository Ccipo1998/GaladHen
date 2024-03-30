
#include <ezengine/light.h>
#include <gtx/quaternion.hpp>

Light::Light()
    : Transform()
    , Color(glm::vec3(1.0f))
    , Intensity(1.0f)
    {}

Light::Light(const glm::vec3& color, float intensity)
    : Transform()
    , Color(color)
    , Intensity(intensity)
    {}

DirectionalLight::DirectionalLight()
    : Light()
    {}

DirectionalLight::DirectionalLight(const glm::vec3& color, float intensity, const glm::vec3& direction)
    : Light(color, intensity)
{
    this->SetDirection(direction);
}

glm::vec3 DirectionalLight::GetDirection()
{
    return this->Transform.GetFront();
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
    glm::quat rot = glm::lookAt(this->Transform.GetPosition(), this->Transform.GetPosition() + direction, TransformQuat::GetGlobalUp());
    this->Transform.SetRotation(rot);
}

PointLight::PointLight()
    : Light()
    , FallOffDistance(5.0f) // default
    {}

PointLight::PointLight(const glm::vec3& color, float intensity, const glm::vec3& position)
    : Light(color, intensity)
{
    this->Transform.SetPosition(position);
}

SpotLight::SpotLight()
    : PointLight()
    {}

SpotLight::SpotLight(const glm::vec3& color, float intensity, const glm::vec3& position, float penumbra_angle, float falloff_angle)
    : PointLight(color, intensity, position)
    , PenumbraAngle(penumbra_angle)
    , FallOffAngle(falloff_angle)
    {}
