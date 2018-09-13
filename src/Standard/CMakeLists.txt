cmake_minimum_required(VERSION 3.9)
project(HGStandard)

set(CMAKE_CXX_STANDARD 17)

option(HG_BUILD_STANDARD_TESTS "Build core tests" Off)

if (${HG_BUILD_STANDARD_TESTS})
    add_subdirectory(tests)
endif()

add_library(HGStandard STATIC
    src/Behaviours/FPSCameraMovement.cpp
    include/Behaviours/FPSCameraMovement.hpp
    src/Behaviours/ServiceInformationOverlay.cpp
    include/Behaviours/ServiceInformationOverlay.hpp
    src/FilesystemResourceAccessor.cpp
    include/FilesystemResourceAccessor.hpp
    src/Behaviours/TiledMap.cpp
    include/Behaviours/TiledMap.hpp
    src/Behaviours/DebugControllerOverlay.cpp
    include/Behaviours/DebugControllerOverlay.hpp
    src/Behaviours/TiledMapRenderer.cpp
    include/Behaviours/TiledMapRenderer.hpp
    src/Behaviours/TiledMapCollider.cpp
    include/Behaviours/TiledMapCollider.hpp
    src/Behaviours/IngameConsole.cpp
    include/Behaviours/IngameConsole.hpp
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