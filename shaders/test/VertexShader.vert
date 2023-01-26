// Test vertex shader

#version 450 core // --> version of the GLSL version we want to use (ex: 450 = OpenGL version 4.5)

layout (location = 0) in vec3 vPosition; // layout qualifier + shader variable (convention: prefixing a vertex attribute with the letter 'v')

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * vec4(vPosition, 1.0);
}
