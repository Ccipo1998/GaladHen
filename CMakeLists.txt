cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(GaladHen VERSION 0.1.0)

# set global variable to override runtime output directory of all the libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)

add_subdirectory(GaladHen)
add_subdirectory(Renderer)
add_subdirectory(libs/assimp)
add_subdirectory(libs/gl3w)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(libs/imgui)
add_subdirectory(Utils)
add_subdirectory(App)