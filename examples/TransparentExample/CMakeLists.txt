cmake_minimum_required(VERSION 3.9)
project(HGExampleTransparent)

set(CMAKE_CXX_STANDARD 17)

add_executable(HGExampleTransparent
    main.cpp
    Assets/Scenes/TransparentScene.cpp
    Assets/Scenes/TransparentScene.hpp
    Assets/Materials/GlassMaterial.cpp
    Assets/Materials/GlassMaterial.hpp
)

target_include_directories(HGExampleTransparent PRIVATE .)

target_link_libraries(HGExampleTransparent
    HGCore
    HGUtils
    HGRenderingOGL
    HGRenderingBase
    HGStandard
)

configure_file(Assets/Models/bottle.obj Assets/Models/bottle.obj COPYONLY)