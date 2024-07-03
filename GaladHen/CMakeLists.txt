
project(GaladHenLib VERSION 0.1.0)

add_library(GaladHenLib STATIC
    #ui.cpp
    Input.cpp
    Input.h
    Scene.cpp
    Scene.h
    SceneObject.cpp
    SceneObject.h
    Material.cpp
    Material.h
    Camera.cpp
    Camera.h
    Transform.cpp
    Transform.h
    Shader.cpp
    Shader.h
    ShaderProgram.cpp
    ShaderProgram.h
    Light.cpp
    Light.h
    Model.cpp
    Model.h
    Mesh.cpp
    Mesh.h
    TextureImage.cpp
    TextureImage.h
    Texture.cpp
    Texture.h
    AssetsManager.cpp
    AssetsManager.h
    MaterialData.h
    MeshData.h
    TextureData.h)

target_include_directories(GaladHenLib PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/assimp/include/
    ${CMAKE_SOURCE_DIR}/Utils/
    ${CMAKE_SOURCE_DIR}/Renderer/LayerAPI/)

target_link_libraries(GaladHenLib
    PRIVATE
    assimp
    glfw
    gl3w
    glm
    Utils)
