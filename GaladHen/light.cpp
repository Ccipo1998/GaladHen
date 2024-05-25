
#include "light.h"
#include <gtx/quaternion.hpp>

Light::Light()
    : Transform()
    , Color(glm::vec3(1.0f))
    , Intensity(1.0f)
    , LightDataShaderFormat(nullptr)
    {}

Light::Light(const glm::vec3& color, float intensity)
    : Transform()
    , Color(color)
    , Intensity(intensity)
    , LightDataShaderFormat(nullptr)
    {}

void* Light::GetLightDataForShader()
{
    return nullptr;
}

std::size_t Light::GetLightSizeForShader()
{
    return 0;
}

Light::~Light()
{
    if (LightDataShaderFormat == nullptr)
        return;

    delete this->LightDataShaderFormat;
}

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

void* DirectionalLight::GetLightDataForShader()
{
    if (LightDataShaderFormat == nullptr)
    {
        // create data
        this->LightDataShaderFormat = new DirectionalLightShaderData;
    }

    // fill data
    DirectionalLightShaderData* data = static_cast<DirectionalLightShaderData*>(LightDataShaderFormat);
    data->Position = this->Transform.GetPosition();
    data->Color = this->Color;
    data->Intensity = this->Intensity;
    data->Direction = this->Transform.GetFront();

    return data;
}

std::size_t DirectionalLight::GetLightSizeForShader()
{
    return sizeof(DirectionalLightShaderData);
}

std::size_t DirectionalLight::GetDirLightSizeForShader()
{
    return sizeof(DirectionalLight);
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

void* PointLight::GetLightDataForShader()
{
    if (LightDataShaderFormat == nullptr)
    {
        // create data
        this->LightDataShaderFormat = new PointLightShaderData;
    }

    // fill data
    PointLightShaderData* data = static_cast<PointLightShaderData*>(LightDataShaderFormat);
    data->Position = this->Transform.GetPosition();
    data->Color = this->Color;
    data->Intensity = this->Intensity;
    data->FallOffDistance = this->FallOffDistance;

    return data;
}

std::size_t PointLight::GetLightSizeForShader()
{
    return sizeof(PointLightShaderData);
}

std::size_t PointLight::GetPointLightSizeForShader()
{
    return sizeof(PointLightShaderData);
}
