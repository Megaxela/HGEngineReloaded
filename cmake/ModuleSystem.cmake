function(describe_tool NAME DEPENDENCIES ADDITIONAL_INCLUDE)

    # Marking project
    project(HG${NAME})

    # Setting C++ standard to C++17
    set(CMAKE_CXX_STANDARD 17)

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/libraries)
        add_subdirectory(libraries)
    endif()

    # Globbing sources and headers
    file(GLOB_RECURSE ${MODULE_NAME}_SOURCES src/*.cpp)
    file(GLOB_RECURSE ${MODULE_NAME}_HEADERS include/*.hpp)

    # Creating executable
    add_executable(${PROJECT_NAME} ${${MODULE_NAME}_SOURCES})

    # Include headers directory
    target_include_directories(${PROJECT_NAME} PUBLIC include ${ADDITIONAL_INCLUDE})

    # Linking libraries
    target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})

    if (${HG_BUILD_WARNINGS})
        target_compile_options(${PROJECT_NAME}
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

endfunction(describe_tool)


function(describe_module MODULE_NAME DEPENDENCIES ADDITIONAL_INCLUDE)

    # Marking project
    project(HG${MODULE_NAME})

    # Setting C++ standard to C++17
    set(CMAKE_CXX_STANDARD 17)

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/libraries)
        add_subdirectory(libraries)
    endif()

    # Creating option for building tests for this module if tests even exists
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/tests)
        message(STATUS "Tests for ${MODULE_NAME} found.")

        set(TEST_KEY "HG_BUILD_${MODULE_NAME}_TESTS")
        option(${TEST_KEY} "Build ${MODULE_NAME} tests" Off)

        # If required - include tests
        if (${${TEST_KEY}} OR ${HG_BUILD_ALL_TESTS})

            # Finding all tests
            file(GLOB TEST_FILES tests/*.cpp)

            set(HG_TEST_CASES ${HG_TEST_CASES} "${TEST_FILES}" CACHE INTERNAL "Test cases files")

            # Setting current module as dependency
            set(HG_TEST_LIBS ${HG_TEST_LIBS} "${PROJECT_NAME}" CACHE INTERNAL "Test libraries")

            # Finding static testing assets
            file(GLOB_RECURSE TEST_ASSETS tests/static/*)

            # Setting it to global variable
            set(${PROJECT_NAME}_TEST_ASSETS
                ${CMAKE_CURRENT_LIST_DIR}/tests/static/
                ${TEST_ASSETS}

                CACHE INTERNAL "" FORCE
            )

            # Adding this variable to var list
            set(TEST_ASSETS_VARIABLES
                ${TEST_ASSETS_VARIABLES}
                ${PROJECT_NAME}_TEST_ASSETS

                CACHE INTERNAL "" FORCE
            )

        endif()
    endif()

    # Globbing sources and headers
    file(GLOB_RECURSE ${MODULE_NAME}_SOURCES src/*.cpp)
    file(GLOB_RECURSE ${MODULE_NAME}_HEADERS include/*.hpp)

    # Creating static library
    add_library(${PROJECT_NAME} STATIC ${${MODULE_NAME}_SOURCES})

    # Include headers directory
    target_include_directories(${PROJECT_NAME} PUBLIC include ${ADDITIONAL_INCLUDE})

    # Linking libraries
    target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})

    if (${HG_BUILD_WARNINGS})
        target_compile_options(${PROJECT_NAME}
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

    # Adding this module to global variable with all projects
    if (NOT PROJECT_NAME IN_LIST MODULES_LIST)
        set(MODULES_LIST ${MODULES_LIST} "${PROJECT_NAME}" CACHE INTERNAL "List of engine modules")
    endif()

endfunction(describe_module)

function(clear_cached_variables)
    set(HG_TEST_CASES  "" CACHE STRING "" FORCE)
    set(HG_TEST_LIBS   "" CACHE STRING "" FORCE)
    set(HG_TEST_ASSETS "" CACHE STRING "" FORCE)

    foreach (VARIABLE ${TEST_ASSETS_VARIABLES})
        set(${VARIABLE} "" CACHE LIST "" FORCE)
    endforeach()

    set(TEST_ASSETS_VARIABLES "" CACHE LIST "" FORCE)
endfunction(clear_cached_variables)