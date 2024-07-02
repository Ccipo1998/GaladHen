
cmake_minimum_required(VERSION 3.5.0)

project(LayerOpenGL VERSION 0.1.0)

add_library(LayerOpenGL STATIC
    WindowGL.cpp
    WindowGL.h
    MeshGL.cpp
    MeshGL.h
    ShaderProgramGL.cpp
    ShaderProgramGL.h
    MaterialDataGL.h
    TextureGL.cpp
    TextureGL.h
)

target_include_directories(LayerOpenGL PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/Libs/gl3w/include/)

