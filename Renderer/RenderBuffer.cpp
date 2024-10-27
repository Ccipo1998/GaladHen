
#include "RenderBuffer.h"

#include <Renderer/LayerAPI/OpenGL/RenderBufferGL.h>

namespace GaladHen
{
	RenderBuffer::RenderBuffer(const IRenderBufferAPI* renderBufferAPI)
		: RenderBufferAPI(renderBufferAPI)
	{}

	unsigned int RenderBuffer::GetRenderBufferColorID() const
	{
		return RenderBufferAPI->GetRenderBufferColorID();
	}

	unsigned int RenderBuffer::GetRenderBufferDepthID() const
	{
		return RenderBufferAPI->GetRenderBufferDepthID();
	}

	glm::uvec2 RenderBuffer::GetRenderBufferSize() const
	{
		return RenderBufferAPI->GetRenderBufferSize();
	}
}
