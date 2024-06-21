
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
}
