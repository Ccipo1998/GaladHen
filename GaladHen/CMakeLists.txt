
project(GaladHenLib VERSION 0.1.0)

add_library(GaladHenLib STATIC
    ui.cpp
    input.cpp
    scene.cpp
    sceneobject.cpp
    material.cpp
    camera.cpp
    transform.cpp
    shader.cpp
    light.cpp
    model.cpp
    mesh.cpp
    pbrmaterial.cpp
    phongmaterial.cpp
    textureimage.cpp
    texture.cpp
    assetsmanager.cpp
    Window.cpp)

target_include_directories(GaladHenLib PRIVATE
    include/
    ${CMAKE_SOURCE_DIR}/libs/assimp/include/
    ${CMAKE_SOURCE_DIR}/libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui/backends/
    ${CMAKE_SOURCE_DIR}/Utils/
    ${CMAKE_SOURCE_DIR}/Renderer/LayerAPI/)

target_link_libraries(GaladHenLib
    PRIVATE
    assimp
    glfw
    gl3w
    glm
    imgui
    Utils)
