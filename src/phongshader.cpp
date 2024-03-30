
#include <ezengine/phongshader.h>

#define PHONG_VERTEX_SHADER_PATH "shaders/rendering/Rendering.vert"
#define PHONG_FRAGMENT_SHADER_PATH "shaders/rendering/Rendering.frag"

PhongShader::PhongShader()
{
    LoadVertexFragmentShaders(PHONG_VERTEX_SHADER_PATH, PHONG_FRAGMENT_SHADER_PATH);
}
