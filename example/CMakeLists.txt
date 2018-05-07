cmake_minimum_required(VERSION 3.9)
project(Example1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Example1
        main.cpp
)

target_include_directories(Example1 PRIVATE
        include
)

target_link_libraries(Example1
    HGCore
    HGUtils
    HGRenderingBase
    HGRenderingOGL
    HGStandard
)