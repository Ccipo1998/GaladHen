
#include "TextureGL.h"

#include <Renderer/LayerAPI/IShaderProgramAPI.h>
#include <Renderer/LayerAPI/OpenGL/ShaderProgramGL.h>
#include <Core/Texture.h>


#include <GL/gl3w.h>

namespace GaladHen
{

    GLenum TextureGL::TextureFormatAssociations[61] =
    {
        GL_RGB, // (int)TextureFormat::RGB
        GL_RGB8, // (int)TextureFormat::RGB8
        GL_SRGB, // (int)TextureFormat::SRGB
        GL_SRGB8 // (int)TextureFormat::SRGB8
    };

    GLenum TextureGL::PixelChannelsAssociations[4] =
    {
        GL_RED,
        GL_RG,
        GL_RGB,
        GL_RGBA
    };

    GLenum TextureGL::PixelChannelDepthAssociations[19] =
    {
        GL_UNSIGNED_BYTE
    };

    GLint TextureGL::WrappingAssociations[4] =
    {
        GL_REPEAT,
        GL_CLAMP_TO_BORDER,
        GL_CLAMP_TO_EDGE,
        GL_MIRRORED_REPEAT
    };

    GLint TextureGL::FilteringAssociations[6] =
    {
        GL_LINEAR,
        GL_NEAREST,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_NEAREST_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_NEAREST,
        GL_NEAREST_MIPMAP_LINEAR
    };

    GLenum TextureGL::TextureUnits[32] =
    {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3,
        GL_TEXTURE4,
        GL_TEXTURE5,
        GL_TEXTURE6,
        GL_TEXTURE7,
        GL_TEXTURE8,
        GL_TEXTURE9,
        GL_TEXTURE10,
        GL_TEXTURE11,
        GL_TEXTURE12,
        GL_TEXTURE13,
        GL_TEXTURE14,
        GL_TEXTURE16,
        GL_TEXTURE17,
        GL_TEXTURE18,
        GL_TEXTURE19,
        GL_TEXTURE20,
        GL_TEXTURE21,
        GL_TEXTURE22,
        GL_TEXTURE23,
        GL_TEXTURE24,
        GL_TEXTURE25,
        GL_TEXTURE26,
        GL_TEXTURE27,
        GL_TEXTURE28,
        GL_TEXTURE29,
        GL_TEXTURE30,
        GL_TEXTURE31
    };

    TextureGL::TextureGL()
        : TextureID(0)
        , AllocationType(TextureAllocationType::Immutable)
    {}

    void TextureGL::LoadMemoryGPU(const void* textureBytes, unsigned int width, unsigned int height, unsigned int numberOfChannels, TextureFormat textureFormat, unsigned int numberOfMipMaps)
    {
        // free old memory
        FreeMemoryGPU();

        // create new texture object
        glGenTextures(1, &TextureID);
        // bind new texture object to texture target
        glBindTexture(GL_TEXTURE_2D, TextureID);

        switch (AllocationType)
        {
        case GaladHen::TextureAllocationType::Mutable:
        {
            glTexImage2D(GL_TEXTURE_2D, numberOfMipMaps, TextureFormatAssociations[(int)textureFormat], width, height, 0, PixelChannelsAssociations[numberOfChannels - 1], PixelChannelDepthAssociations[0], textureBytes);
            break;
        }
        case GaladHen::TextureAllocationType::Immutable:
        {
            // allocate immutable storage basing on number of channels and on bit depth
            // IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
            // levels are the number of mipmaps
            glTexStorage2D(GL_TEXTURE_2D, numberOfMipMaps, TextureFormatAssociations[(int)textureFormat], width, height);

            // copy texture data to texture object
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, PixelChannelsAssociations[numberOfChannels - 1], PixelChannelDepthAssociations[0], textureBytes);
        }
        default:
            break;
        }

        if (numberOfMipMaps > 0)
            glGenerateTextureMipmap(TextureID);
    }

    void TextureGL::LoadDepthStencilMemoryGPU(unsigned int width, unsigned int height)
    {
        // free old memory
        FreeMemoryGPU();

        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void TextureGL::LoadTextureParameters(const TextureParameters& params) const
    {
        Bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrappingAssociations[(int)params.HorizontalWrapping]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrappingAssociations[(int)params.VerticalWrapping]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringAssociations[(int)params.MinFiltering]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringAssociations[(int)params.MagFiltering]);
    }

    void TextureGL::BindToTextureUnit(IShaderProgramAPI* shaderProgram, unsigned int unit) const
    {
        // set shader program as current
        shaderProgram->Use();

        // select texture unit for the binded texture
        glActiveTexture(TextureUnits[unit]);

        glBindTexture(GL_TEXTURE_2D, TextureID);
    }

    bool TextureGL::IsLoaded()
    {
        // if texture id == 0 -> glGenTextures() not called yet
        return TextureID > 0;
    }

    void TextureGL::SetTextureSamplerName(IShaderProgramAPI* shaderProgram, unsigned int unit, const char* samplerName) const
    {
        BindToTextureUnit(shaderProgram, unit);

        // create uniform sampler
        int loc = glGetUniformLocation((static_cast<ShaderProgramGL*>(shaderProgram))->GetShaderProgram(), samplerName);
        glUniform1i(loc, unit);
    }

    void TextureGL::FreeMemoryGPU()
    {
        // TODO
        glDeleteTextures(1, &TextureID);
    }

    void TextureGL::SetAllocationType(TextureAllocationType allocationType)
    {
        AllocationType = allocationType;
    }

    TextureAllocationType TextureGL::GetAllocationType() const
    {
        return AllocationType;
    }

    void TextureGL::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, TextureID);
    }

    GLuint TextureGL::GetTextureID() const
    {
        return TextureID;
    }
}
