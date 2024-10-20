
#include "RenderFrameBufferGL.h"

#include <Core/TextureData.h>

namespace GaladHen
{
	RenderFrameBufferGL::RenderFrameBufferGL()
		: FrameBufferGL()
		, DepthStencilTexture()
	{}

	void RenderFrameBufferGL::Create(unsigned int width, unsigned int height)
	{
		FrameBufferGL::Create();

		ColorTexture.LoadMemoryGPU(nullptr, width, height, 3, TextureFormat::SRGB8, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture.GetTextureID(), 0);

		DepthStencilTexture.LoadMemoryGPU(nullptr, width, height, 3, TextureFormat::SRGB8, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, DepthStencilTexture.GetTextureID(), 0);
	}

	void RenderFrameBufferGL::Free()
	{
		ColorTexture.FreeMemoryGPU();
		DepthStencilTexture.FreeMemoryGPU();

		FrameBufferGL::Free();
	}
}
