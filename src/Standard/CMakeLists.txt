cmake_minimum_required(VERSION 3.9)
project(HGStandard)

set(CMAKE_CXX_STANDARD 17)

add_library(HGStandard STATIC
    src/MeshLoaders/OBJ.cpp
    include/MeshLoaders/OBJ.hpp
    src/Behaviours/FPSCameraMovement.cpp
    include/Behaviours/FPSCameraMovement.hpp
        src/Behaviours/ServiceInformationOverlay.cpp
        include/Behaviours/ServiceInformationOverlay.hpp
        src/FilesystemResourceAccessor.cpp include/FilesystemResourceAccessor.hpp)

target_include_directories(HGStandard PUBLIC
    include
)

target_link_libraries(HGStandard
    HGCore
    HGUtils
    ALogger
)

target_compile_definitions(HGStandard
    PUBLIC -DSTD_MODULE_NS=HG::Standard
)