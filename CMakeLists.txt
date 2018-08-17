cmake_minimum_required(VERSION 3.9)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/)

option(HG_BUILD_EXAMPLES "Build project example" Off)
option(HG_BUILD_TESTS    "Build tests"           Off)

# Clearing testing cached variables
set(HG_TEST_CASES  "" CACHE STRING "" FORCE)
set(HG_TEST_LIBS   "" CACHE STRING "" FORCE)
set(HG_TEST_ASSETS "" CACHE STRING "" FORCE)

add_subdirectory(src)

if (${HG_BUILD_EXAMPLES})
    add_subdirectory(example)
endif()

if (${HG_BUILD_TESTS})
    add_subdirectory(testing)
endif()