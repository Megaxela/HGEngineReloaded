cmake_minimum_required(VERSION 3.9)
project(HGPhysicsPlayRho)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libraries)

add_library(HGPhysicsPlayRho STATIC
    src/Behaviours/Rigidbody.cpp
        include/HG/Physics/PlayRho/Behaviours/Rigidbody.hpp
    src/Controller.cpp
        include/HG/Physics/PlayRho/Controller.hpp
        include/HG/Physics/PlayRho/TiledMap/Creator.hpp
        include/HG/Physics/PlayRho/TiledMap/Remover.hpp
    src/DebugDraw.cpp
        include/HG/Physics/PlayRho/DebugDraw.hpp
        include/HG/Physics/PlayRho/DebugSettings.hpp
    src/Behaviours/DebugControllerBehaviour.cpp
        include/HG/Physics/PlayRho/Behaviours/DebugControllerBehaviour.hpp
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