
#include <glm/glm.hpp>

#include "Light.h"

namespace GaladHen
{
	Light::Light()
		: Color(glm::vec3(1.0f))
		, Intensity(1.0f)
	{}

	Light::Light(const glm::vec3& color, float intensity)
		: Color(color)
		, Intensity(intensity)
	{}
}
