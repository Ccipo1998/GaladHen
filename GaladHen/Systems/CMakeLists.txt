cmake_minimum_required(VERSION 3.5.0)

project(Systems VERSION 0.1.0)

add_library(Systems STATIC
    RenderingSystem/RenderingSystem.cpp
    RenderingSystem/RenderingSystem.h
    RenderingSystem/LayerAPI/IRendererAPI.h
    RenderingSystem/LayerAPI/OpenGL/RendererGL.h
    RenderingSystem/LayerAPI/OpenGL/RendererGL.cpp
    RenderingSystem/Common.h
    RenderingSystem/Entities/RenderBuffer.h
    RenderingSystem/Entities/RenderBuffer.cpp
    RenderingSystem/CommandBuffer.h
    RenderingSystem/Entities/Material.h
    RenderingSystem/Entities/Texture.h
    RenderingSystem/Entities/Texture.cpp
    RenderingSystem/Entities/Buffer.hpp
    RenderingSystem/Entities/IGPUResource.h
    RenderingSystem/Entities/IGPUResource.cpp
    RenderingSystem/Entities/Material.cpp
    RenderingSystem/Entities/ShaderPipeline.h
    RenderingSystem/Entities/ShaderPipeline.cpp
    RenderingSystem/Entities/Mesh.h
    RenderingSystem/Entities/Mesh.cpp
    RenderingSystem/GPUResourceInspector.h
    RenderingSystem/GPUResourceInspector.cpp
    RenderingSystem/Entities/Camera.h
    RenderingSystem/Entities/Camera.cpp
    RenderingSystem/Entities/Scene.h
    RenderingSystem/Entities/SceneObject.h
    RenderingSystem/Entities/SceneObject.cpp
    RenderingSystem/Entities/Light.h
    RenderingSystem/Entities/Light.cpp
    RenderingSystem/Entities/PointLight.h
    RenderingSystem/Entities/PointLight.cpp
    RenderingSystem/Entities/DirectionalLight.h
    RenderingSystem/Entities/DirectionalLight.cpp
    RenderingSystem/Entities/SpotLight.h
    RenderingSystem/Entities/SpotLight.cpp
    RenderingSystem/Entities/Scene.cpp
    RenderingSystem/Entities/Model.cpp
    RenderingSystem/Entities/BufferData/CameraBufferData.h
    RenderingSystem/Entities/BufferData/TransformBufferData.h
    RenderingSystem/ShaderPreprocessor/ShaderPreprocessor.h
    RenderingSystem/ShaderPreprocessor/ShaderPreprocessor.cpp
    RenderingSystem/Entities/BufferData/PointLightBufferData.h
    RenderingSystem/Entities/BufferData/DirLightBufferData.h
    RenderingSystem/Entities/BufferData/LightingBufferData.h
    RenderingSystem/UI/Page.h
    RenderingSystem/UI/Page.cpp
    RenderingSystem/UI/Widget.h
    RenderingSystem/UI/Widget.cpp
    SystemsCoordinator.h
    SystemsCoordinator.cpp
    InputSystem/InputSystem.h
    InputSystem/InputSystem.cpp
    InputSystem/Input.h
    InputSystem/LayerAPI/IInputProvider.h
    InputSystem/LayerAPI/OpenGL/InputProviderGL.h
    InputSystem/LayerAPI/OpenGL/InputProviderGL.cpp
)

target_include_directories(Systems PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/GaladHen/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/Libs/imgui/)

target_link_libraries(Systems
    PRIVATE
    glfw
    gl3w
    glm
    Math
    imgui
    Utils)
