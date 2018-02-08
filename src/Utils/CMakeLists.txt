cmake_minimum_required(VERSION 3.9)
project(HGUtils)

set(CMAKE_CXX_STANDARD 17)

add_library(HGUtils STATIC
        include/UniqueMergeContainer.hpp
        src/UniqueMergeContainer.cpp)

target_include_directories(HGUtils PUBLIC
        include
        )

target_compile_definitions(HGUtils
        PUBLIC -DUTILS_MODULE_NS=HG::Utils
)