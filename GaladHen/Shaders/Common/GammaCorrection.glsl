
// const
const float gamma = 2.2;

uniform bool GammaCorrectionEnabled = true;

bool IsGammaCorrectionActive()
{
	return GammaCorrectionEnabled;
}

vec4 GammaCorrection(vec4 shading)
{
    vec4 newShading = shading / (shading + vec4(1.0));
    newShading = pow(newShading, vec4(1.0/gamma));

    return newShading;
}
