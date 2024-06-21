
#pragma once

// This class represents a complete rendering pipeline

namespace GaladHen
{
    class Shader;

    class ShaderProgram
    {

    public:

        ShaderProgram();

        ShaderProgram(Shader* vertex, Shader* tesselation, Shader* geometry, Shader* fragment);

        Shader* VertexShader;
        Shader* TesselationShader;
        Shader* GeometryShader;
        Shader* FragmentShader;

    protected:

        unsigned int ShaderProgramID; // 0 means -1, so the low level pair is not already been created
    };
}
