
#include "RenderBuffer.h"

namespace GaladHen
{
	RenderBuffer::RenderBuffer(unsigned int width, unsigned int height)
		: Size(glm::uvec2(width, height))
		, ClearColor(GH_DEFAULT_RENDER_CLEAR_COLOR)
	{}

	glm::uvec2 RenderBuffer::GetSize()
	{
		return Size;
	}

	void RenderBuffer::SetSize(const glm::uvec2& size)
	{
		Size = size;

		InvalidateResource();
	}
}
