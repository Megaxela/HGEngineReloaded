cmake_minimum_required(VERSION 3.9)

option(HG_BUILD_RENDERING_TESTS "Build core tests" Off)

if (${HG_BUILD_RENDERING_TESTS})
    message(STATUS "No tests currently available for Rendering module")
endif()

add_subdirectory(Base)
add_subdirectory(OpenGL)