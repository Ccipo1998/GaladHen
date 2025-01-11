
#include "Texture.h"
#include <glm/glm.hpp>

namespace GaladHen
{
	Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int numberOfChannels, unsigned int numberOfMipMaps, TextureFormat format)
		: Width(width)
		, Height(height)
		, NumberOfChannels(numberOfChannels)
		, NumberOfMipMaps(numberOfMipMaps)
		, Format(format)
		, TextureID(0)
		, Filtering(TextureFiltering::Linear)
		, Wrapping(TextureWrapping::Repeat)
	{
		Data = std::shared_ptr<unsigned char>(data);
	}

	std::weak_ptr<unsigned char> Texture::GetData() const
	{
		return std::weak_ptr<unsigned char>(Data);
	}

	void Texture::GetSize(glm::uvec2& outSize) const
	{
		outSize.x = Width;
		outSize.y = Height;
	}

	unsigned int Texture::GetNumberOfChannels() const
	{
		return NumberOfChannels;
	}

	unsigned int Texture::GetNumberOfMipMaps() const
	{
		return NumberOfMipMaps;
	}

	TextureFormat Texture::GetFormat() const
	{
		return Format;
	}

	TextureWrapping Texture::GetWrapping() const
	{
		return Wrapping;
	}

	TextureFiltering Texture::GetFiltering() const
	{
		return Filtering;
	}

	void Texture::SetWrapping(TextureWrapping wrapping)
	{
		Wrapping = wrapping;

		InvalidateResource();
	}

	void Texture::SetFiltering(TextureFiltering filtering)
	{
		Filtering = filtering;

		InvalidateResource();
	}

	void Texture::SetNumberOfMipMaps(unsigned int numberOfMipMaps)
	{
		NumberOfMipMaps = numberOfMipMaps;

		InvalidateResource();
	}
}
