// Test vertex shader

#version 450 core // --> version of the GLSL version we want to use (ex: 450 = OpenGL version 4.5)

layout (location = 0) in vec4 vPosition; // layout qualifier + shader variable (convention: prefixing a vertex attribute with the letter 'v')

void main()
{
    gl_Position = vPosition;
}
