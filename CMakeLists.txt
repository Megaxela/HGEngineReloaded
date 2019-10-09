cmake_minimum_required(VERSION 3.9)

include(cmake/ModuleSystem.cmake)
include(cmake/PrepareProjects.cmake)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/cmake/
    ${CMAKE_CURRENT_LIST_DIR}/cmake/external/sanitizers-cmake/cmake/
)

option(HG_BUILD_EXAMPLES "Build project example" Off)
option(HG_BUILD_TESTS    "Build tests"           Off)
option(HG_TEST_COVERAGE  "Enables test coverage" Off)

# Clearing testing cached variables

clear_cached_variables()

add_subdirectory(src)

if (${HG_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()

if (${HG_BUILD_TESTS})
    add_subdirectory(testing)
endif()
