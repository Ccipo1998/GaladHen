
// Class for a generic opengl frame buffer with a texture attached

#pragma once

#include "FrameBufferGL.h"
#include "TextureGL.h"

namespace GaladHen
{
	class TextureFrameBufferGL : public FrameBufferGL
	{

	public:

		TextureFrameBufferGL();

		void Create(unsigned int width, unsigned int height, unsigned int numberOfChannels, TextureFormat format);

		virtual void Free() override;

	protected:

		TextureGL Texture;

	};
}
