
cmake_minimum_required(VERSION 3.5.0)

project(LayerAPI VERSION 0.1.0)

add_library(LayerAPI STATIC
    WindowGL.cpp
)

target_include_directories(LayerAPI PRIVATE
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include
    ${CMAKE_SOURCE_DIR})

