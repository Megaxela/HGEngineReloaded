cmake_minimum_required(VERSION 3.10)
project(HGCore)

set(CMAKE_CXX_STANDARD 17)

add_library(HGCore STATIC
        src/Application.cpp
        include/Application.hpp
        src/Scene.cpp include/Scene.hpp)

target_include_directories(HGCore PUBLIC
        include
)

target_link_libraries(HGCore
        HGUtils # UniqueMergeContainer
)

target_compile_definitions(HGCore
        PUBLIC -DCORE_MODULE_NS=HG::Core
)