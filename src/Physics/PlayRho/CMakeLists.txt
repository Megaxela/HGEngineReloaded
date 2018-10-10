cmake_minimum_required(VERSION 3.9)
project(HGPhysicsPlayRho)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libraries)

add_library(HGPhysicsPlayRho STATIC
    src/Behaviours/Rigidbody.cpp
    include/Behaviours/Rigidbody.hpp
    src/Controller.cpp
    include/Controller.hpp
    include/TiledMap/Creator.hpp
    include/TiledMap/Remover.hpp
    src/DebugDraw.cpp
    include/DebugDraw.hpp
    include/DebugSettings.hpp
    src/Behaviours/DebugControllerBehaviour.cpp
    include/Behaviours/DebugControllerBehaviour.hpp
)

target_link_libraries(HGPhysicsPlayRho
    PlayRho
    HGCore
    HGRenderingBase
)

target_include_directories(HGPhysicsPlayRho PUBLIC
    include
)

target_compile_definitions(HGPhysicsPlayRho PUBLIC
    HG_HAS_PHYSICS_PLAYRHO=1
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGPhysicsPlayRho
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