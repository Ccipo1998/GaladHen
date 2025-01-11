
#include "SpotLight.h"

namespace GaladHen
{
	SpotLight::SpotLight()
		: PenumbraAngle(10.0f)
		, FallOffAngle(45.0f)
	{}

	SpotLight::SpotLight(const glm::vec3& color, float intensity, float bulbSize, float radius, float penumbraAngle, float falloffAngle)
		: PointLight(color, intensity, bulbSize, radius)
		, PenumbraAngle(penumbraAngle)
		, FallOffAngle(falloffAngle)
	{}
}
