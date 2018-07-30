cmake_minimum_required(VERSION 3.9)
project(HGEngineReloaded)

#set(CMAKE_CXX_FLAGS -O3)

option(HG_BUILD_ALL_TESTS "Build all tests" Off)

if (${HG_BUILD_ALL_TESTS})

    set(HG_BUILD_CORE_TESTS      On CACHE BOOL "Enable core tests"   FORCE)
    set(HG_BUILD_UTILS_TESTS     On CACHE BOOL "Enable utils tests"  FORCE)
    set(HG_BUILD_RENDERING_TESTS On CACHE BOOL "Enable render tests" FORCE)
    set(HG_BUILD_STANDARD_TESTS  On CACHE BOOL "Enable std tests"    FORCE)

endif()

add_subdirectory(Core)
add_subdirectory(Utils)
add_subdirectory(Rendering)
add_subdirectory(Standard)
add_subdirectory(Physics)