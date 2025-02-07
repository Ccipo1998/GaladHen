#pragma once

#define GH_CURRENT_API API::OpenGL

#define GH_DEFAULT_WINDOW_WIDTH 1280
#define GH_DEFAULT_WINDOW_HEIGHT 720
#define GH_DEFAULT_WINDOW_NAME "GaladHen"

#define GH_GLSL_VERSION "#version 450 core"
#define GH_GLSL_VERSION_MAJOR 4
#define GH_GLSL_VERSION_MINOR 5

namespace GaladHen
{
    enum class API
    {
        OpenGL = 0
    };
}
