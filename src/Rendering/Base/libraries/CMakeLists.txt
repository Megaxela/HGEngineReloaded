cmake_minimum_required(VERSION 3.9)

# ImGui cmake
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC imgui/)