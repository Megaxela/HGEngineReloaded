project(HGEngineReloaded)

option(HG_BUILD_ALL_TESTS "Build all tests" Off)
option(HG_BUILD_WARNINGS  "Enable many build warnings" Off)

add_subdirectory(Core)
add_subdirectory(Utils)
add_subdirectory(Rendering)
add_subdirectory(Standard)
add_subdirectory(Physics)
add_subdirectory(System)
add_subdirectory(Networking)
add_subdirectory(Tools)


# Build standard package
#pack_package(
#    NAME Standard
#    PATH ${CMAKE_CURRENT_LIST_DIR}/Standard
#    AUTHOR DiveBrain
#    VERSION_MAJOR 1
#    VERSION_MINOR 0
#)
