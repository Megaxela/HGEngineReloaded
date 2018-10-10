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
    src/Loaders/StringLoader.cpp
    include/Loaders/StringLoader.hpp
    src/Loaders/AssimpLoader.cpp
    include/Loaders/AssimpLoader.hpp
    src/Model.cpp
    include/Model.hpp
    include/FutureHandler.hpp
)

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
    nlohmann_json
    assimp
    ${ZLIB_LIBRARIES}
)

target_compile_definitions(HGUtils PUBLIC
    -DGLM_ENABLE_EXPERIMENTAL
    HG_HAS_UTILS=1
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGUtils
        PRIVATE
            -ansi
            -pedantic
            -Wall
            -Wextra
            -Woverloaded-virtual
            -Winit-self
            -Wunreachable-code
    )
endif()