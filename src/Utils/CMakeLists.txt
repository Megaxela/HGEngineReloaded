cmake_minimum_required(VERSION 3.9)
project(HGUtils)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libraries)

add_library(HGUtils STATIC
        include/DoubleBufferContainer.hpp
        src/DoubleBufferContainer.cpp
        include/Rect.hpp
        src/Rect.cpp
        include/Color.hpp
        src/Color.cpp
        include/Vertex.hpp
        src/Vertex.cpp
        include/Mesh.hpp
        src/Mesh.cpp)

target_include_directories(HGUtils PUBLIC
        include
)

target_link_libraries(HGUtils
        ALogger
        glm
)

target_compile_definitions(HGUtils
        PUBLIC -DUTILS_MODULE_NS=HG::Utils
        PUBLIC -DGLM_ENABLE_EXPERIMENTAL
)