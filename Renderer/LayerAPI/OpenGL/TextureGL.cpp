
#include "TextureGL.h"

#include <Renderer/LayerAPI/IShaderProgramAPI.h>
#include <Renderer/LayerAPI/OpenGL/ShaderProgramGL.h>

namespace GaladHen
{
    TextureGL::TextureGL()
        : TextureID(0)
    {
        // fill associations
        TextureFormatAssociations[(int)TextureFormat::RGB] = GL_RGB;
        TextureFormatAssociations[(int)TextureFormat::SRGB] = GL_SRGB;
        
        PixelFormatAssociations[(int)PixelDataFormat::R] = GL_RED;
        PixelFormatAssociations[(int)PixelDataFormat::RG] = GL_RED;
        PixelFormatAssociations[(int)PixelDataFormat::RGB] = GL_RED;
        PixelFormatAssociations[(int)PixelDataFormat::RGBA] = GL_RED;

        PixelDataTypeAssociations[(int)PixelDataType::UnsignedByte] = GL_UNSIGNED_BYTE;

        WrappingAssociations[(int)TextureWrapping::Repeat] = GL_REPEAT;
        WrappingAssociations[(int)TextureWrapping::ClampToBorder] = GL_CLAMP_TO_BORDER;
        WrappingAssociations[(int)TextureWrapping::ClampToEdge] = GL_CLAMP_TO_EDGE;
        WrappingAssociations[(int)TextureWrapping::MirroredRepeat] = GL_MIRRORED_REPEAT;
    }

    void TextureGL::LoadMemoryGPU(const void* textureBytes, unsigned int width, unsigned int height, TextureFormat textureFormat, PixelDataFormat pixelFormat, PixelDataType pixelType, bool generateMipMaps)
    {
        // create new texture object
        glGenTextures(1, &TextureID);
        // bind new texture object to texture target
        glBindTexture(GL_TEXTURE_2D, TextureID);

        // allocate immutable storage basing on number of channels and on bit depth
        // IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
        glTexStorage2D(GL_TEXTURE_2D, 1, TextureFormatAssociations[(int)textureFormat], width, height);

        // copy texture data to texture object
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, PixelFormatAssociations[(int)pixelFormat], PixelDataTypeAssociations[(int)pixelType], textureBytes);

        if (generateMipMaps)
            glGenerateTextureMipmap(TextureID);
    }

    void TextureGL::LoadTextureParameters(IShaderProgramAPI* shaderProgram, unsigned int unit, TextureWrapping wrapping, TextureFiltering filtering, TextureMipMap mipmap)
    {
        BindToTextureUnit(shaderProgram, unit);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrappingAssociations[(int)wrapping]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrappingAssociations[(int)wrapping]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringAssociations[(int)filtering]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringAssociations[(int)filtering]);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MipMapAssociations[mipmap]); // TODO: check if this is correct
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MipMapAssociations[mipmap]); 
    }

    void TextureGL::BindToTextureUnit(IShaderProgramAPI* shaderProgram, unsigned int unit)
    {
        // set shader program as current
        shaderProgram->Use();

        glBindTexture(GL_TEXTURE_2D, TextureID);
        // select texture unit for the binded texture
        glActiveTexture(unit);
    }

    bool TextureGL::IsLoaded()
    {
        // if texture id == 0 -> glGenTextures() not called yet
        return TextureID > 0;
    }

    void TextureGL::SetTextureSamplerName(IShaderProgramAPI* shaderProgram, unsigned int unit, char* samplerName)
    {
        BindToTextureUnit(shaderProgram, unit);

        // create uniform sampler
        int loc = glGetUniformLocation((static_cast<ShaderProgramGL*>(shaderProgram))->GetShaderProgram(), samplerName);
        glUniform1i(loc, 1);
    }
}