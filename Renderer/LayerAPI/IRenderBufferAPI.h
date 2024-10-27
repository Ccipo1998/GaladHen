
// This is a pure virtual class (interface) to define basic functionalities of a render buffer at API level

#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	class IRenderBufferAPI
	{

	public:

		virtual unsigned int GetRenderBufferColorID() const = 0;

		virtual unsigned int GetRenderBufferDepthID() const = 0;

		virtual glm::uvec2 GetRenderBufferSize() const = 0;

	};
}
