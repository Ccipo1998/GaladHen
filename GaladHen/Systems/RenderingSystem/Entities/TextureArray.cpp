
#include "TextureArray.h"

namespace GaladHen
{
	TextureArray::TextureArray(unsigned char* data, unsigned int width, unsigned int height, unsigned int numberOfLayers, unsigned int numberOfMipMaps, TextureFormat format)
		: Texture(data, width, height, numberOfMipMaps, format)
		, NumberOfLayers(numberOfLayers)
	{}

	unsigned int TextureArray::GetNumberOfLayers() const
	{
		return NumberOfLayers;
	}
}
