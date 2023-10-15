cmake_minimum_required(VERSION 3.5.0)

project(utils VERSION 0.1.0)

add_library(utils STATIC
${CMAKE_SOURCE_DIR}/utils/src/dblog.cpp)

target_include_directories(utils PRIVATE
${CMAKE_SOURCE_DIR}/utils/include/)
