
// This is a pure virtual class (interface) to define basic functionalities of a texture at API level

#pragma once

#include <Core/TextureData.h>

namespace GaladHen
{
    class IShaderProgramAPI;
    enum class TextureFormat;

    class ITextureAPI
    {

    public:

        // @brief
        // Load texture data in GPU memory
        // @param textureBytes: actual texture data
        // @param width: texture width
        // @param height: texture height
        // @param numberOfChannels: the number of channels the texture has (R, RG, RGB, RGBA)
        // @param textureFormat: internal texture format (used to interpret correctly texture data)
        // @param numberOfMipMaps: the number of mipmaps to generate
        virtual void LoadMemoryGPU(const void* textureBytes, unsigned int width, unsigned int height, unsigned int numberOfChannels, TextureFormat textureFormat, unsigned int numberOfMipMaps) = 0;

        // @brief
        // Load texture parameters to use with texture data
        // @param shaderProgram: the shader program which unit and texture parameters refer to
        // @param unit: texture unit to use to set texture parameters
        // @param wrapping: wrapping mode
        // @param filtering: filtering mode
        // @param mipmap: mipmap filtering mode
        virtual void LoadTextureParameters(IShaderProgramAPI* shaderProgram, unsigned int unit, const TextureParameters& params) const = 0;

        // @brief
        // Bind a texture to a specific texture unit to be used inside a shader
        // @param shaderProgram: the shader program which the unit refers to
        // @param unit: texture unit to which bind the texture
        virtual void BindToTextureUnit(IShaderProgramAPI* shaderProgam, unsigned int unit) const = 0;

        // @brief
        // Check if texture data is already loaded in GPU memory
        virtual bool IsLoaded() = 0;

        // @brief
        // Set a sampler inside a shader program
        // @param shaderProgram: the shader program in which the sampler is set
        // @param unit: the texture unit of the sampler
        // @param samplerName: the name of the sampler inside the shader program
        virtual void SetTextureSamplerName(IShaderProgramAPI* shaderProgram, unsigned int unit, const char* samplerName) const = 0;

        virtual void FreeMemoryGPU() = 0;

    };
}
