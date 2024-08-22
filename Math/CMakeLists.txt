cmake_minimum_required(VERSION 3.5.0)

project(MathLib VERSION 0.1.0)

add_library(MathLib STATIC
    Math.h
    Math.cpp
)

target_include_directories(MathLib PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/gl3w/include/)

target_link_libraries(MathLib
    PRIVATE
    GaladHenCore
    glm)