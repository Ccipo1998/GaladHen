
#include "ShaderProgram.h"

namespace GaladHen
{
    ShaderProgram::ShaderProgram()
        : VertexShader(nullptr)
        , TesselationShader(nullptr)
        , GeometryShader(nullptr)
        , FragmentShader(nullptr)
        , ShaderProgramID(0)
        {}

    ShaderProgram::ShaderProgram(Shader* vertex, Shader* tesselation, Shader* geometry, Shader* fragment)
        : VertexShader(vertex)
        , TesselationShader(tesselation)
        , GeometryShader(geometry)
        , FragmentShader(fragment)
        {}
}
