
// Vertex shader for rendering

#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;

// uniforms
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

// output
out vec3 SmoothViewNormal;
flat out vec3 FlatViewNormal;
out vec3 ViewPosition;
out vec3 ViewDirection;
out vec2 TexCoord;

void main()
{
    SmoothViewNormal = normalize((NormalMatrix * vec4(Normal, 1.0)).xyz);
    FlatViewNormal = normalize((NormalMatrix * vec4(Normal, 1.0)).xyz);

    // light position and vertex position in view coords
    ViewPosition = (ViewMatrix * vec4(Position, 1.0)).xyz;

    // point of view direction in view coords
    ViewDirection = normalize(-ViewPosition);

    // pass texture coordinates
    TexCoord = UV;

    // transformed vertex position
    gl_Position = ProjectionMatrix * vec4(ViewPosition, 1.0);
}
