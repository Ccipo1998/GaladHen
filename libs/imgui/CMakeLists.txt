cmake_minimum_required(VERSION 3.5.0)

project(imgui VERSION 0.1.0)

add_library(imgui STATIC 
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_win32.cpp
)

# include directories inside imgui sources files
target_include_directories(imgui PRIVATE
  ${CMAKE_SOURCE_DIR}/Libs/glfw/include/
  ${CMAKE_SOURCE_DIR}/Libs/imgui/
)

target_link_libraries(imgui PUBLIC
    glfw)
