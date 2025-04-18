
// Shadow depth vertex shader

layout (location = 0) in vec3 Position;

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

void main()
{
    // vertex position in view coordinates
    vec3 ViewPosition = (ViewMatrix * ModelMatrix * vec4(Position, 1.0)).xyz;

    // transformed vertex position
    gl_Position = ProjectionMatrix * vec4(ViewPosition, 1.0);
}
