add_executable(HGEngineTests
    ${HG_TEST_CASES}
    main.cpp
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(HGEngineTests
    -pthread
    ${GTEST_BOTH_LIBRARIES}
    ${HG_TEST_LIBS}
)

configure_file(${HG_TEST_ASSETS})