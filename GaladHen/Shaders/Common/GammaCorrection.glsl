
// const
const float gamma = 2.2;

uniform bool GammaCorrectionEnabled = true;

bool IsGammaCorrectionActive()
{
	return GammaCorrectionEnabled;
}

vec3 GammaCorrection(vec3 shading)
{
    vec3 newShading = shading / (shading + vec3(1.0));
    newShading = pow(newShading, vec3(1.0/gamma));

    return newShading;
}
