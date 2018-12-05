project(HGEngineReloaded)

option(HG_BUILD_ALL_TESTS "Build all tests" Off)
option(HG_BUILD_WARNINGS  "Enable many build warnings" Off)

add_subdirectory(Core)
add_subdirectory(Utils)
add_subdirectory(Rendering)
add_subdirectory(Standard)
add_subdirectory(Physics)
add_subdirectory(Tools)