// Vertex shader for a stylized shading model (Gooch + highlight)

#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPos;

out vec3 viewNormal;
out vec3 viewLightDir;
out vec3 viewViewDir;

void main()
{
    viewNormal = normalize(vec4(vNormal, 1.0).xyz);
    vec3 viewLightPos = (viewMatrix * vec4(lightPos, 1.0)).xyz;
    vec3 viewPosition = (viewMatrix * vec4(vPosition, 1.0)).xyz;
    viewLightDir = normalize(viewLightPos - viewPosition);
    viewViewDir = normalize(-viewPosition);

    gl_Position = projectionMatrix * viewMatrix * vec4(vPosition, 1.0);
}
