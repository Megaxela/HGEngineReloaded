cmake_minimum_required(VERSION 3.9)
project(HGUtils)

set(CMAKE_CXX_STANDARD 17)

option(HG_BUILD_UTILS_TESTS "Build core tests" Off)

if (${HG_BUILD_UTILS_TESTS})
    add_subdirectory(tests)
endif()

add_subdirectory(libraries)

find_package(ZLIB REQUIRED)

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
    include/base64.hpp
    include/zlib.hpp
    src/PhysicalResource.cpp
    include/PhysicalResource.hpp
    include/Platform.hpp
    src/StringTools.cpp
    include/StringTools.hpp
        src/Loaders/StringLoader.cpp include/Loaders/StringLoader.hpp)

target_include_directories(HGUtils PUBLIC
    include
    ${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(HGUtils
    ALogger
    glm
    stb
    rapidxml
    bytearray
    stdc++fs
    STLRoundBuffer
    ${ZLIB_LIBRARIES}
)

target_compile_definitions(HGUtils
        PUBLIC -DUTILS_MODULE_NS=HG::Utils
        PUBLIC -DGLM_ENABLE_EXPERIMENTAL
)