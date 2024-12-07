
cmake_minimum_required(VERSION 3.5.0)

project(Renderer VERSION 0.1.0)

add_library(Renderer STATIC
    Renderer.cpp
    Renderer.h
    Window.h
    Window.cpp
    LayerAPI/IWindowAPI.h
    LayerAPI/ITextureAPI.h
    LayerAPI/IShaderProgramAPI.h
    LayerAPI/IMeshAPI.h
    LayerAPI/IMaterialDataAPI.h
    LayerAPI/OpenGL/WindowGL.cpp
    LayerAPI/OpenGL/WindowGL.h
    LayerAPI/OpenGL/MeshGL.cpp
    LayerAPI/OpenGL/MeshGL.h
    LayerAPI/OpenGL/ShaderProgramGL.cpp
    LayerAPI/OpenGL/ShaderProgramGL.h
    LayerAPI/OpenGL/MaterialDataGL.h
    LayerAPI/OpenGL/TextureGL.cpp
    LayerAPI/OpenGL/TextureGL.h
    LayerAPI/IRendererAPI.h
    LayerAPI/OpenGL/RendererGL.h
    LayerAPI/OpenGL/RendererGL.cpp
    Common.h
    LayerAPI/OpenGL/ShaderData.h
    LayerAPI/OpenGL/FrameBufferGL.h
    LayerAPI/OpenGL/FrameBufferGL.cpp
    LayerAPI/OpenGL/RenderBufferGL.h
    LayerAPI/OpenGL/RenderBufferGL.cpp
    LayerAPI/OpenGL/TextureFrameBufferGL.h
    LayerAPI/OpenGL/TextureFrameBufferGL.cpp
    RenderBuffer.h
    LayerAPI/IRenderBufferAPI.h
    RenderBuffer.cpp
    CommandBuffer.h
    ShaderData.h
    MeshData.h
    BufferData.h
    TextureData.h)

target_include_directories(Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/Libs/imgui/)

target_link_libraries(Renderer
    PRIVATE
    GaladHenCore
    glfw
    gl3w
    glm
    imgui
    Utils)
