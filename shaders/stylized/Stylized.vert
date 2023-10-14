// Vertex shader for a stylized shading model (Gooch + highlight)

#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightDir;

out vec3 viewNormal;
out vec3 viewLightDir;
out vec3 viewViewDir;

void main()
{
    viewNormal = normalize(vec4(vNormal, 1.0).xyz);
    //vec3 viewLightPos = (viewMatrix * vec4(lightPos, 1.0)).xyz;
    vec3 viewPosition = (viewMatrix * vec4(vPosition, 1.0)).xyz;
    viewLightDir = normalize((viewMatrix * vec4(lightDir, 1.0)).xyz);
    viewLightDir = -viewLightDir;
    viewViewDir = normalize(-viewPosition);

    gl_Position = projectionMatrix * viewMatrix * vec4(vPosition, 1.0);
}
