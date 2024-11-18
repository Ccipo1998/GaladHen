
cmake_minimum_required(VERSION 3.5.0)

project(Editor VERSION 0.1.0)

add_library(Editor STATIC
    UI/Page.cpp
    UI/Widgets/NavBarWidget.h
    UI/Widgets/NavBarWidget.cpp
    UI/Widget.h
    UI/Widget.cpp
    UI/Pages/MainPage.h
    UI/Pages/MainPage.cpp
    Editor.h
    Editor.cpp
    UI/Widgets/AssetsViewWidget.h
    UI/Widgets/AssetsViewWidget.cpp
    UI/Widgets/ViewportWidget.h
    UI/Widgets/ViewportWidget.cpp
    UI/Widgets/InspectorWidget.h
    UI/Widgets/InspectorWidget.cpp  )

target_include_directories(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/Core/
    ${CMAKE_SOURCE_DIR}/Renderer/
    ${CMAKE_SOURCE_DIR}/Libs/
    ${CMAKE_SOURCE_DIR}/Libs/imgui)

target_link_libraries(Editor
    PRIVATE
    GaladHenCore
    Renderer
    imgui
    glm
    Utils)
