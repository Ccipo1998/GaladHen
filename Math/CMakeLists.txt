cmake_minimum_required(VERSION 3.5.0)

project(GaladHenMath VERSION 0.1.0)

add_library(GaladHenMath STATIC
    Math.h
    Math.cpp
    Ray.h
)

target_include_directories(GaladHenMath PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/)

target_link_libraries(GaladHenMath
    PRIVATE
    GaladHenCore
    glm)