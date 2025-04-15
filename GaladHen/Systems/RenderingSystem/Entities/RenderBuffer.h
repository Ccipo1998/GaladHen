
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
	enum class TextureFormat;

	class RenderBuffer : public IGPUResource
	{
	public:

		RenderBuffer(unsigned int width, unsigned int height, TextureFormat format, bool enableDepth = true);

		glm::uvec2 GetSize() const;
		void SetSize(const glm::uvec2& size);

		bool IsDepthBufferAttached() const;
		TextureFormat GetFormat() const;

		glm::vec4 ClearColor;

	protected:

		TextureFormat Format;
		glm::uvec2 Size;
		bool DepthBufferAttached;

	};
}
