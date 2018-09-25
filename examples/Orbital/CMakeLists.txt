cmake_minimum_required(VERSION 3.9)
project(HGExampleOrital)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleOrital
    main.cpp
    Assets/Scenes/OrbitalScene.cpp
    Assets/Scenes/OrbitalScene.hpp
    Assets/Materials/PlainMaterial.cpp
    Assets/Materials/PlainMaterial.hpp
)

target_include_directories(HGExampleOrital PRIVATE .)

target_link_libraries(HGExampleOrital
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/Models/icosphere.obj Assets/Models/icosphere.obj COPYONLY)
configure_file(Assets/Models/icosphere.mtl Assets/Models/icosphere.mtl COPYONLY)