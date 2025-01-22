
// Class for a generic (api agnostic) render buffer -> a buffer resource on GPU used as a target for rendering operations
// 
// A render buffer object offers read-only operations, like reading and copying its content from GPU memory to CPU memory; for write or change operations,
// the corresponding Renderer object must be used

#pragma once

#include "IGPUResource.h"
#include <glm/glm.hpp>

#define GH_DEFAULT_RENDER_CLEAR_COLOR glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f }

namespace GaladHen
{
	class RenderBuffer : public IGPUResource
	{
	public:

		RenderBuffer(unsigned int width, unsigned int height);

		glm::uvec2 GetSize();
		void SetSize(const glm::uvec2& size);

		glm::vec4 ClearColor;

	protected:

		glm::uvec2 Size;

	};
}
