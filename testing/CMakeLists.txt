# C++ STD
set(CMAKE_CXX_STANDARD 17)

# Searching for testing core files
file(GLOB_RECURSE TESTING_CORE_SOURCES *.cpp)

# Creating test executable
add_executable(HGEngineTests
    ${HG_TEST_CASES}
    ${TESTING_CORE_SOURCES}
)

# Adding sanitizing to tests
set(SANITIZE_UNDEFINED On)
set(SANITIZE_MEMORY On)

add_sanitize_undefined(HGEngineTests)
add_sanitize_memory(HGEngineTests)

# Enabling ctest testing
enable_testing()

# Finding GTest
find_package(GTest REQUIRED)

# Including GTest
include_directories(
    include
    ${GTEST_INCLUDE_DIRS}
)

# Linking required libraries
target_link_libraries(HGEngineTests
    -pthread
    ${GTEST_BOTH_LIBRARIES}
    ${HG_TEST_LIBS}
)

# Enabling test coverate if required
if (${HG_TEST_COVERAGE})
    message(STATUS "Enabling test coverage for HGEngineTests")
    target_compile_options(HGEngineTests BEFORE
        PUBLIC
            -fprofile-arcs
            -ftest-coverage
    )
endif()

if (EMSCRIPTEN)
    target_link_options(HGEngineTests BEFORE
        PUBLIC
            SHELL:-s\ ERROR_ON_UNDEFINED_SYMBOLS=0
            SHELL:-s\ DISABLE_EXCEPTION_CATCHING=0
            SHELL:-s\ EXIT_RUNTIME=1
            SHELL:-s\ TOTAL_MEMORY=104857600 # 100 MB
    )

    target_compile_options(HGEngineTests BEFORE
        PUBLIC
            SHELL:-s\ DISABLE_EXCEPTION_CATCHING=0
            SHELL:-s\ TOTAL_MEMORY=104857600 # 100 MB
    )

    set_target_properties(HGEngineTests
        PROPERTIES
            SUFFIX ".html"
    )
endif()

# Copying test assets
foreach(ASSET_VARIABLE ${TEST_ASSETS_VARIABLES})

    # Getting path to static directory
    list(GET ${ASSET_VARIABLE} 0 STATIC_PATH)

    # Removing this path from list
    list(REMOVE_AT ${ASSET_VARIABLE} 0)

    # Getting elements
    foreach(ASSET_FULL_PATH ${${ASSET_VARIABLE}})

        # Cutting path to static directory from asset path
        string(REPLACE ${STATIC_PATH} "" ASSET_RELATIVE_PATH ${ASSET_FULL_PATH})

        message(STATUS "Copying test asset file \"${ASSET_RELATIVE_PATH}\"")

        # Configuring
        configure_file(${ASSET_FULL_PATH} ${ASSET_RELATIVE_PATH} COPYONLY)

    endforeach()

endforeach()
