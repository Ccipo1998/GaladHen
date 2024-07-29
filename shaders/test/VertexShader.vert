// Test vertex shader

#version 450 core

layout (location = 0) in vec3 Position;

layout (std140, binding = 0) uniform CameraData
{
    uniform mat4 ViewMatrix;
    uniform mat4 ProjectionMatrix;
    uniform vec3 WCameraPosition;
};

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(Position, 1.0);
}
