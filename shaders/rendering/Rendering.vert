
// Vertex shader for rendering

#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

// uniforms
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 LightPos;

// output
out vec3 ViewNormal;
out vec3 ViewLightDir;
out vec3 ViewDirection;

void main()
{
    // normal remains the same
    ViewNormal = Normal;

    // light position and vertex position in view coords
    vec4 viewLightPos = View * vec4(LightPos, 1.0);
    vec4 viewPosition = View * vec4(Position, 1.0);
    ViewLightDir = normalize((viewLightPos - viewPosition)).xyz;

    // point of view direction in view coords
    ViewDirection = -(viewPosition).xyz;

    // transformed vertex position
    gl_Position = Projection * viewPosition;
}
