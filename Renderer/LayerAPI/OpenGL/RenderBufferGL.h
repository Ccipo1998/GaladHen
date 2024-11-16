
// This class is not the GaladHen direct wrapper for an OpenGL render buffer, but it is the class holding a framebuffer with 2 attachments:
// - Color buffer -> always a frame buffer and not a render buffer, because we want to be able to sample it inside shaders
// - Depth and stencil buffer -> always a frame buffer and not a render buffer, becausa we wanto to be able to sample it inside shaders

#pragma once

#include <Renderer/LayerAPI/IRenderBufferAPI.h>

#include "FrameBufferGL.h"
#include "TextureGL.h"

namespace GaladHen
{
	class RenderBufferGL : public IRenderBufferAPI, public FrameBufferGL
	{

	public:

		RenderBufferGL();

		// Explicit copy needed for a render buffer
		RenderBufferGL(const RenderBufferGL& other) = delete;
		RenderBufferGL& operator=(const RenderBufferGL& other) = delete;
		RenderBufferGL& operator=(RenderBufferGL&& other) = delete;
		RenderBufferGL(RenderBufferGL&& other) = delete;

		// FRAMEBUFFER ----------------------------------------------------------------------------------------

		void Create(unsigned int width, unsigned int height);

		virtual void Free() override;

		// RENDERBUFFER ---------------------------------------------------------------------------------------

		virtual unsigned int GetRenderBufferColorID() const override;

		virtual unsigned int GetRenderBufferDepthID() const override;

		virtual glm::uvec2 GetRenderBufferSize() const override;

	protected:

		TextureGL ColorTexture;
		TextureGL DepthStencilTexture;
	};
}
