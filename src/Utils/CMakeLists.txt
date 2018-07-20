cmake_minimum_required(VERSION 3.9)
project(HGUtils)

set(CMAKE_CXX_STANDARD 17)

option(HG_BUILD_UTILS_TESTS "Build core tests" Off)

if (${HG_BUILD_UTILS_TESTS})
    message(STATUS "No tests currently available for Utils module")
endif()

add_subdirectory(libraries)

add_library(HGUtils STATIC
        include/DoubleBufferContainer.hpp
        include/Rect.hpp
        src/Rect.cpp
        include/Color.hpp
        src/Color.cpp
        include/Vertex.hpp
        src/Vertex.cpp
        include/Mesh.hpp
        src/Mesh.cpp
        include/glmex.hpp
        src/Surface.cpp
        include/Surface.hpp
        src/Loaders/STBImageLoader.cpp
        include/Loaders/STBImageLoader.hpp
)

target_include_directories(HGUtils PUBLIC
        include
)

target_link_libraries(HGUtils
        ALogger
        glm
        stb
        rapidxml
)

target_compile_definitions(HGUtils
        PUBLIC -DUTILS_MODULE_NS=HG::Utils
        PUBLIC -DGLM_ENABLE_EXPERIMENTAL
)