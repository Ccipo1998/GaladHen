
// OpenGL-specific texture class

#pragma once

#include <Renderer/LayerAPI/ITextureAPI.h>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

namespace GaladHen
{
    class TextureGL : public ITextureAPI
    {

    public:

        TextureGL();

        virtual void LoadMemoryGPU(const void* textureBytes, unsigned int width, unsigned int height, TextureFormat textureFormat, PixelDataFormat pixelFormat, PixelDataType pixelType, bool generateMipMaps) override;

        virtual void LoadTextureParameters(IShaderProgramAPI* shaderProgram, unsigned int unit, TextureWrapping wrapping, TextureFiltering filtering, TextureMipMap mipmap) override;

        virtual void BindToTextureUnit(IShaderProgramAPI* shaderProgram, unsigned int unit) override;

        virtual bool IsLoaded() override;

        virtual void SetTextureSamplerName(IShaderProgramAPI* shaderProgram, unsigned int unit, char* samplerName) override;

        virtual void FreeMemoryGPU() override;

    protected:

        GLuint TextureID;

        // OpenGL texture formats are called Sized Internal Formats
        // https://www.khronos.org/opengl/wiki/Image_Format
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
        int TextureFormatAssociations[61];

        // OpenGL pixel formats are called Base Internal Formats
        // https://www.khronos.org/opengl/wiki/Image_Format
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
        int PixelFormatAssociations[4];

        // OpenGL pixel data type
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml (type parameter)
        int PixelDataTypeAssociations[19];

        // OpenGL wrapping parameters
        int WrappingAssociations[4];

        // OpenGL filtering parameters
        int FilteringAssociations[2];

        // OpenGL mipmapping filtering associations
        int MipMapAssociations[4];

    };
}
