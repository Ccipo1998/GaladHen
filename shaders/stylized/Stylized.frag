// Fragment shader for stylized shading model (Gooch + highlight)

#version 450 core

layout (location = 0) out vec4 fColor;

in vec3 viewNormal;
in vec3 viewLightDir;
in vec3 viewViewDir;

void main()
{
    //fColor = vec4(abs(viewNormal), 1.0);

    vec3 cool = vec3(0.0, 0.0, 0.55) + 0.25;
    vec3 warm = vec3(0.3, 0.3, 0.0) + 0.25;
    vec3 highlight = vec3(1.0, 1.0, 1.0);

    float t = (dot(viewNormal, viewLightDir) + 1) / 2;
    vec3 r = 2 * dot(viewNormal, viewLightDir) * viewNormal - viewLightDir;
    float s = clamp((100 * dot(r, viewViewDir) - 97), 0, 1);

    vec3 shadedColor = s * highlight + (1 - s) * (t * warm + (1 - t) * cool);
    fColor = vec4(shadedColor, 1.0);
}
