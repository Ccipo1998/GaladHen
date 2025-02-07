cmake_minimum_required(VERSION 3.5.0)

project(Utils VERSION 0.1.0)

add_library(Utils STATIC
    Log.cpp
    stb_image.cpp
    Singleton.hpp
    IdList.hpp
    FileLoader.h
    FileLoader.cpp
    WeakSingleton.hpp)

target_include_directories(Utils PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/GaladHen/)
