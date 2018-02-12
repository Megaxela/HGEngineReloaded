cmake_minimum_required(VERSION 3.9)
project(HGRenderingBase)

set(CMAKE_CXX_STANDARD 17)

add_library(HGRenderingBase STATIC
    include/Camera.hpp
    src/Camera.cpp
)

target_include_directories(HGRenderingBase PUBLIC
        include
)

target_link_libraries(HGRenderingBase
        HGCore
        HGUtils
)

target_compile_definitions(HGRenderingBase
        PUBLIC -DBASE_RENDERING_MODULE_NS=HG::Rendering::Base
)