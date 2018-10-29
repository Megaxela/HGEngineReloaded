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
        include/HG/Utils/DoubleBufferContainer.hpp
        include/HG/Utils/Rect.hpp
    src/Rect.cpp
        include/HG/Utils/Color.hpp
    src/Color.cpp
        include/HG/Utils/Vertex.hpp
    src/Vertex.cpp
        include/HG/Utils/Mesh.hpp
    src/Mesh.cpp
        include/HG/Utils/glmex.hpp
    src/Surface.cpp
        include/HG/Utils/Surface.hpp
    src/Loaders/STBImageLoader.cpp
        include/HG/Utils/Loaders/STBImageLoader.hpp
        include/HG/Utils/base64.hpp
        include/HG/Utils/zlib.hpp
    src/PhysicalResource.cpp
        include/HG/Utils/PhysicalResource.hpp
        include/HG/Utils/Platform.hpp
    src/StringTools.cpp
        include/HG/Utils/StringTools.hpp
    src/Loaders/StringLoader.cpp
        include/HG/Utils/Loaders/StringLoader.hpp
    src/Loaders/AssimpLoader.cpp
        include/HG/Utils/Loaders/AssimpLoader.hpp
    src/Model.cpp
        include/HG/Utils/Model.hpp
        include/HG/Utils/FutureHandler.hpp
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