
// Class for a generic (api agnostic) render buffer -> a buffer resource on GPU used as a target for rendering operations
// 
// A render buffer object offers read-only operations, like reading and copying its content from GPU memory to CPU memory; for write or change operations,
// the corresponding Renderer object must be used

#pragma once

#include "Common.h"

#include <glm/fwd.hpp>

namespace GaladHen
{
	class IRenderBufferAPI;

	class RenderBuffer
	{

	public:

		RenderBuffer(const IRenderBufferAPI* renderBufferAPI);

		unsigned int GetRenderBufferColorID() const;

		unsigned int GetRenderBufferDepthID() const;

		glm::uvec2 GetRenderBufferSize() const;

	protected:

		const IRenderBufferAPI* RenderBufferAPI;

	};
}
