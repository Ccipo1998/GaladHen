
// OpenGL-specific texture class

#pragma once

#include <Renderer/LayerAPI/ITextureAPI.h>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

namespace GaladHen
{
    enum class TextureAllocationType
    {
        Mutable,
        Immutable
    };

    class TextureGL : public ITextureAPI
    {

    public:

        TextureGL();

        virtual void LoadMemoryGPU(const void* textureBytes, unsigned int width, unsigned int height, unsigned int numberOfChannels, TextureFormat textureFormat, unsigned int numberOfMipMaps) override;

        void LoadDepthStencilMemoryGPU(unsigned int width, unsigned int height);

        virtual void LoadTextureParameters(const TextureParameters& params) const override;

        virtual void BindToTextureUnit(IShaderProgramAPI* shaderProgram, unsigned int unit) const override;

        virtual bool IsLoaded() override;

        virtual void SetTextureSamplerName(IShaderProgramAPI* shaderProgram, unsigned int unit, const char* samplerName) const override;

        virtual void FreeMemoryGPU() override;

        // OPENGL --------------------------------------------------------------------------------------------------------------------------

        void SetAllocationType(TextureAllocationType allocationType);

        TextureAllocationType GetAllocationType() const;

        void Bind() const;

        GLuint GetTextureID() const;

        GLuint TextureID;
    protected:


        TextureAllocationType AllocationType;

        // OpenGL texture formats are called Sized Internal Formats
        // https://www.khronos.org/opengl/wiki/Image_Format
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
        static GLenum TextureFormatAssociations[61];

        // OpenGL pixel formats are called Base Internal Formats
        // https://www.khronos.org/opengl/wiki/Image_Format
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
        static GLenum PixelChannelsAssociations[4];

        // OpenGL pixel data type
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml (type parameter)
        static GLenum PixelChannelDepthAssociations[19];

        // OpenGL wrapping parameters
        static GLint WrappingAssociations[4];

        // OpenGL filtering parameters
        static GLint FilteringAssociations[6];

        static GLenum TextureUnits[32]; // OpenGL has a maximum of 32 texture units

    };
}
