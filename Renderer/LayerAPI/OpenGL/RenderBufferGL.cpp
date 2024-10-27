
#include "RenderBufferGL.h"

#include <Core/TextureData.h>

#include <Utils/Log.h>

namespace GaladHen
{
	RenderBufferGL::RenderBufferGL()
		: FrameBufferGL()
		, ColorTexture()
		, DepthStencilTexture()
	{}

	void RenderBufferGL::Create(unsigned int width, unsigned int height)
	{
		FrameBufferGL::Create();
		Bind();

		TextureParameters params{};
		params.MinFiltering = TextureFiltering::Linear;
		params.MagFiltering = TextureFiltering::Linear;

		ColorTexture.SetAllocationType(TextureAllocationType::Mutable);
		ColorTexture.LoadMemoryGPU(nullptr, width, height, 3, TextureFormat::RGB, 0);
		ColorTexture.LoadTextureParameters(params);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture.GetTextureID(), 0);

		DepthStencilTexture.LoadDepthStencilMemoryGPU(width, height);
		DepthStencilTexture.LoadTextureParameters(params);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthStencilTexture.GetTextureID(), 0);

		/*glGenTextures(1, &ColorTexture.TextureID);
		glBindTexture(GL_TEXTURE_2D, ColorTexture.TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture.TextureID, 0);

		glGenTextures(1, &DepthStencilTexture.TextureID);
		glBindTexture(GL_TEXTURE_2D, DepthStencilTexture.TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthStencilTexture.TextureID, 0);*/

		if (CheckStatus() != GL_FRAMEBUFFER_COMPLETE)
		{
			Log::Error("RenderBufferGL", "ERROR::FRAMEBUFFER::Framebuffer is not complete!\n");
		}

		Unbind();
	}

	void RenderBufferGL::Free()
	{
		ColorTexture.FreeMemoryGPU();
		DepthStencilTexture.FreeMemoryGPU();

		FrameBufferGL::Free();
	}

	unsigned int RenderBufferGL::GetRenderBufferColorID() const
	{
		return ColorTexture.GetTextureID();
	}

	unsigned int RenderBufferGL::GetRenderBufferDepthID() const
	{
		return DepthStencilTexture.GetTextureID();
	}

	glm::uvec2 RenderBufferGL::GetRenderBufferSize() const
	{
		return glm::uvec2();
	}
}
