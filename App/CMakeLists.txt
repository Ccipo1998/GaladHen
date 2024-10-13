
project(GaladHenApp VERSION 0.1.0)

add_executable(GaladHenApp
    GaladHen.cpp)


target_include_directories(GaladHenApp PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/assimp/include
    ${CMAKE_SOURCE_DIR}/Utils/)

set_target_properties(GaladHenApp
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

target_link_libraries(GaladHenApp
    PRIVATE
    Editor)
