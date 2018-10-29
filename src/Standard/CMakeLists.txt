cmake_minimum_required(VERSION 3.9)
project(HGStandard)

set(CMAKE_CXX_STANDARD 17)

option(HG_BUILD_STANDARD_TESTS "Build core tests" Off)

if (${HG_BUILD_STANDARD_TESTS})
    add_subdirectory(tests)
endif()

add_library(HGStandard STATIC
    src/Behaviours/FPSCameraMovement.cpp
        include/HG/Standard/Behaviours/FPSCameraMovement.hpp
    src/Behaviours/ServiceInformationOverlay.cpp
        include/HG/Standard/Behaviours/ServiceInformationOverlay.hpp
    src/FilesystemResourceAccessor.cpp
        include/HG/Standard/FilesystemResourceAccessor.hpp
    src/Behaviours/TiledMap.cpp
        include/HG/Standard/Behaviours/TiledMap.hpp
    src/Behaviours/DebugControllerOverlay.cpp
        include/HG/Standard/Behaviours/DebugControllerOverlay.hpp
    src/Behaviours/TiledMapRenderer.cpp
        include/HG/Standard/Behaviours/TiledMapRenderer.hpp
    src/Behaviours/TiledMapCollider.cpp
        include/HG/Standard/Behaviours/TiledMapCollider.hpp
    src/Behaviours/IngameConsole.cpp
        include/HG/Standard/Behaviours/IngameConsole.hpp
)

target_include_directories(HGStandard PUBLIC
    include
)

target_link_libraries(HGStandard
    HGCore
    HGUtils
    HGPhysicsBase
    ALogger
)

target_compile_definitions(HGStandard PUBLIC
    HG_HAS_STANDARD
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGStandard
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