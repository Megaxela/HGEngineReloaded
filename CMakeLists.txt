cmake_minimum_required(VERSION 3.9)

option(HG_BUILD_EXAMPLES "Build project example" Off)

# Clearing testing cached variables
set(HG_TEST_CASES  "" CACHE STRING "" FORCE)
set(HG_TEST_LIBS   "" CACHE STRING "" FORCE)
set(HG_TEST_ASSETS "" CACHE STRING "" FORCE)

add_subdirectory(src)

if (${HG_BUILD_EXAMPLES})
    add_subdirectory(example)
endif()

if (DEFINED HG_TEST_CASES)
    add_subdirectory(testing)
endif()