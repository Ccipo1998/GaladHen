
// PBR vertex shader

// TODO: add model matrix

#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

// uniforms

layout (std140, binding = 0) uniform CameraData
{
    uniform mat4 ViewMatrix;
    uniform mat4 ProjectionMatrix;
    uniform vec3 WCameraPosition;
};

layout (std140, binding = 1) uniform ModelData
{
    uniform mat4 NormalMatrix;
};


// output
out vec3 SmoothWNormal;
flat out vec3 FlatWNormal;
out vec3 WPosition;
out vec3 WViewDirection;
out vec2 TexCoord;
out mat3 TBN;

void main()
{
    // smooth and flat normals in world coordinates
    SmoothWNormal = Normal;
    FlatWNormal = Normal;

    // vertex position in view coordinates
    vec3 ViewPosition = (ViewMatrix * vec4(Position, 1.0)).xyz;

    // point of view direction in world coords
    WViewDirection = normalize(WCameraPosition - Position);
    // vertex position in world coords
    WPosition = Position;

    // pass texture coordinates
    TexCoord = UV;

    // TBN matrix for normal mapping
    vec3 adjTangent = normalize(Tangent - dot(Tangent, Normal) * Normal); // re-orthogonalize tangent with respect to normal to ensure tangents are orthogonal when calculated (possible smoothing)
    TBN = mat3(adjTangent, normalize(cross(Normal, adjTangent)), Normal);

    // transformed vertex position
    gl_Position = ProjectionMatrix * vec4(ViewPosition, 1.0);
}
