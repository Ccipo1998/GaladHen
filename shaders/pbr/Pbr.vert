
// PBR vertex shader

// TODO: add model matrix

#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

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
out mat3 TBN;

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

    // calculate TBN matrix
    vec3 testNormal = normalize((NormalMatrix * vec4(Normal, 1.0)).xyz);
    vec3 testTangent = normalize((NormalMatrix * vec4(Tangent, 1.0)).xyz);
    vec3 testBitangent = normalize((NormalMatrix * vec4(Bitangent, 1.0)).xyz);
    TBN = mat3(testTangent, testBitangent, testNormal);

    // transformed vertex position
    gl_Position = ProjectionMatrix * vec4(ViewPosition, 1.0);
}
