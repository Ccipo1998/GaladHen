
#pragma once

#include "RenderBuffer.h"

namespace GaladHen
{
	class RenderBufferArray : public RenderBuffer
	{
	public:

		RenderBufferArray(unsigned int width, unsigned int height, unsigned int numberOfLayers, TextureFormat format, RenderBufferType renderBufferType = RenderBufferType::ColorOnly);

		unsigned int GetNumberOfLayers() const;

	protected:

		unsigned int NumberOfLayers;
	};
}
