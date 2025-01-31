
cmake_minimum_required(VERSION 3.5.0)

project(Renderer VERSION 0.1.0)

add_library(Renderer STATIC
    Renderer.cpp
    Renderer.h
    Window.h
    Window.cpp
    LayerAPI/IWindowAPI.h
    LayerAPI/OpenGL/WindowGL.cpp
    LayerAPI/OpenGL/WindowGL.h
    LayerAPI/IRendererAPI.h
    LayerAPI/OpenGL/RendererGL.h
    LayerAPI/OpenGL/RendererGL.cpp
    Common.h
    Entities/RenderBuffer.h
    Entities/RenderBuffer.cpp
    CommandBuffer.h
    Entities/Material.h
    Entities/Texture.h
    Entities/Texture.cpp
    Entities/Buffer.hpp
    Entities/IGPUResource.h
    Entities/IGPUResource.cpp
    Entities/Material.cpp
    Entities/ShaderPipeline.h
    Entities/ShaderPipeline.cpp
    Entities/Mesh.h
    Entities/Mesh.cpp
    GPUResourceInspector.h
    GPUResourceInspector.cpp
    Entities/Camera.h
    Entities/Camera.cpp
    Entities/Scene.h
    Entities/SceneObject.h
    Entities/SceneObject.cpp
    Entities/Light.h
    Entities/Light.cpp
    Entities/PointLight.h
    Entities/PointLight.cpp
    Entities/DirectionalLight.h
    Entities/DirectionalLight.cpp
    Entities/SpotLight.h
    Entities/SpotLight.cpp
    Entities/Scene.cpp
    Entities/Model.cpp
    Entities/BufferData/CameraBufferData.h
    Entities/BufferData/TransformBufferData.h "ShaderPreprocessor/ShaderPreprocessor.h" "ShaderPreprocessor/ShaderPreprocessor.cpp" "Entities/BufferData/PointLightBufferData.h" "Entities/BufferData/DirLightBufferData.h" "Entities/BufferData/LightingBufferData.h")

target_include_directories(Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/GaladHen/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/Libs/imgui/)

target_link_libraries(Renderer
    PRIVATE
    glfw
    gl3w
    glm
    Math
    Utils)
