
// Datas about textures

#pragma once

namespace GaladHen
{
	enum class TextureFormat
	{
		RGB,
		RGB8,
		SRGB,
		SRGB8
	};

	enum class TextureAccessType
	{
		ReadOnly,
		WriteOnly,
		ReadWrite
	};

	struct TextureData
	{
		unsigned int Width;
		unsigned int Height;
		void* Data;
		unsigned int NumberOfChannels;
		unsigned int NumberOfMipMaps;
		TextureFormat Format;
		TextureAccessType AccessType;
	};
}
