
#include "TextureFrameBufferGL.h"

#include <Core/Texture.h>

namespace GaladHen
{
	TextureFrameBufferGL::TextureFrameBufferGL()
		: FrameBufferGL()
		, Texture()
	{}

	void TextureFrameBufferGL::Create(unsigned int width, unsigned int height, unsigned int numberOfChannels, TextureFormat format)
	{
		FrameBufferGL::Create();

		Texture.LoadMemoryGPU(nullptr, width, height, numberOfChannels, format, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture.GetTextureID(), 0);
	}

	void TextureFrameBufferGL::Free()
	{
		Texture.FreeMemoryGPU();

		FrameBufferGL::Free();
	}
}
