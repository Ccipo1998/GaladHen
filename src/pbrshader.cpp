
#include <ezengine/pbrshader.h>

#define PBR_VERTEX_SHADER_PATH "shaders/rendering/Rendering.vert"
#define PBR_FRAGMENT_SHADER_PATH "shaders/rendering/Rendering.frag"

PBRShader::PBRShader()
{
    LoadVertexFragmentShaders(PBR_VERTEX_SHADER_PATH, PBR_FRAGMENT_SHADER_PATH);
}
