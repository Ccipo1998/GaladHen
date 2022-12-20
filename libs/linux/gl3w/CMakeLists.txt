cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

project(gl3w)

find_package(OpenGL REQUIRED)

add_library(gl3w STATIC 
  src/gl3w.c
)

target_link_libraries(gl3w PUBLIC ${OPENGL_gl_LIBRARY})
target_include_directories(gl3w PUBLIC . src include)

add_library(gl3w::gl3w ALIAS gl3w)

if(UNIX AND NOT APPLE)
target_link_libraries(gl3w PUBLIC dl)
endif(UNIX AND NOT APPLE)