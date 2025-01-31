
#include "DirectionalLight.h"

namespace GaladHen
{
	DirectionalLight::DirectionalLight()
	{}

	DirectionalLight::DirectionalLight(const glm::vec4& color, float intensity, const glm::vec3& direction)
		: Light(color, intensity)
	{
		SetLightDirection(direction);
	}

	glm::vec3 DirectionalLight::GetLightDirection() const
	{
		return Transform.GetFront();
	}

	void DirectionalLight::SetLightDirection(const glm::vec3& direction)
	{
		Transform.LookAt(Transform.GetPosition() + direction);
	}
}
