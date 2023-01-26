// Test fragment shader

#version 450 core

layout (location = 0) out vec4 fColor; // layout qualifier + shader variable (convention: prefixing a fragment attribute with the letter 'f')

void main()
{
    fColor = vec4(1.0, 0.0, 0.0, 1.0); // colors represented in RGBA color space
}
