
cmake_minimum_required(VERSION 3.5.0)

project(LayerOpenGL VERSION 0.1.0)

add_library(LayerOpenGL STATIC
    WindowGL.cpp
    MeshGL.cpp
    ShaderProgramGL.cpp
)

target_include_directories(LayerOpenGL PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/glm/
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/Libs/gl3w/include/)

