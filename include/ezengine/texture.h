
// This is a class pointing to a texture and binded to a shader, exposing opengl-related functions

#include <GL/gl3w.h>

class Shader;
class TextureImage;

class Texture
{

public:

    // Void texture is non-sense
    Texture() = delete;

    Texture(const TextureImage* textureData);

    // Texture has not copy and move constructors and assignments -> a texture can be created only calling explicitly LoadTexture
    Texture(Texture& OtherTexture) = delete;
    Texture& operator=(Texture& OtherTexture) = delete;
    Texture(Texture&& OtherTexture) = delete;
    Texture&& operator=(Texture&& OtherTexture) = delete;

    const TextureImage* GetTextureImage() const;

    // @brief
    // Load texture data into gpu memory
    // @param samplername: the name of the uniform sampler variable used inside shader's code
    // @param shader: pointer to shader instance to which link the texture data
    void SendDataToShader(const unsigned char* samplerName, const Shader* shader);

    GLuint GetTextureID();

    // @brief
    // Set wrapping mode to GL_REPEAT for X axis
    void SetRepeatWrappingX();

    // @brief
    // Set wrapping mode to GL_REPEAT for Y axis
    void SetRepeatWrappingY();

    // @brief
    // Set wrapping mode to GL_MIRRORED_REPEAT for X axis
    void SetMirrorRepeatWrappingX();
    
    // @brief
    // Set wrapping mode to GL_MIRRORED_REPEAT for Y axis
    void SetMirrorRepeatWrappingY();

    // @brief
    // Set wrapping mode to GL_CLAMP_TO_BORDER for X axis
    void SetClampToBorderWrappingX();
    
    // @brief
    // Set wrapping mode to GL_CLAMP_TO_BORDER for Y axis
    void SetClampToBorderWrappingY();

    // @brief
    // Set wrapping mode to GL_CLAMP_TO_EDGE for X axis
    void SetClampToEdgeWrappingX();
    
    // @brief
    // Set wrapping mode to GL_CLAMP_TO_EDGE for Y axis
    void SetClampToEdgeWrappingY();

    // @brief
    // Set filtering mode to GL_LINEAR
    void SetLinearFiltering();

    // @brief
    // Set wrapping mode to GL_NEAREST
    void SetNearestFiltering();

    // @brief
    // Set mipmap mode to GL_NEAREST_MIPMAP_NEAREST
    void SetNearestMipMap();

    // @brief
    // Set mipmap mode to GL_LINEAR_MIPMAP_LINEAR
    void SetLinearMipMap();

    // @brief
    // Set mipmap mode to GL_NEAREST_MIPMAP_LINEAR
    void SetNearestLinearMipMap();

    // @brief
    // Set mipmap mode to GL_LINEAR_MIPMAP_NEAREST
    void SetLinearNearestMipMap();

protected:

    const TextureImage* TextureData;
    GLuint TextureID;
    bool IsBinded;

    // texture rendering parameters
    int WrappingModeX;
    int WrappingModeY;
    int FilteringMode;
    int MipMapMode;

};
