
// gamma correction include needed

vec4 SRGBToLinearSpace(vec4 srgbColor)
{
    return pow(srgbColor, vec4(gamma));
}
