function(describe_module MODULE_NAME DEPENDENCIES ADDITIONAL_INCLUDE)

    # Marking project
    project(HG${MODULE_NAME})

    # Setting C++ standard to C++17
    set(CMAKE_CXX_STANDARD 17)

    # Creating option for building tests for this module if tests even exists
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/tests)
        message(STATUS "Tests for ${MODULE_NAME} found.")
        option(HG_BUILD_${MODULE_NAME}_TESTS "Build ${MODULE_NAME} tests" Off)

        if (${HG_BUILD_${MODULE_NAME}_TESTS})
            add_subdirectory(tests)
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

endfunction(describe_module)