
// Unlit (solid color) vertex shader

#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 2) in vec2 UV;
layout (location = 5) in vec4 Color;

// uniforms
layout (std140, binding = 0) uniform CameraData
{
    uniform mat4 ViewMatrix;
    uniform mat4 ProjectionMatrix;
    uniform vec3 WCameraPosition;
};
layout (std140, binding = 1) uniform TransformData
{
    uniform mat4 ModelMatrix;
    uniform mat4 NormalMatrix;
};

// output
out VS_OUT
{
    out vec2 TexCoord;
    out vec4 VertexColor;
} vs_out;

void main()
{
    // pass texture coordinates
    vs_out.TexCoord = UV;
    // pass vertex Color
    vs_out.VertexColor = Color;

    // vertex position in view coordinates
    vec3 ViewPosition = (ViewMatrix * ModelMatrix * vec4(Position, 1.0)).xyz;

    // transformed vertex position
    gl_Position = ProjectionMatrix * vec4(ViewPosition, 1.0);
}
