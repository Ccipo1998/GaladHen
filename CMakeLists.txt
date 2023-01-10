cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(ezengine VERSION 0.1.0)

add_executable(ezengine main.cpp)
target_include_directories(ezengine PUBLIC
    "${CMAKE_SOURCE_DIR}/libs/linux/glfw/include/"
    "${CMAKE_SOURCE_DIR}/libs/linux/gl3w/include/"
    "${CMAKE_SOURCE_DIR}/include/")
set_target_properties(ezengine
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# initially those were for assimp
#set(BUILD_SHARED_LIBS OFF)
#set(INJECT_DEBUG_POSTFIX OFF)
#set(ASSIMP_INSTALL OFF)
#set(ASSIMP_BUILD_TESTS OFF)

add_subdirectory("libs/linux/glfw")
add_subdirectory("libs/linux/gl3w")
add_subdirectory("include/glm")
target_link_libraries(ezengine
    PUBLIC
    glfw
    gl3w
    glm
    "${CMAKE_SOURCE_DIR}/libs/linux/assimp/libassimp.so")
