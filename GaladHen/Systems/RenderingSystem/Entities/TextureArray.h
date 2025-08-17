
#pragma once

#include "Texture.h"

namespace GaladHen
{
	class TextureArray : public Texture
	{
	public:

		TextureArray(unsigned char* data, unsigned int width, unsigned int height, unsigned int numberOfLayers, unsigned int numberOfMipMaps, TextureFormat format);

		unsigned int GetNumberOfLayers() const;

	protected:

		unsigned int NumberOfLayers;
	};
}
