cmake_minimum_required(VERSION 3.9)

option(HG_BUILD_EXAMPLES "Build project example" Off)

add_subdirectory(src)

if (${HG_BUILD_EXAMPLES})
    add_subdirectory(example)
endif()

if (DEFINED HG_TEST_CASES)
    add_subdirectory(testing)
endif()