
project(GaladHenApp VERSION 0.1.0)

add_executable(GaladHenApp
    ${CMAKE_SOURCE_DIR}/App/GaladHen.cpp)

target_include_directories(GaladHenApp PRIVATE
    ${CMAKE_SOURCE_DIR}/GaladHen/
    ${CMAKE_SOURCE_DIR}/libs/assimp/include
    ${CMAKE_SOURCE_DIR}/libs/gl3w/include/
    ${CMAKE_SOURCE_DIR}/libs/glfw/include/
    ${CMAKE_SOURCE_DIR}/libs/glm/
    ${CMAKE_SOURCE_DIR}/libs/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui/
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui/backends/
    ${CMAKE_SOURCE_DIR}/utils/include/
    ${CMAKE_SOURCE_DIR}/include/)

set_target_properties(GaladHenApp
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_link_libraries(GaladHenApp
    PRIVATE
    GaladHenLib
    assimp
    glfw
    gl3w
    glm
    imgui
    Utils)
