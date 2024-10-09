
cmake_minimum_required(VERSION 3.5.0)

project(UserInterface VERSION 0.1.0)

add_library(UserInterface STATIC
    Page.cpp)

target_include_directories(UserInterface PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Core/
    ${CMAKE_SOURCE_DIR}/Renderer/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/imgui)

target_link_libraries(UserInterface
    PRIVATE
    Renderer
    imgui
    Utils)
