cmake_minimum_required(VERSION 3.0.0)

project(imgui)

#find_package(OpenGL REQUIRED)

add_library(imgui STATIC 
  src/imgui_demo.cpp
  src/imgui_draw.cpp
  src/imgui_impl_glfw.cpp
  src/imgui_impl_opengl3.cpp
  src/imgui_tables.cpp
  src/imgui_widgets.cpp
  src/imgui.cpp
)

target_include_directories(imgui PUBLIC . src include ../glfw/include)

add_library(gl3w::gl3w ALIAS gl3w)

