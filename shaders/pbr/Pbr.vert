
// PBR vertex shader

#version 450 core

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
layout (std140, binding = 1) uniform TransformData
{
    uniform mat4 ModelMatrix;
    uniform mat4 NormalMatrix;
};

// output
out VS_OUT
{
    out vec3 SmoothWNormal;
    flat out vec3 FlatWNormal;
    out vec3 WPosition;
    out vec3 WViewDirection;
    out vec2 TexCoord;
    out mat3 TBN;
} vs_out;

void main()
{
    // smooth and flat normals in world coordinates
    vec3 transNormal = (NormalMatrix * vec4(Normal, 1.0)).xyz;
    vs_out.SmoothWNormal = transNormal;
    vs_out.FlatWNormal = transNormal;

    // vertex position in view coordinates
    vec3 ViewPosition = (ViewMatrix * ModelMatrix * vec4(Position, 1.0)).xyz;

    // point of view direction in world coords
    vs_out.WViewDirection = normalize(WCameraPosition - Position);
    // vertex position in world coords
    vs_out.WPosition = Position;

    // pass texture coordinates
    vs_out.TexCoord = UV;

    // TBN matrix for normal mapping
    vec3 adjTangent = normalize(Tangent - dot(Tangent, transNormal) * transNormal); // re-orthogonalize tangent with respect to normal to ensure tangents are orthogonal when calculated (possible smoothing)
    vs_out.TBN = mat3(adjTangent, normalize(cross(transNormal, adjTangent)), transNormal);

    // transformed vertex position
    gl_Position = ProjectionMatrix * vec4(ViewPosition, 1.0);
}
