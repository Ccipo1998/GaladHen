
cmake_minimum_required(VERSION 3.5.0)

project(Renderer VERSION 0.1.0)

add_library(Renderer STATIC
    Renderer.cpp
)

target_include_directories(Renderer PRIVATE
${CMAKE_SOURCE_DIR}/
${CMAKE_SOURCE_DIR}/Libs/)


add_subdirectory(LayerAPI)
