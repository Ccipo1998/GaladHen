
// Shadow depth fragment shader
// Only set fragment depth (actually useless to do explicitly, cause it is already done automatically, but repetita iuvant)

void main()
{
    gl_FragDepth = gl_FragCoord.z;
}
