
#include "PointLight.h"

namespace GaladHen
{
	PointLight::PointLight()
		: BulbSize(1.0f)
		, Radius(10.0f)
	{}

	PointLight::PointLight(const glm::vec3& color, float intensity, float bulbSize, float radius)
		: Light(color, intensity)
		, BulbSize(bulbSize)
		, Radius(radius)
	{}
}
