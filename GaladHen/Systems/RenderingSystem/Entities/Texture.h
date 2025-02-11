
#pragma once

#include <memory>

#include <glm/fwd.hpp>
#include "IGPUResource.h"

namespace GaladHen
{
	enum class TextureFormat
	{
		RGB,
		RGB8,
		SRGB,
		SRGB8
	};

	enum class TextureWrapping
	{
		Repeat = 0,
		ClampToBorder = 1,
		ClampToEdge = 2,
		MirroredRepeat = 3
	};

	enum class TextureFiltering
	{
		Linear = 0,
		Nearest = 1,
		LinearLinear = 2,
		NearestNearest = 3,
		LinearNearest = 4,
		NearestLinear = 5
	};

	enum class TextureAccessType
	{
		ReadOnly,
		WriteOnly,
		ReadWrite
	};

	class Texture : public IGPUResource
	{
	public:

		Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int numberOfChannels, unsigned int numberOfMipMaps, TextureFormat format);

		Texture(const Texture& sourceTexture) = delete;
		Texture& operator=(Texture& sourceTexture) = delete;
		Texture(Texture&& sourceTexture) = delete;
		Texture& operator=(Texture&& sourceTexture) = delete;

		unsigned char* GetData() const;
		void GetSize(glm::uvec2& outSize) const;
		unsigned int GetNumberOfChannels() const;
		unsigned int GetNumberOfMipMaps() const;
		TextureFormat GetFormat() const;
		TextureWrapping GetWrapping() const;
		TextureFiltering GetFiltering() const;
		void SetWrapping(TextureWrapping wrapping);
		void SetFiltering(TextureFiltering filtering);
		void SetNumberOfMipMaps(unsigned int numberOfMipMaps);

		~Texture();

	protected:

		unsigned char* Data; // ownership of data
		unsigned int Width;
		unsigned int Height;
		unsigned int NumberOfChannels;
		unsigned int NumberOfMipMaps;
		TextureFormat Format;
		TextureWrapping Wrapping;
		TextureFiltering Filtering;
	};
}
