
#pragma once

#include "Light.h"

namespace GaladHen
{
	class DirectionalLight : public Light
	{
	public:

		DirectionalLight();

		DirectionalLight(const glm::vec4& color, float intensity, const glm::vec3& direction); // Dir light direction is front of Transform

		glm::vec3 GetLightDirection() const;

		void SetLightDirection(const glm::vec3& direction);

	};
}
