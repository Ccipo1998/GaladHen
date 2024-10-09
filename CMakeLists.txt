cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(GaladHen VERSION 0.1.0)

# set global variable to override runtime output directory of all the libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)

add_subdirectory(Core)
add_subdirectory(Renderer)
add_subdirectory(Libs/assimp)
add_subdirectory(Libs/gl3w)
add_subdirectory(Libs/glfw)
add_subdirectory(Libs/glm)
add_subdirectory(Libs/imgui)
add_subdirectory(Utils)
add_subdirectory(Math)
add_subdirectory(App)
add_subdirectory(UI)