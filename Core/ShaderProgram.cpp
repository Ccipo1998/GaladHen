
#include "ShaderProgram.h"

namespace GaladHen
{
    ShaderProgram::ShaderProgram()
        : ShaderProgramID(0)
    {}

    ShaderPipeline::ShaderPipeline()
        : VertexShader(nullptr)
        , TessContShader(nullptr)
        , TessEvalShader(nullptr)
        , GeometryShader(nullptr)
        , FragmentShader(nullptr)
        {}

    ShaderPipeline::ShaderPipeline(Shader* vertex, Shader* tessCont, Shader* tessEval, Shader* geometry, Shader* fragment)
        : VertexShader(vertex)
        , TessContShader(tessCont)
        , TessEvalShader(tessEval)
        , GeometryShader(geometry)
        , FragmentShader(fragment)
        {}

    ComputeShader::ComputeShader()
        : CompShader(nullptr)
    {}

    ComputeShader::ComputeShader(Shader* compute)
        : CompShader(compute)
    {}
}
