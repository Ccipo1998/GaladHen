
#pragma once

#include "Light.h"

namespace GaladHen
{
	class PointLight : public Light
	{
	public:

		PointLight();

		PointLight(const glm::vec3& color, float intensity, float bulbSize, float radius);

		float BulbSize; // The (fake) "physical" size of the light bulb
		float Radius; // Of the illuminated area (sphere)

	};
}
