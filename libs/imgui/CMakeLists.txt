cmake_minimum_required(VERSION 3.5.0)

project(imgui)

add_library(imgui SHARED 
  imgui_demo.cpp
  imgui_draw.cpp
  backends/imgui_impl_glfw.cpp
  backends/imgui_impl_opengl3.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui.cpp
)

target_include_directories(imgui PUBLIC
  ${CMAKE_SOURCE_DIR}/libs/imgui/
  ${CMAKE_SOURCE_DIR}/libs/imgui/backends/
  ${CMAKE_SOURCE_DIR}/libs/glfw/include/
)

target_link_libraries(imgui PUBLIC
    glfw)
