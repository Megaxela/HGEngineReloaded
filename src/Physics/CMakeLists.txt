cmake_minimum_required(VERSION 3.9)

option(HG_BUILD_PHYSICS_TESTS "Build physics tests" Off)

IF (${HG_BUILD_PHYSICS_TESTS})
    message(STATUS "No tests currently available for Physics module")
endif()

add_subdirectory(Base)
add_subdirectory(PlayRho)