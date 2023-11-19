cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(ezengine VERSION 0.1.0)

# set global variable to override runtime output directory of all the libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(ezengine
${CMAKE_SOURCE_DIR}/src/ui.cpp
${CMAKE_SOURCE_DIR}/src/input.cpp
${CMAKE_SOURCE_DIR}/src/scene.cpp
main.cpp)
target_include_directories(ezengine PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/assimp/include
    ${CMAKE_SOURCE_DIR}/libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui/backends/
    ${CMAKE_SOURCE_DIR}/utils/include/
    ${CMAKE_SOURCE_DIR}/include/)
set_target_properties(ezengine
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory(libs/assimp)
add_subdirectory(libs/gl3w)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(libs/imgui)
add_subdirectory(utils)
target_link_libraries(ezengine
    PRIVATE
    assimp
    glfw
    gl3w
    glm
    imgui
    utils)
