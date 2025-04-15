
#include "RenderBuffer.h"

namespace GaladHen
{
	RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, TextureFormat format, bool enableDepth)
		: Size(glm::uvec2(width, height))
		, ClearColor(GH_DEFAULT_RENDER_CLEAR_COLOR)
		, Format(format)
		, DepthBufferAttached(enableDepth)
	{}

	glm::uvec2 RenderBuffer::GetSize() const
	{
		return Size;
	}

	void RenderBuffer::SetSize(const glm::uvec2& size)
	{
		Size = size;

		InvalidateResource();
	}

	bool RenderBuffer::IsDepthBufferAttached() const
	{
		return DepthBufferAttached;
	}

	TextureFormat RenderBuffer::GetFormat() const
	{
		return Format;
	}
}
