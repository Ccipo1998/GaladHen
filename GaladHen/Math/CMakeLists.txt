cmake_minimum_required(VERSION 3.5.0)

project(Math VERSION 0.1.0)

add_library(Math STATIC
    Math.h
    Math.cpp
    Ray.h
    Transform.h
    Transform.cpp
    BVH/BVH.h
    BVH/BVH.cpp
    BVH/BVHNode.h
    AABB/AABB.h
    AABB/AABB.cpp)

target_include_directories(Math PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/GaladHen/
    ${CMAKE_SOURCE_DIR}/Libs/)

target_link_libraries(Math
    PRIVATE
    Systems
    glm)