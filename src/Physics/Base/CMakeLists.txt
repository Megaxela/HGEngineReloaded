cmake_minimum_required(VERSION 3.9)
project(HGPhysicsBase)

set(CMAKE_CXX_STANDARD 17)

add_library(HGPhysicsBase STATIC
    src/PhysicsController.cpp
        include/HG/Physics/Base/PhysicsController.hpp
)

target_link_libraries(HGPhysicsBase
    HGCore
)

target_include_directories(HGPhysicsBase PUBLIC
    include
)

target_compile_definitions(HGPhysicsBase PUBLIC
    HG_HAS_PHYSICS_BASE=1
)

if (${HG_BUILD_WARNINGS})
    target_compile_options(HGPhysicsBase
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