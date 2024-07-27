
#pragma once

// These classes represent a complete shader pipeline and a compute shader program

namespace GaladHen
{
    class Shader;

    class ShaderProgram
    {

    public:

        ShaderProgram();

        unsigned int ShaderProgramID; // 0 means -1, so the low level pair is not already been created
    };

    class ShaderPipeline : public ShaderProgram
    {
    public:

        ShaderPipeline();

        ShaderPipeline(Shader* vertex, Shader* tessCont, Shader* tessEval, Shader* geometry, Shader* fragment);

        Shader* VertexShader;
        Shader* TessContShader;
        Shader* TessEvalShader;
        Shader* GeometryShader;
        Shader* FragmentShader;
    };

    class ComputeShader : public ShaderProgram
    {
    public:

        ComputeShader();

        ComputeShader(Shader* compute);

        Shader* CompShader;
    };
}
