/*
Fragment shader for physically based rendering with GGX approximations
*/

#version 450 core

layout (location = 0) out vec4 fColor;

in vec3 fNormal;
in vec3 viewLightDir;
in vec3 viewViewDir;

uniform vec3 DiffuseColor;
uniform vec3 Specular;
uniform float Metallic;
uniform float Roughness;

const float pi = 3.141592653589793;
const float e = 0.0001; // tiny value to avoid dividing per zero

vec3 FresnelSchlick(vec3 F0, vec3 half_vector, vec3 view_dir)
{
    float cosTheta = max(dot(half_vector, view_dir), 0.0);
    return F0 + (vec3(1.0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float NormalDistributionGGX(vec3 normal, vec3 half_vector, float roughness)
{
    float numerator = roughness * roughness;
    float dot2 = max(dot(normal, half_vector), 0.0);
    dot2 *= dot2;
    float denominator = (dot2 * (numerator - 1.0) + 1.0);
    denominator *= denominator * pi;

    return numerator / denominator;
}

float SmithMaskingGGX(float lightORview_dot_normal, float roughness)
{
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;

    float num   = lightORview_dot_normal;
    float denom = lightORview_dot_normal * (1.0 - k) + k;
	
    return num / denom;
}

float MaskingShadowingGeometryGGX(vec3 normal, vec3 light_dir, vec3 view_dir, float roughness)
{
    float ndotl = max(dot(normal, light_dir), 0.0);
    float ndotv = max(dot(normal, view_dir), 0.0);
    
    return SmithMaskingGGX(ndotl, roughness) * SmithMaskingGGX(ndotv, roughness);
}

void main()
{
    vec3 half_vector = normalize(viewLightDir + viewViewDir);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, DiffuseColor, Metallic);

    // specular term
    vec3 F = FresnelSchlick(F0, half_vector, viewViewDir);
    vec3 spec_numerator = NormalDistributionGGX(fNormal, half_vector, Roughness) * MaskingShadowingGeometryGGX(fNormal, viewLightDir, viewViewDir, Roughness) * F;
    float spec_denominator = 4.0 * max(dot(fNormal, viewLightDir), 0.0) * max(dot(fNormal, viewViewDir), 0.0) + e;
    vec3 specular = spec_numerator / spec_denominator;

    // diffuse term balanced by conservation of energy law and by the material (metallic or not)
    vec3 diffuse = (vec3(1.0) - F) * (1.0 - Metallic) * DiffuseColor / pi;

    vec3 lightColor = vec3(1.0);
    vec3 L0 = (diffuse + specular) * lightColor * max(dot(fNormal, viewLightDir), 0.0);

    // gamma correction
    vec3 color = L0;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    //fColor = vec4(1.0, 1.0, 1.0, 1.0);
    fColor = vec4(color, 1.0);
}
