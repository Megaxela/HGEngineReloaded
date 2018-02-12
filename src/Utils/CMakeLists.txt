cmake_minimum_required(VERSION 3.9)
project(HGUtils)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libraries)

add_library(HGUtils STATIC
        include/UniqueMergeContainer.hpp
        src/UniqueMergeContainer.cpp
        include/Rect.hpp
        src/Rect.cpp)

target_include_directories(HGUtils PUBLIC
        include
)

target_link_libraries(HGUtils
        ALogger
        glm
)

target_compile_definitions(HGUtils
        PUBLIC -DUTILS_MODULE_NS=HG::Utils
)