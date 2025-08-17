
#include "RenderBufferArray.h"

namespace GaladHen
{
	RenderBufferArray::RenderBufferArray(unsigned int width, unsigned int height, unsigned int numberOfLayers, TextureFormat format, RenderBufferType renderBufferType /*= RenderBufferType::ColorOnly*/)
		: RenderBuffer(width, height, format, renderBufferType)
		, NumberOfLayers(numberOfLayers)
	{}

	unsigned int RenderBufferArray::GetNumberOfLayers() const
	{
		return NumberOfLayers;
	}
}
