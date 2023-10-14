cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(ezengine VERSION 0.1.0)

add_executable(ezengine main.cpp)
target_include_directories(ezengine PUBLIC
    ${CMAKE_SOURCE_DIR}/libs/assimp/include
    ${CMAKE_SOURCE_DIR}/libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/
    ${CMAKE_SOURCE_DIR}/include/)
set_target_properties(ezengine
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# initially those were for assimp
#set(BUILD_SHARED_LIBS OFF)
#set(INJECT_DEBUG_POSTFIX OFF)
#set(ASSIMP_INSTALL OFF)
#set(ASSIMP_BUILD_TESTS OFF)

add_subdirectory(libs/assimp)
add_subdirectory(libs/gl3w)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(libs/imgui)
target_link_libraries(ezengine
    PUBLIC
    assimp
    glfw
    gl3w
    glm
    imgui)
