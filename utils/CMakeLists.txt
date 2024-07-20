cmake_minimum_required(VERSION 3.5.0)

project(Utils VERSION 0.1.0)

add_library(Utils STATIC
    log.cpp
    stb_image.cpp
    singleton.hpp
    FileLoader.cpp
)

target_include_directories(Utils PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/assimp/include/)

target_link_libraries(Utils
    PRIVATE
    assimp)
