cmake_minimum_required(VERSION 3.9)
project(HGPhysicsBase)

set(CMAKE_CXX_STANDARD 17)

add_library(HGPhysicsBase STATIC
    src/PhysicsController.cpp
    include/PhysicsController.hpp
)

target_link_libraries(HGPhysicsBase
    HGCore
)

target_include_directories(HGPhysicsBase PUBLIC
    include
)