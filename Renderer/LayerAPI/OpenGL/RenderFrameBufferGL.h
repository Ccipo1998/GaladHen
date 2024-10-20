
// This class is not the GaladHen direct wrapper for an OpenGL render buffer, but it is the class holding a framebuffer with 3 attachments:
// - Color buffer -> always a TextureFrameBufferGL because we want to be able to sample it inside shaders
// - Depth and stencil buffer -> always a TextureFrameBufferGL becausa we wanto to be able to sample it inside shaders

#pragma once

// Specific class for a framebuffer used as GaladHen main render target

#include "FrameBufferGL.h"
#include "TextureGL.h"

namespace GaladHen
{
	class RenderFrameBufferGL : public FrameBufferGL
	{

	public:

		RenderFrameBufferGL();

		void Create(unsigned int width, unsigned int height);

		virtual void Free() override;

	protected:

		TextureGL ColorTexture;
		TextureGL DepthStencilTexture;

	};
}
