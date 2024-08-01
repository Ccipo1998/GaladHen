
project(GaladHenCore VERSION 0.1.0)

add_library(GaladHenCore STATIC
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
    Texture.cpp
    Texture.h
    AssetsManager.cpp
    AssetsManager.h
    MaterialData.h
    MeshData.h
    TextureData.h
    SceneData.h
    FileLoader.h
    FileLoader.cpp)

target_include_directories(GaladHenCore PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Utils/
    ${CMAKE_SOURCE_DIR}/Libs/assimp/include/)

target_link_libraries(GaladHenCore
    PRIVATE
    glm
    assimp
    Utils)
