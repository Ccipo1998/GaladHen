
// This is a pure virtual class (interface) to define basic functionalities of a texture at API level

#pragma once

#include <Common/TextureData.h>

namespace GaladHen
{
    class IShaderProgramAPI;

    class ITextureAPI
    {

    public:

        // @brief
        // Load texture data in GPU memory
        // @param textureBytes: actual texture data
        // @param width: texture width
        // @param height: texture height
        // @param textureFormat: format used to load entire texture data
        // @param pixelFormat: format used to interpret pixel data
        // @param pixelType: data type of a single pixel
        virtual void LoadMemoryGPU(const void* textureBytes, unsigned int width, unsigned int height, TextureFormat textureFormat, PixelDataFormat pixelFormat, PixelDataType pixelType, bool generateMipMaps = false) = 0;

        // @brief
        // Load texture parameters to use with texture data
        // @param shaderProgram: the shader program which unit and texture parameters refer to
        // @param unit: texture unit to use to set texture parameters
        // @param wrapping: wrapping mode
        // @param filtering: filtering mode
        // @param mipmap: mipmap filtering mode
        virtual void LoadTextureParameters(IShaderProgramAPI* shaderProgram, unsigned int unit, TextureWrapping wrapping, TextureFiltering filtering, TextureMipMap mipmap) = 0;

        // @brief
        // Bind a texture to a specific texture unit to be used inside a shader
        // @param shaderProgram: the shader program which the unit refers to
        // @param unit: texture unit to which bind the texture
        virtual void BindToTextureUnit(IShaderProgramAPI* shaderProgam, unsigned int unit) = 0;

        // @brief
        // Check if texture data is already loaded in GPU memory
        virtual bool IsLoaded() = 0;

        // @brief
        // Set a sampler inside a shader program
        // @param shaderProgram: the shader program in which the sampler is set
        // @param unit: the texture unit of the sampler
        // @param samplerName: the name of the sampler inside the shader program
        virtual void SetTextureSamplerName(IShaderProgramAPI* shaderProgram, unsigned int unit, char* samplerName) = 0;

    };
}
