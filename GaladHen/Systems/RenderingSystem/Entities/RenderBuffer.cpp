
#include "RenderBuffer.h"

namespace GaladHen
{
	RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, TextureFormat format, RenderBufferType renderBufferType)
		: Size(glm::uvec2(width, height))
		, ClearColor(GH_DEFAULT_RENDER_CLEAR_COLOR)
		, Format(format)
		, Type(renderBufferType)
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

	RenderBufferType RenderBuffer::GetRenderBufferType() const
	{
		return Type;
	}

	TextureFormat RenderBuffer::GetFormat() const
	{
		return Format;
	}
}
